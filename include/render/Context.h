/*
 *  Context.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Context__
#define __Context__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented context interface.
	class Context : public GReference
	{		
	public:
		// constructor
		Context();
		
		// destructor
		virtual ~Context();		

		// reset the context 
		VOID Reset();

		// exec a operation
		VOID Exec(const Operation* op);

		// free all of the resource
		VOID Free();

	private:
		// release the render object of the context
		VOID Release(U32 type, U32 id);

		// bind the frame buffer
		VOID BindFramebuffer(U32 fbo);

		// bind the render buffer
		VOID BindRenderbuffer(U32 rbo);

		// bind the shader program
		VOID BindProgram(U32 program);

		// bind the texture object
		VOID BindTexture(U32 target, U32 object, U32 unit = 0);

		// bind the vertex buffer
		VOID BindVertexBuffer(U32 object, U32 stride = 0, const U8* attribs = NULL);

		// bind the index buffer
		VOID BindIndexBuffer(U32 object);

		// bind the blend func
		VOID BindBlendFunc(U32 src, U32 dst);

		// bind the depth func
		VOID BindDepthFunc(U32 func);

		// bind the depth mask
		VOID BindDepthMask(BOOL mask);

		// bind the cull func
		VOID BindCullFunc(U32 mode);

	private:
		// the render state
		U32 mFBO;
		U32 mRBO;
		U32 mProgram;
		#define MAX_TEXTURE_UNITS 4
		U32 mTextureUnit;
		U32 mTextureObject[MAX_TEXTURE_UNITS];
		U32 mVertexObject;		
		U32 mVertexAttribCount;
		U32 mIndexObject;
		U32 mBlendSrc;
		U32 mBlendDst;
		U32 mDepthFunc;	
		BOOL mDepthMask;
		U32 mCullMode;

		// the unused resource
		// GMutex mUnusedMutex;
		std::vector< std::pair<U32,U32> >mUnuseds;

	private: 
		friend class Target;
		friend class Shader;
		friend class VertexBuffer;
		friend class IndexBuffer;
		friend class BaseTexture;
		friend class VolumeTexture;
		friend class CubeTexture;
		friend class Swap;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Context>ContextPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Context__

//////////////////////////////////////////////////////////////////////////