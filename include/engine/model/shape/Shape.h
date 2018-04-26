/*
 *  Shape.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __Shape__
#define __Shape__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Node.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented shape interface.
	class Shape : public Node
	{	
	public:
		// constructor
		Shape();
		
		// destructor
		virtual ~Shape();

		// set the type
		VOID type(U32 t);
		U32 type() const;

		// set the color
		VOID color(const Vector4& c);
		const Vector4& color() const;

		// draw the box
		VOID draw(Graph *gc);

		// trace the box
		virtual F32 trace(const Vector3& s, const Vector3& e) const = 0;
		
	protected:
		U32 mType;
		Vector4 mColor;
		PrimitivePtr mPrimitivePtr;
		std::list<KeyPtr>mKeys;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Shape>ShapePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Shape__

//////////////////////////////////////////////////////////////////////////