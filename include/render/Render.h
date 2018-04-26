/*
 *  Render.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Render__
#define __Render__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include <GMath.h>
NAMESPACE_USE(Math)

//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#include <GLES2/gl2.h>
	#include <EGL/egl.h>
	#pragma comment(lib,"libEGL.lib")
	#pragma comment(lib,"libGLESv2.lib")
#elif LINUX
#elif __APPLE__
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#elif _XBOX
#else
#error Unknown platform!
#endif

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented operation interface.
	class Operation : public GReference
	{		
	protected:
		// do the actual task of this operation.
		virtual VOID operator() (class Context*rc) = 0;

	private: friend class Context;
	};	

	typedef GPtr<Operation>OperationPtr;

	//////////////////////////////////////////////////////////////////////////

	enum RenderObjectType
	{
		ROT_TEXTURE,
		ROT_PROGRAM,
		ROT_RENDER_BUFFER,
		ROT_FRAME_BUFFER,
		ROT_INDEX_BUFFER,
		ROT_VERTEX_BUFFER
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Image.h>
#include <BaseTexture.h>
#include <Texture.h>
// #include <VolumeTexture.h>
#include <CubeTexture.h>
#include <Constant.h>
#include <Shader.h>

#include <Clear.h>
#include <Swap.h>
#include <Scissor.h>
#include <Viewport.h>
#include <Primitive.h>
#include <Target.h>

#include <Context.h>

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////