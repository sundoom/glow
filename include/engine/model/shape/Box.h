/*
 *  Box.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __Box__
#define __Box__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Node.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented box interface.
	class Box : public Shape
	{	
	public:
		// constructor
		Box();
		
		// destructor
		virtual ~Box();	

		// the box dimension
		VOID dimension(const Vector3& d);
		const Vector3& dimension() const;

		// trace the box
		F32 trace(const Vector3& s, const Vector3& e) const;

	private:
		// build the box
		VOID build();
		
	private:
		Vector3 mDimension;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Box>BoxPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Box__

//////////////////////////////////////////////////////////////////////////