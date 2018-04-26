/*
 *  StaticMesh.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __StaticMesh__
#define __StaticMesh__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <GReference.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	typedef GPtr<class StaticMesh>StaticMeshPtr;

	//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented static mesh interface.
	class StaticMesh : public GReference
	{
	public:
		// constructor
		StaticMesh();

		// destructor
		virtual ~StaticMesh();

		// Load the data
		VOID Load(const GData* data);

		// draw the model
		VOID Draw(Graph *gc);

	protected:
		std::vector<PrimitivePtr>mPrimitives;
		std::list<KeyPtr>mKeys;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __StaticMesh__

//////////////////////////////////////////////////////////////////////////