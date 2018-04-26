//////////////////////////////////////////////////////////////////////////

#include "CubeTexture.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(RHI)

//////////////////////////////////////////////////////////////////////////		

	CubeTexture::CubeTexture()
	{
		mTarget = GL_TEXTURE_CUBE_MAP;
	}

	CubeTexture::~CubeTexture() 
	{

	}

	//////////////////////////////////////////////////////////////////////////

  /*
	====================
	Load
	====================
	*/
	VOID CubeTexture::Load(U32 level, U32 width, U32 height, U32 format, U32 type, const U8* pixels)
	{
		
	}

	/*
	====================
	Update
	====================
	*/
	VOID CubeTexture::Update(U32 level, I32 x, I32 y, U32 width, U32 height, U32 format, U32 type, const U8* pixels)
	{
		
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
