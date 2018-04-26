//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	BaseTexture::BaseTexture()
	{
		GUARD(BaseTexture::BaseTexture);

		mObject = 0;
		mWidth = mHeight = 0;

		mWrapS = GL_REPEAT;
		mWrapT = GL_REPEAT;
		mMinFilter = GL_NEAREST_MIPMAP_LINEAR;
		mMagFilter = GL_LINEAR;
		mDirty = TRUE;
		
		UNGUARD;
	}

	BaseTexture::~BaseTexture() 
	{
		GUARD(BaseTexture::~BaseTexture);
		
		if(mRCPtr) {if(mObject){mRCPtr->Release(ROT_TEXTURE, mObject); mObject = 0;}}

		UNGUARD;
	}	

	//////////////////////////////////////////////////////////////////////////
	
	/*
	====================
	Load
	====================
	*/
	VOID BaseTexture::Load(const Image* image)
	{
		GUARD(BaseTexture::load);

		// store the data
		CHECK(image);
		mImagePtr = (Image*)image;
		mWidth = image->Width();		
		mHeight = image->Height();

		UNGUARD;
	}

	/*
	====================
	Width
	====================
	*/
	U32 BaseTexture::Width() const
	{
		GUARD(BaseTexture::Width);

		return mWidth;

		UNGUARD;
	}

	/*
	====================
	Height
	====================
	*/
	U32 BaseTexture::Height() const
	{
		GUARD(BaseTexture::Height);

		return mHeight;

		UNGUARD;
	}	

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	wrap
	====================
	*/
	VOID BaseTexture::WrapS(U32 s)
	{
		GUARD(BaseTexture::WrapS);

		if(mWrapS != s) {mWrapS = s; mDirty = TRUE;}

		UNGUARD;
	}
	
	U32 BaseTexture::WrapS() const
	{
		GUARD(BaseTexture::WrapS);

		return mWrapS;

		UNGUARD;
	}

	VOID BaseTexture::WrapT(U32 t)
	{
		GUARD(BaseTexture::WrapT);

		if(mWrapT != t) {mWrapT = t; mDirty = TRUE;}

		UNGUARD;
	}

	U32 BaseTexture::WrapT() const
	{
		GUARD(BaseTexture::WrapT);

		return mWrapT;

		UNGUARD;
	}

	/*
	====================
	filter
	====================
	*/
	VOID BaseTexture::MinFilter(U32 min)
	{
		GUARD(BaseTexture::MinFilter);

		if(mMinFilter != min) {mMinFilter = min; mDirty = TRUE;}

		UNGUARD;
	}

	U32 BaseTexture::MinFilter() const
	{
		GUARD(BaseTexture::MinFilter);

		return mMinFilter;

		UNGUARD;
	}

	VOID BaseTexture::MagFilter(U32 mag)
	{
		GUARD(BaseTexture::MagFilter);

		if(mMagFilter != mag) {mMagFilter = mag; mDirty = TRUE;}

		UNGUARD;
	}

	U32 BaseTexture::MagFilter() const
	{
		GUARD(BaseTexture::MagFilter);

		return mMagFilter;

		UNGUARD;
	}	

	//////////////////////////////////////////////////////////////////////////	

	/*
	====================
	Bind
	====================
	*/
	VOID BaseTexture::Bind(Context * rc, U32 unit)
	{
		GUARD(BaseTexture::Bind);

		// check the context
		if(mRCPtr == NULL) mRCPtr = (Context*)rc;
		CHECK(mRCPtr == rc);

		// generate the texture object
		if(mObject == 0) 
		{
			// generate the texture object
			glGenTextures(1, &mObject); CHECK(mObject);

			// bind the texture
			rc->BindTexture(mTarget, mObject, unit);

			// update the texture
			if(mImagePtr){this->Update(mImagePtr.Ptr(), TRUE); mImagePtr.Release();}
		}
		else
		{
			// bind the texture
			rc->BindTexture(mTarget, mObject, unit);

			// update the texture
			if(mImagePtr){this->Update(mImagePtr.Ptr(), FALSE); mImagePtr.Release();}
		}

		// update the texture parameter
		if(mDirty)
		{
			glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, mWrapS);
			glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, mWrapT);
			glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, mMinFilter);
			glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, mMagFilter);
			mDirty = FALSE;
		}	
		
		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
