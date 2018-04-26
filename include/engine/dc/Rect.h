/*
 *  Rect.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Rect__
#define __Rect__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	struct Rect
	{
		I32 left;
		I32 top;
		I32 right;
		I32	bottom;
	};

	//////////////////////////////////////////////////////////////////////////
	
	BOOL SetRect(Rect* prc, I32 left, I32 top, I32 right, I32 bottom);

	BOOL SetRectEmpty(Rect* lprc);

	BOOL CopyRect(Rect* lprcDst, const Rect *lprcSrc);

	BOOL InflateRect(Rect* lprc, I32 cx, I32 cy);

	BOOL IntersectRect(Rect* lprcDst, const Rect* lprcSrc1, const Rect* lprcSrc2);

	BOOL UnionRect(Rect* lprcDst, const Rect* lprcSrc1, const Rect* lprcSrc2);

	BOOL SubtractRect(Rect* lprcDst, const Rect* lprcSrc1, const Rect* lprcSrc2);

	BOOL OffsetRect(Rect* lprc, I32 cx, I32 cy);

	BOOL IsRectEmpty(const Rect* lprc);

	BOOL EqualRect(const Rect *lprc1, const Rect *lprc2);

	BOOL PtInRect(const Rect *prc, I32 x, I32 y);

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Rect__

//////////////////////////////////////////////////////////////////////////