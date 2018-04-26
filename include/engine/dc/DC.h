/*
 *  DC.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __DC__
#define __DC__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented DC interface.
	class DC : public GReference
	{
	public:
		// constructor
		DC();

		// destructor
		virtual ~DC();

		// set/get the width of the bitmap
		VOID Width(U32 w);
		U32 Width() const;

		// set/get the height of the bitmap
		VOID Height(U32 h);
		U32 Height() const;		

		// draw the bitmap to the dc
		VOID Draw(const Bitmap* bitmap);

		// flush the rect
		VOID Flush(const Rect& rc);

		// swap the back buffer of the dc
		VOID Swap(Graph* gc);

	private:
		// build the dc
		VOID Build();

		// update the ranges
		VOID Update();

		// paint the bitmap
		VOID Paint(const Bitmap*bitmap);

	private:
		BOOL mDirty;
		U32 mWidth, mHeight;
		ImagePtr mImagePtr;
		BaseTexturePtr mTexPtr;		
		PrimitivePtr mQuadPtr;
		std::list<BitmapPtr>mBitmaps;		
		std::list<Rect>mRanges;
		std::list<KeyPtr>mKeys;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<class DC>DCPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __DC__

//////////////////////////////////////////////////////////////////////////