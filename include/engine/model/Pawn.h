/*
 *  Pawn.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __Pawn__
#define __Pawn__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <GReference.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	typedef GPtr<class Pawn>PawnPtr;

	//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented pawn interface.
	class Pawn: public GReference
	{
	public:
		// constructor
		Pawn();

		// destructor
		virtual ~Pawn();

		// set/get the animation set
		VOID SetAnimSet(const AnimSet* as);
		const AnimSet* GetAnimSet() const;

		// add/remove the static mesh
		VOID AddStaticMesh(const CHAR* name, StaticMesh*mesh);
		VOID RemoveStaticMesh(const CHAR* name);

		// add/remove the skeletal mesh
		VOID AddSkeletalMesh(const CHAR* name, SkeletalMesh*mesh);
		VOID RemoveSkeletalMesh(const CHAR* name);

		// play the action
		VOID Play(const CHAR* name, BOOL mix = FALSE);

		// tick 
		VOID Tick(F32 delta_time);

		// draw the model
		VOID Draw(Graph *gc);	

	private:
		AnimMixer mAnimMixer;
		AnimSetPtr mAnimSetPtr;
		std::map<Str, StaticMeshPtr>mStaticMeshes;
		std::map<Str, SkeletalMeshPtr>mSkeletalMeshes;
		AnimBonePtr mRootPtr;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Pawn__

//////////////////////////////////////////////////////////////////////////