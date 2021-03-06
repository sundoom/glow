/*----------------------------------------------------------------------------*\
|
|						Public Interface to Muse Technology
|
|							     www.Muse.com
| Author : SunJun 
| Email : sundoom@msn.com
\*----------------------------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////////
// $Id: Capsule.h,v 1.2 2009/04/03 12:51:25 sunjun Exp $
//////////////////////////////////////////////////////////////////////////

#ifndef __Capsule__
#define __Capsule__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Node.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented capsule interface.
	class Capsule : public Shape
	{	
	public:
		// constructor
		Capsule();
		
		// destructor
		virtual ~Capsule();	

		// the capsule radius
		VOID radius( F32 r );
		const F32 radius() const;

		// the capsule height
		VOID height( F32 h );
		const F32 height() const;

		// trace the capsule
		F32 trace(const Vector3& s, const Vector3& e) const;

	private:
		// build the capsule
		VOID build();

	private:
		F32 mRadius, mHeight;
	};

	typedef GPtr<Capsule>CapsulePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Capsule__

//////////////////////////////////////////////////////////////////////////