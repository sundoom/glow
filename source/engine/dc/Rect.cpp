//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	#define VALIDATERECT(prc, retval)   if (prc == NULL) return retval
	
	//////////////////////////////////////////////////////////////////////////

	BOOL SetRect(Rect* prc, I32 left, I32 top, I32 right, I32 bottom)
	{
		VALIDATERECT(prc, FALSE);

		prc->left = left;
		prc->top = top;
		prc->right = right;
		prc->bottom = bottom;
		return TRUE;
	}

	BOOL SetRectEmpty(Rect* prc)
	{
		VALIDATERECT(prc, FALSE);
		
		::memset(prc,0,sizeof(Rect));
		return TRUE;
	}

	BOOL CopyRect(Rect* prcDst, const Rect *prcSrc)
	{
		VALIDATERECT(prcDst, FALSE);
		VALIDATERECT(prcSrc, FALSE);

		*prcDst = *prcSrc;
		return TRUE;
	}

	BOOL InflateRect(Rect* prc, I32 cx, I32 cy)
	{
		VALIDATERECT(prc, FALSE);

		prc->left   -= cx;
		prc->right  += cx;
		prc->top    -= cy;
		prc->bottom += cy;
		return TRUE;
	}

	BOOL IntersectRect(Rect* prcDst, const Rect* prcSrc1, const Rect* prcSrc2)
	{
		VALIDATERECT(prcDst, FALSE);
		VALIDATERECT(prcSrc1, FALSE);
		VALIDATERECT(prcSrc2, FALSE);

		prcDst->left  = GMath::max(prcSrc1->left, prcSrc2->left);
		prcDst->right = GMath::min(prcSrc1->right, prcSrc2->right);

		/*
		* check for empty rect
		*/
		if (prcDst->left < prcDst->right) {

			prcDst->top = GMath::max(prcSrc1->top, prcSrc2->top);
			prcDst->bottom = GMath::min(prcSrc1->bottom, prcSrc2->bottom);

			/*
			* check for empty rect
			*/
			if (prcDst->top < prcDst->bottom) {
				return TRUE;        // not empty
			}
		}

		/*
		* empty rect
		*/
		SetRectEmpty(prcDst);

		return FALSE;
	}

	BOOL UnionRect(Rect* prcDst, const Rect* prcSrc1, const Rect* prcSrc2)
	{
		BOOL frc1Empty, frc2Empty;

		VALIDATERECT(prcDst, FALSE);
		VALIDATERECT(prcSrc1, FALSE);
		VALIDATERECT(prcSrc2, FALSE);

    frc1Empty = ((prcSrc1->left >= prcSrc1->right) ||	(prcSrc1->top >= prcSrc1->bottom)) ? TRUE : FALSE;

    frc2Empty = ((prcSrc2->left >= prcSrc2->right) || (prcSrc2->top >= prcSrc2->bottom)) ? TRUE : FALSE;

		if (frc1Empty && frc2Empty) {
			SetRectEmpty(prcDst);
			return FALSE;
		}

		if (frc1Empty) {
			*prcDst = *prcSrc2;
			return TRUE;
		}

		if (frc2Empty) {
			*prcDst = *prcSrc1;
			return TRUE;
		}

		/*
		* form the union of the two non-empty rects
		*/
		prcDst->left   = GMath::min(prcSrc1->left,   prcSrc2->left);
		prcDst->top    = GMath::min(prcSrc1->top,    prcSrc2->top);
		prcDst->right  = GMath::max(prcSrc1->right,  prcSrc2->right);
		prcDst->bottom = GMath::max(prcSrc1->bottom, prcSrc2->bottom);

		return TRUE;
	}

	BOOL SubtractRect(Rect* prcDst, const Rect* prcSrc1, const Rect* prcSrc2)
	{
		I32 cSidesOut;
		BOOL fIntersect;
		Rect rcInt;

		VALIDATERECT(prcDst, FALSE);
		VALIDATERECT(prcSrc1, FALSE);
		VALIDATERECT(prcSrc2, FALSE);

		fIntersect = IntersectRect(&rcInt, prcSrc1, prcSrc2);

		/*
		* this is done after the intersection in case prcDst is the same
		* pointer as prcSrc2
		*/
		*prcDst = *prcSrc1;

		if (fIntersect) {
			/*
			* exactly any 3 sides of prc2 must be outside prc1 to subtract
			*/
			cSidesOut = 0;
			if (rcInt.left   <= prcSrc1->left)
				cSidesOut++;
			if (rcInt.top    <= prcSrc1->top)
				cSidesOut++;
			if (rcInt.right  >= prcSrc1->right)
				cSidesOut++;
			if (rcInt.bottom >= prcSrc1->bottom)
				cSidesOut++;

			if (cSidesOut == 4) {
				/*
				* result is the empty rect
				*/
				SetRectEmpty(prcDst);
				return FALSE;
			}

			if (cSidesOut == 3) {
				/*
				* subtract the intersecting rect
				*/
				if (rcInt.left > prcSrc1->left)
					prcDst->right = rcInt.left;

				else if (rcInt.right < prcSrc1->right)
					prcDst->left = rcInt.right;

				else if (rcInt.top > prcSrc1->top)
					prcDst->bottom = rcInt.top;

				else if (rcInt.bottom < prcSrc1->bottom)
					prcDst->top = rcInt.bottom;
			}
		}

		if ((prcDst->left >= prcDst->right) || (prcDst->top >= prcDst->bottom))
			return FALSE;

		return TRUE;
	}

	BOOL OffsetRect(Rect* prc, I32 cx, I32 cy)
	{
		VALIDATERECT(prc, FALSE);

		prc->left   += cx;
		prc->right  += cx;
		prc->bottom += cy;
		prc->top    += cy;
		return TRUE;
	}

	BOOL IsRectEmpty(const Rect* prc)
	{
		VALIDATERECT(prc, TRUE);

    return ((prc->left >= prc->right) || (prc->top >= prc->bottom)) ? TRUE : FALSE;
	}

	BOOL EqualRect(const Rect *prc1, const Rect *prc2)
	{
		VALIDATERECT(prc1, FALSE);
		VALIDATERECT(prc2, FALSE);

		/*
		* Test equality only. This is what win31 does. win31 does not check to
		* see if the rectangles are "empty" first.
		*/
		return (!memcmp(prc1, prc2, sizeof(Rect))) ? TRUE : FALSE;
	}

	BOOL PtInRect(const Rect *prc, I32 x, I32 y)
	{
		VALIDATERECT(prc, FALSE);

		return ((x >= prc->left) && (x < prc->right) && (y >= prc->top)  && (y < prc->bottom)) ? TRUE : FALSE;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
