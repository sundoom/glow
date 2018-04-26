/*
 *  Bitmap.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Bitmap__
#define __Bitmap__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented Bitmap interface.
	class Bitmap : public GReference
	{
	public:
		// constructor
		Bitmap();

		// destructor
		virtual ~Bitmap();

		// set/get the x of the bitmap
		VOID X(I32 x);
		I32 X() const;

		// set/get the y of the bitmap
		VOID Y(I32 y);
		I32 Y() const;

		// set/get the width of the bitmap
		VOID Width(U32 w);
		U32 Width() const;

		// set/get the height of the bitmap
		VOID Height(U32 h);
		U32 Height() const;

		// set/get the format of the bitmap
		VOID Format(U32 f);
		U32 Format() const;

		// set/get the data of the bitmap
		VOID Data(const U8* data);
		const U8* Data() const;

	private:
		BOOL mDirty;
		Rect mArea[2];
		I32 mX1, mY1, mX2, mY2;
		U32 mW1, mH1, mW2, mH2;
		U32 mFormat;
		U8* mData;

	private:friend class DC;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<class Bitmap>BitmapPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Bitmap__

//////////////////////////////////////////////////////////////////////////