/*
 *  BaseTexture.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __BaseTexture__
#define __BaseTexture__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	enum PIXEL_FORMAT
	{
		PF_ALPHA		= 0x1906,
		PF_RGB			= 0x1907,
		PF_RGBA			= 0x1908,
		PF_L			= 0x1909,
		PF_LA			= 0x190A,
	};

	enum DATA_TYPE
	{
		DT_BYTE						= 0x1400,
		DT_UNSIGNED_BYTE			= 0x1401,
		DT_SHORT					= 0x1402,
		DT_UNSIGNED_SHORT			= 0x1403,
		DT_INT						= 0x1404,
		DT_UNSIGNED_INT				= 0x1405,
		DT_FLOAT					= 0x1406,
		DT_UNSIGNED_SHORT_4_4_4_4	= 0x8033,
		DT_UNSIGNED_SHORT_5_5_5_1	= 0x8034,
		DT_UNSIGNED_SHORT_5_6_5		= 0x8363,
	};

	//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented base texture interface.
	class BaseTexture : public GReference
	{
	public:		
		// constructor
		BaseTexture();
		
		// destructor
		virtual ~BaseTexture();

		// load the data
		virtual VOID Load(const Image* image);

		// get the width of the texture		
		U32 Width() const;

		// get the height of the texture		
		U32 Height() const;

		// set/get the wrap of the texture
		VOID WrapS(U32 s);
		U32 WrapS() const;
		VOID WrapT(U32 t);
		U32 WrapT() const;

		// set/get the filter of the texture
		VOID MinFilter(U32 min);
		U32 MinFilter() const;
		VOID MagFilter(U32 mag);
		U32 MagFilter() const;

	private:
		// bind the texture
		virtual VOID Bind(Context * rc, U32 unit = 0);

		// update the texture
		virtual VOID Update(const Image* image, BOOL created) = 0;	
			
	protected:
		GPtr<class Context>mRCPtr;
		ImagePtr mImagePtr;
		U32	mObject, mTarget;
		U32	mWidth, mHeight;
		U32 mWrapS, mWrapT;
		U32 mMinFilter, mMagFilter;
		BOOL mDirty;

	private:friend class Shader;
			friend class Target;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<BaseTexture>BaseTexturePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __BaseTexture__

//////////////////////////////////////////////////////////////////////////