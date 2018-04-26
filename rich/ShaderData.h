/*
 *  TextureRef.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __ShaderData__
#define __ShaderData__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented ShaderData interface.
	class ShaderData : public Resource
	{
  private:		
		// constructor
		ShaderData(const CHAR* name);
		
		// destructor
		virtual ~ShaderData();
  
    // load the data
    VOID Load(VOID* data, U32 size);    

  private:
    HANDLE mHandle;

  private:friend class Resource;
          friend class Primitive;
	};

  //////////////////////////////////////////////////////////////////////////

  typedef GPtr<ShaderData>ShaderDataPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __TextureData__

//////////////////////////////////////////////////////////////////////////