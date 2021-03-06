/*
 *  TextureRef.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __VertexData__
#define __VertexData__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented VertexData interface.
	class VertexData : public GReference
	{
  public:
		// constructor
		VertexData();
		
		// destructor
		virtual ~VertexData();

  private:
    HANDLE mHandle;

  private:friend class Primitive;
	};

  //////////////////////////////////////////////////////////////////////////

  typedef GPtr<VertexData>VertexDataPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __VertexData__

//////////////////////////////////////////////////////////////////////////