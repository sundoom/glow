//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////		

	VolumeTexture::VolumeTexture()
	{
		GUARD(VolumeTexture::VolumeTexture);

		mTarget = GL_TEXTURE_3D_OES;
		mDepth = 1;
		mWrapR = GL_REPEAT;
		
		UNGUARD;
	}

	VolumeTexture::~VolumeTexture() 
	{
		GUARD(VolumeTexture::~VolumeTexture);		

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID VolumeTexture::Load(const Image* image)
	{
		GUARD(VolumeTexture::Load);

		// store the data
		CHECK(image);
		mImagePtr = (Image*)image;
		mWidth = image->Width();
		mHeight = image->Height();
		mDepth = image->Depth();

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Depth
	====================
	*/
	U32 VolumeTexture::Depth() const
	{
		GUARD(BaseTexture::Depth);

		return mDepth;

		UNGUARD;
	}

	/*
	====================
	WrapR
	====================
	*/
	VOID VolumeTexture::WrapR(U32 r)
	{
		GUARD(VolumeTexture::WrapR);

		if(mWrapR != r) {mWrapR = r; mDirty = TRUE;}

		UNGUARD;
	}

	U32 VolumeTexture::WrapR() const
	{
		GUARD(VolumeTexture::WrapR);

		return mWrapR;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Bind
	====================
	*/
	VOID VolumeTexture::Bind(Context * rc, U32 unit)
	{
		GUARD(VolumeTexture::Bind);
#if 0
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
			glTexParameteri(mTarget, GL_TEXTURE_WRAP_R_OES, mWrapR);
			glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, mMinFilter);
			glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, mMagFilter);
			mDirty = FALSE;
		}
#endif
		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Update
	====================
	*/
	VOID VolumeTexture::Update(const Image* image, BOOL created)
	{
		GUARD(VolumeTexture::Update);		
		
		UNGUARD;
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
