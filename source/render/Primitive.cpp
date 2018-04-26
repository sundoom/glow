//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	Primitive::Primitive() 
	{
		GUARD(Primitive::Primitive);

		mType = PT_TRIANGLES;

		UNGUARD;
	}

	Primitive::~Primitive() 
	{
		GUARD(Primitive::~Primitive);

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	type
	====================
	*/
	VOID Primitive::SetType(U32 t)
	{
		GUARD(Primitive::SetType);
		
		mType = t;

		UNGUARD;
	}
	
	U32 Primitive::GetType() const
	{
		GUARD(Primitive::GetType);

		return mType;

		UNGUARD;
	}

	/*
	====================
	Shader
	====================
	*/
	VOID Primitive::SetShader(const Shader * s, const CHAR* pass)
	{
		GUARD(Primitive::SetShader);
		
		CHECK(s!=NULL&&pass!=NULL);
		if(mShaderPtr != s)
		{
			mShaderPtr = (Shader*)s;
			for(std::map< Str, std::pair<ConstantPtr, HANDLE> >::iterator it = mConstants.begin(); it!=mConstants.end(); ++it)
			{ it->second.second = NULL; }
		}
		mPassName = pass;

		UNGUARD;
	}
	
	const Shader* Primitive::GetShader() const
	{
		GUARD(Primitive::GetShader);
		
		return mShaderPtr.Ptr();

		UNGUARD;
	}

	Shader* Primitive::GetShader()
	{
		GUARD(Primitive::GetShader);

		return mShaderPtr.Ptr();

		UNGUARD;
	}

	/*
	====================
	constant
	====================
	*/
	VOID Primitive::SetConstant(const CHAR* name, const Constant* c)
	{
		GUARD(Primitive::SetConstant);

		CHECK(name&&c);
		if(mConstants.find(name)!=mConstants.end())GAssert(VA("The constant is already exists : %s.",name));
		mConstants.insert(std::make_pair(name, std::make_pair((Constant*)c,(HANDLE)NULL)));

		UNGUARD;
	}

	const Constant* Primitive::GetConstant(const CHAR *name) const
	{
		GUARD(Primitive::GetConstant);

		CHECK(name);
		std::map< Str, std::pair<ConstantPtr, HANDLE> >::const_iterator it = mConstants.find(name);
		if(it == mConstants.end()) return NULL;
		return it->second.first.Ptr();

		UNGUARD;
	}

	Constant* Primitive::GetConstant(const CHAR *name)
	{
		GUARD(Primitive::GetConstant);

		CHECK(name);
		std::map< Str, std::pair<ConstantPtr, HANDLE> >::iterator it = mConstants.find(name);
		if(it == mConstants.end()) return NULL;
		return it->second.first.Ptr();

		UNGUARD;
	}

	/*
	====================
	vertex
	====================
	*/
	VOID Primitive::SetVertexBuffer(const VertexBuffer* vb)
	{
		GUARD(Primitive::SetVertexBuffer);
		
		CHECK(vb);
		mVBPtr = (VertexBuffer*)vb;

		UNGUARD;
	}

	const VertexBuffer* Primitive::GetVertexBuffer() const
	{
		GUARD(Primitive::GetVertexBuffer);
		
		return mVBPtr.Ptr();

		UNGUARD;
	}

	VertexBuffer* Primitive::GetVertexBuffer()
	{
		GUARD(Primitive::GetVertexBuffer);

		return mVBPtr.Ptr();

		UNGUARD;
	}

	/*
	====================
	index
	====================
	*/
	VOID Primitive::SetIndexBuffer(const IndexBuffer* ib)
	{
		GUARD(Primitive::SetIndexBuffer);
		
		CHECK(ib);
		mIBPtr = (IndexBuffer*)ib;

		UNGUARD;
	}

	const IndexBuffer* Primitive::GetIndexBuffer() const
	{
		GUARD(Primitive::GetIndexBuffer);
		
		return mIBPtr.Ptr();

		UNGUARD;
	}

	IndexBuffer* Primitive::GetIndexBuffer()
	{
		GUARD(Primitive::GetIndexBuffer);

		return mIBPtr.Ptr();

		UNGUARD;
	}
	
	/*
	====================
	box
	====================
	*/
	VOID Primitive::SetBox(const BoundingBox& box)
	{
		GUARD(Primitive::SetBox);

		mBox = box;

		UNGUARD;
	}

	const BoundingBox& Primitive::GetBox() const
	{
		GUARD(Primitive::GetBox);

		return mBox;

		UNGUARD;
	}	

	BoundingBox& Primitive::GetBox()
	{
		GUARD(Primitive::GetBox);

		return mBox;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	operator
	====================
	*/
	VOID Primitive::operator() (Context*rc)
	{
		GUARD(Primitive::operator());

		CHECK(mShaderPtr&&mVBPtr&&mIBPtr);

		// bind the vertex buffer
		mVBPtr->Bind(rc);

		// bind the index buffer
		mIBPtr->Bind(rc);
		U32 count = mIBPtr->mCount;
		U32 stride = mIBPtr->mStride;
		GLenum type;
		if(stride == sizeof(U16))type = GL_UNSIGNED_SHORT;
		else if(stride == sizeof(U32)) type = GL_UNSIGNED_INT;
		else GAssert("The index stride is error!");

		// bind the shader
		mShaderPtr->Bind(rc);

		// update the value of the uniform
		for(std::map< Str, std::pair<ConstantPtr, HANDLE> >::iterator it = mConstants.begin(); it!=mConstants.end(); ++it)
		{
			ConstantPtr& constant_ptr = it->second.first;
			HANDLE& handle = it->second.second;
			if(handle == NULL) handle = mShaderPtr->GetUniform(it->first.c_str());
			CHECK(handle);
			switch(constant_ptr->mType)
			{
			case Constant::CT_INT:
				mShaderPtr->Set1iv(handle, constant_ptr->mCount, (I32*)constant_ptr->mData);
				break;
			case Constant::CT_INT_ARRAY:
				mShaderPtr->Set1iv(handle, constant_ptr->mCount, (I32*)constant_ptr->mData);
				break;
			case Constant::CT_FLOAT:
				mShaderPtr->Set1fv(handle, constant_ptr->mCount, (F32*)constant_ptr->mData);
				break;
			case Constant::CT_FLOAT_ARRAY:
				mShaderPtr->Set1fv(handle, constant_ptr->mCount, (F32*)constant_ptr->mData);
				break;
			case Constant::CT_VECTOR:
				mShaderPtr->Set4fv(handle, constant_ptr->mCount, (F32*)constant_ptr->mData);
				break;
			case Constant::CT_VECTOR_ARRAY:
				mShaderPtr->Set4fv(handle, constant_ptr->mCount, (F32*)constant_ptr->mData);
				break;
			case Constant::CT_MATRIX:
				mShaderPtr->SetMatrix4fv(handle, constant_ptr->mCount, (F32*)constant_ptr->mData);
				break;
			case Constant::CT_MATRIX_ARRAY:
				mShaderPtr->SetMatrix4fv(handle, constant_ptr->mCount, (F32*)constant_ptr->mData);
				break;
			case Constant::CT_TEXTURE:
				mShaderPtr->SetSampler(handle, constant_ptr->mTexturePtr.Ptr());
				break;
			default:
				GAssert(VA("The constant type is error : %s", it->first.c_str()));
				break;
			}
		}

		// bind the shader pass
		if(mShaderPtr->BindPass(mPassName.c_str()))
		{
			// draw the elements
			glDrawElements(mType, count, type, 0);
		}

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
