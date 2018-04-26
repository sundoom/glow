/*
 *  AnimBone.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __AnimBone__
#define __AnimBone__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <GReference.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	typedef GPtr<class AnimBone>AnimBonePtr;

	//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented animation bone interface.
	class AnimBone: public GReference
	{
	public:
		// constructor
		AnimBone();

		// destructor
		virtual ~AnimBone();

		// the bone name
		VOID name(const CHAR* name);
		const CHAR* name() const;

		// the parent bone
		VOID parent(const AnimBone* n);
		const AnimBone* parent() const;
		AnimBone* parent();

		// the child bone
		const AnimBone* child( U32 i ) const;
		AnimBone* child( U32 i );

		// find a bone
		const AnimBone* find( const CHAR* name ) const;
		AnimBone* find( const CHAR* name );

		// blend the frame
		VOID blend(F32 frame, F32 ratio, U32 cookie);

		// the local space
		const Matrix& local() const;
		Matrix& local();

		// the world space
		const Matrix& world() const;
		Matrix& world();

	private:
		Str mName;
		AnimBone*mParent;
		std::vector<AnimBonePtr>mChildren;
		mutable Matrix mLocal, mWorld;
		mutable BOOL mDirty;
		BoneFramePtr mFramePtr;
		BoneFrame::KEY mKey;
		F32 mRatio;
		U32 mCookie;

	private: friend class AnimSet;
	};	

	//////////////////////////////////////////////////////////////////////////

	

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __AnimBone__

//////////////////////////////////////////////////////////////////////////