//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	Target::Target(U32& fbo) 
	{
		GUARD(Target::Target);

		mFBO = fbo;
		mRBO = 0;
		mColorPtr = NULL;
		mFace = 0;
		mUnused = FALSE;

		UNGUARD;
	}

	Target::Target(const BaseTexture*color, U32 face) 
	{
		GUARD(Target::Target);

		mFBO = 0;
		mRBO = 0;		
		mColorPtr = (BaseTexture*)color;
		CHECK(mColorPtr);
		mFace = face;
		mUnused = TRUE;

		UNGUARD;
	}

	Target::~Target() 
	{
		GUARD(Target::~Target);

		if(mRCPtr) 
		{ 
			if(mFBO) { mRCPtr->Release(ROT_FRAME_BUFFER, mFBO); } 
			if(mRBO) { mRCPtr->Release(ROT_RENDER_BUFFER, mRBO); }
		}

		UNGUARD;
	}


	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	operator
	====================
	*/
	VOID Target::operator() (Context*rc)
	{
		GUARD(Target::operator());	

		// check the context
		if(mRCPtr == NULL) mRCPtr = rc;
		CHECK(mRCPtr == rc);

		// if the fbo is unused?
		if(mUnused)
		{
			// create the fbo
			if(mFBO == 0) { glGenFramebuffers(1, &mFBO); CHECK(mFBO); }
			rc->BindFramebuffer(mFBO);

			// attach the color texture
			CHECK(mColorPtr);
			mColorPtr->Bind(rc);
			GLenum target = mColorPtr->mTarget;
			GLuint object = mColorPtr->mObject;
			switch(target)
			{
			case GL_TEXTURE_2D:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, object, 0);
				break;
			case GL_TEXTURE_CUBE_MAP:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + mFace, object, 0);
				break;
			default:
				GAssert( "The color texture target is unknown.\n" );
				break;
			}

			// attach the depth render buffer
			if(mRBO == 0) { glGenRenderbuffers(1, &mRBO); CHECK(mRBO); }
			rc->BindRenderbuffer(mRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mColorPtr->Width(), mColorPtr->Height());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO);

			// check the frame buffer status
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			switch(status)
			{
			case GL_FRAMEBUFFER_COMPLETE:
				// GAssert("GL_FRAMEBUFFER_COMPLETE");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				GAssert("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				GAssert("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
				GAssert("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				GAssert("GL_FRAMEBUFFER_UNSUPPORTED");
				break;
			default:
				GAssert("Unknown FBO error");
				break;
			}

			mUnused = FALSE;
		}
		else
		{
			// bind the fbo
			rc->BindFramebuffer(mFBO);
		}		

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
