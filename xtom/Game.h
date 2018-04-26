/*
 *  Game.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __Game__
#define __Game__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include <GMath.h>
NAMESPACE_USE(Math)
#include <Render.h>
NAMESPACE_USE(Render)
#include <Engine.h>
NAMESPACE_USE(Engine)

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(XTom)

//////////////////////////////////////////////////////////////////////////

	class Game
	{
	public:
		// query the instance of the application
		static Game& instance();

		// the application's run method.
		VOID init();
		VOID run(F32 delta_time);
		VOID exit();

		// bind the fbo
		VOID bind(U32 fbo);

		// set the window
		VOID window(U32 width, U32 height);

	private:
		// constructor
		Game();

		// destructor
		~Game();

		// build the resource
		VOID build();

	private:
		GraphPtr mGraphPtr;
		U32 mFBO;
		U32 mWndWidth, mWndHeight;
		F32 mFov, mAspect, mZNear, mZFar;
		Vector3 mEye, mCenter, mUp;
/*
		BoxPtr mBoxPtr;
		CapsulePtr mCapsulePtr;
		ConePtr mConePtr;
		CylinderPtr mCylinderPtr;
		SpherePtr mSpherePtr;
*/
		PawnPtr mPawnPtr;

		BaseTexturePtr mColorTexPtr;
		TargetPtr mColorRTPtr, mNullRTPtr;
		PrimitivePtr mQuadPtr;

		F32 mAngle;		

		std::list<KeyPtr>mKeys;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////