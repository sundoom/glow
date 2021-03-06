/*
 *  Component.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Component__
#define __Component__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(UI)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented Component interface.
	class Component
	{
	public:		
		// constructor
		Component();
		
		// destructor
		virtual ~Component();
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Component__

//////////////////////////////////////////////////////////////////////////