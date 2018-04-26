//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////	

	Context::Context() 
	{
		GUARD(Context::Context);

		UNGUARD;
	}

	Context::~Context() 
	{
		GUARD(Context::~Context);

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Reset
	====================
	*/
	VOID Context::Reset()
	{
		GUARD(Context::Reset);

		glEnable(GL_SCISSOR_TEST);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glClearDepthf(1.0f);
		glDepthRangef(0.0f, 1.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		mFBO = 0;
		mRBO = 0;
		mProgram = 0;
		mVertexObject = 0;
		mVertexAttribCount = 0;
		mIndexObject = 0;
		mTextureUnit = 0;
		memset(mTextureObject, 0, sizeof(mTextureObject));
		mDepthFunc = GL_LEQUAL;	
		mDepthMask = TRUE;
		mBlendSrc = GL_ONE;
		mBlendDst = GL_ZERO;
		mCullMode = GL_NONE;

		UNGUARD;
	}

	/*
	====================
	Exec
	====================
	*/
	VOID Context::Exec(const Operation * op)
	{
		GUARD(Context::Exec);

		(*((Operation*)op))(this);

		UNGUARD;
	}

	/*
	====================
	Free
	====================
	*/
	VOID Context::Free()
	{
		GUARD(Context::Free);

		// delete all of the render object of the context
		// GTHREAD_LOCK(&mUnusedMutex);
		for(U32 i = 0; i < mUnuseds.size(); i++)
		{
			std::pair<U32,U32>&res = mUnuseds[i];
			switch(res.first)
			{
			case ROT_TEXTURE:
				glDeleteTextures(1, &res.second);
				break;
			case ROT_PROGRAM:
				glDeleteProgram(res.second);
				break;
			case ROT_RENDER_BUFFER:
				glDeleteRenderbuffers(1, &res.second);
				break;
			case ROT_FRAME_BUFFER:
				glDeleteFramebuffers(1, &res.second);
				break;
			case ROT_INDEX_BUFFER:
				glDeleteBuffers(1, &res.second);
				break;
			case ROT_VERTEX_BUFFER:
				glDeleteBuffers(1, &res.second);
				break;
			}
		}
		mUnuseds.clear();

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Release

	release the render object of the context
	====================
	*/	
	VOID Context::Release(U32 type, U32 id)
	{
		GUARD(Context::Release);
		
		// GTHREAD_LOCK(&mUnusedMutex)
		mUnuseds.push_back(std::pair<U32,U32>(type, id));

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	BindFramebuffer
	====================
	*/
	VOID Context::BindFramebuffer(U32 fbo)
	{
		GUARD(Context::BindFramebuffer);

		if(fbo != mFBO)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			mFBO = fbo;
		}

		UNGUARD;
	}

	/*
	====================
	BindRenderbuffer
	====================
	*/
	VOID Context::BindRenderbuffer(U32 rbo)
	{
		GUARD(Context::BindRenderbuffer);

		if(rbo != mRBO)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			mRBO = rbo;
		}

		UNGUARD;
	}

	/*
	====================
	BindProgram
	====================
	*/
	VOID Context::BindProgram(U32 program)
	{
		GUARD(Context::BindProgram);

		if(program != mProgram)
		{
			glUseProgram(program);
			mProgram = program;
		}

		UNGUARD;
	}

	/*
	====================
	BindTexture
	====================
	*/	
	VOID Context::BindTexture(U32 target, U32 object, U32 unit)
	{
		GUARD(Context::BindTexture);

		CHECK(unit < MAX_TEXTURE_UNITS);

		// active the texture unit
		if(unit != mTextureUnit)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			mTextureUnit = unit;
		}

		// bind textures to texture image units as needed
		if(object != mTextureObject[unit])
		{
			glBindTexture(target, object);
			mTextureObject[unit] = object;
		}

		UNGUARD;
	}

	/*
	====================
	BindVertexBuffer
	====================
	*/
	VOID Context::BindVertexBuffer(U32 object, U32 stride, const U8* attribs)
	{
		GUARD(Context::BindVertexBuffer);

		if(object == mVertexObject) return;

		// bind the vertex buffer object
		if(object == 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else 
		{
			CHECK(attribs);

			// get the attribute count
			U32 attrib_count = attribs[0];

			// enable the vertex attrib array
			if(mVertexAttribCount < attrib_count) { for(U32 i = mVertexAttribCount; i < attrib_count; i++) glEnableVertexAttribArray(i); }
			if(mVertexAttribCount > attrib_count) { for(U32 i = attrib_count; i < mVertexAttribCount; i++) glDisableVertexAttribArray(i); }
			mVertexAttribCount = attrib_count;

			// bind the buffer
			glBindBuffer(GL_ARRAY_BUFFER, object);

			// bind the attribute
			U8 *attrib_ptr = NULL;
			for(U32 i = 1; i <= attrib_count; i++)
			{
				U32 component; U32 data_type; U32 data_size;
				switch(attribs[i])
				{
				case VertexBuffer::VT_1I:
					component = 1;
					data_type = GL_INT;
					data_size = component * sizeof( I32 );
					break;
				case VertexBuffer::VT_2I:
					component = 2;
					data_type = GL_INT;
					data_size = component * sizeof( I32 );
					break;
				case VertexBuffer::VT_3I:
					component = 3;
					data_type = GL_INT;
					data_size = component * sizeof( I32 );
					break;
				case VertexBuffer::VT_4I:
					component = 4;
					data_type = GL_INT;
					data_size = component * sizeof( I32 );
					break;
				case VertexBuffer::VT_1F:
					component = 1;
					data_type = GL_FLOAT;
					data_size = component * sizeof( F32 );
					break;
				case VertexBuffer::VT_2F:
					component = 2;
					data_type = GL_FLOAT;
					data_size = component * sizeof( F32 );
					break;
				case VertexBuffer::VT_3F:
					component = 3;
					data_type = GL_FLOAT;
					data_size = component * sizeof( F32 );
					break;
				case VertexBuffer::VT_4F:
					component = 4;
					data_type = GL_FLOAT;
					data_size = component * sizeof( F32 );
					break;
				default:
					GAssert("The type of attrib is error!\n");
					break;
				}
				glVertexAttribPointer(i-1, component, data_type, GL_FALSE, stride, attrib_ptr);
				attrib_ptr += data_size;
			}
		}		
		mVertexObject = object;
		UNGUARD;
	}

	/*
	====================
	BindIndexBuffer
	====================
	*/	
	VOID Context::BindIndexBuffer(U32 object)
	{
		GUARD(Context::BindIndexBuffer);

		if(object != mIndexObject)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object);
			mIndexObject = object;
		}

		UNGUARD;
	}

	/*
	====================
	BindBlendFunc
	====================
	*/	
	VOID Context::BindBlendFunc(U32 src, U32 dst)
	{
		GUARD(Context::BindBlendFunc);

		if(src == mBlendSrc && dst == mBlendDst) return;

		if(src == GL_ONE && dst == GL_ZERO)
		{
			glDisable(GL_BLEND);
		}
		else
		{
			if(mBlendSrc == GL_ONE && mBlendDst == GL_ZERO)
			{
				glEnable(GL_BLEND);
			}

			glBlendFunc(src, dst);
		}

		mBlendSrc = src, mBlendDst = dst;

		UNGUARD;
	}
	
	/*
	====================
	BindDepthFunc
	====================
	*/	
	VOID Context::BindDepthFunc(U32 func)
	{
		GUARD(Context::BindDepthFunc);

		if(func == mDepthFunc) return;

		if(func == GL_ALWAYS) 
		{
			glDisable(GL_DEPTH_TEST);
		} 
		else 
		{
			if(mDepthFunc == GL_ALWAYS)
			{
				glEnable(GL_DEPTH_TEST);
			}

			glDepthFunc(func);
		}

		mDepthFunc = func;

		UNGUARD;
	}

	/*
	====================
	BindDepthMask
	====================
	*/	
	VOID Context::BindDepthMask(BOOL mask)
	{
		GUARD(Context::BindDepthMask);

		if(mask != mDepthMask)
		{
			glDepthMask(mask ? GL_TRUE : GL_FALSE);
			mDepthMask = mask;
		}

		UNGUARD;
	}

	/*
	====================
	BindCullFunc
	====================
	*/	
	VOID Context::BindCullFunc(U32 mode)
	{
		GUARD(Context::BindCullFunc);

		if(mode == mCullMode) return;

		if(mode == GL_NONE)
		{
			glDisable(GL_CULL_FACE);
		} 
		else 
		{
			if(mCullMode == GL_NONE) 
			{
				glEnable(GL_CULL_FACE);
			}

			glCullFace(mode);
		}

		mCullMode = mode;

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
