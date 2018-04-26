/*
 *  Image.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Image__
#define __Image__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented base image interface.
	class Image : public GReference
	{
	public:
		// constructor
		Image();

		// destructor
		virtual ~Image();		

		// set/get the width of the image
		VOID Width(U32 w);
		U32 Width() const;

		// set/get the height of the image
		VOID Height(U32 h);
		U32 Height() const;

		// set/get the depth of the image
		VOID Depth(U32 d);
		U32 Depth() const;

		// set/get the format of the pixel
		VOID PixelFormat(U32 f);
		U32 PixelFormat() const;

		// set/get the data type of the pixel
		VOID DataType(U32 t);
		U32 DataType() const;

		// set/get the count of the mipmap
		VOID MipmapCount(U32 n);
		U32 MipmapCount() const;

		// set/get the data of the mipmap
		VOID Mipmap(U32 level, const U8*data, U32 size);
		const U8* Mipmap(U32 level, U32& size) const;
		const U8* Mipmap(U32 level) const;

	public:
		// load the image
		static Image* Load(const GData* data);
	
	private:
		static Image* LoadTGA(const GData* data);
		static Image* LoadDDS(const GData* data);

	public:
		U32	mWidth, mHeight, mDepth;
		U32	mPixelFormat, mDataType;
		std::vector< std::vector<U8> >mMipmaps;	
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Image>ImagePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Image__

//////////////////////////////////////////////////////////////////////////