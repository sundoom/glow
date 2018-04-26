//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	Bitmap::Bitmap()
	{
		mDirty = FALSE;
		mX1 = mY1 = 0;
		mW1 = mH1 = 0;
		mX2 = mY2 = 0;
		mW2 = mH2 = 0;
		mFormat = 0xFF;
		mData = NULL;
	}
	
	Bitmap::~Bitmap()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	X
	====================
	*/
	VOID Bitmap::X(I32 x)
	{
		if(mX1 != x)
		{
			mX2 = mX1;
			mX1 = x;
			mDirty = TRUE;
		}
	}

	I32 Bitmap::X() const
	{
		return mX1;
	}

	/*
	====================
	Y
	====================
	*/
	VOID Bitmap::Y(I32 y)
	{
		if(mY1 != y)
		{
			mY2 = mY1;
			mY1 = y;
			mDirty = TRUE;
		}
	}

	I32 Bitmap::Y() const
	{
		return mY1;
	}

	/*
	====================
	Width
	====================
	*/
	VOID Bitmap::Width(U32 w)
	{
		if(mW1 != w)
		{
			mW2 = mW1;
			mW1 = w;
			mDirty = TRUE;
		}
	}

	U32 Bitmap::Width() const
	{
		return mW1;
	}

	/*
	====================
	Height
	====================
	*/
	VOID Bitmap::Height(U32 h)
	{
		if(mH1 != h)
		{
			mH2 = mH1;
			mH1 = h;
			mDirty = TRUE;
		}
	}

	U32 Bitmap::Height() const
	{
		return mH1;
	}

	/*
	====================
	Format
	====================
	*/
	VOID Bitmap::Format(U32 f)
	{
		if(mFormat != f)
		{
			mFormat = f;
			mDirty = TRUE;
		}
	}

	U32 Bitmap::Format() const
	{
		return mFormat;
	}

	/*
	====================
	Data
	====================
	*/
	VOID Bitmap::Data(const U8* data)
	{
		if(mData != data)
		{
			mData = (U8*)data;
			mDirty = TRUE;
		}
	}

	const U8* Bitmap::Data() const
	{
		return mData;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
