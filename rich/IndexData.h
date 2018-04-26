/*
 *  TextureRef.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __IndexData__
#define __IndexData__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented IndexData interface.
	class IndexData : public GReference
	{
  public:
		// constructor
		IndexData();
		
		// destructor
		virtual ~IndexData();  

  private:
    HANDLE mHandle;
    U32	mCount;
    U32	mStride;

  private:friend class Primitive;
	};

  //////////////////////////////////////////////////////////////////////////

  typedef GPtr<IndexData>IndexDataPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __IndexData__

//////////////////////////////////////////////////////////////////////////