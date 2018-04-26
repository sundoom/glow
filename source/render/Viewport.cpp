//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	Viewport::Viewport(I32 x, I32 y, I32 w, I32 h) 
	{
		GUARD(Viewport::Viewport);

		mRect[0] = x;
		mRect[1] = y;
		mRect[2] = w;
		mRect[3] = h;

		UNGUARD;
	}

	Viewport::~Viewport() 
	{
		GUARD(Viewport::~Viewport);

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	operator
	====================
	*/
	VOID Viewport::operator() (Context*rc)
	{
		GUARD(Viewport::operator());

		glViewport(mRect[0], mRect[1], mRect[2], mRect[3]);

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
