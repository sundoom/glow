/*
 *  BoneFrame.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __BoneFrame__
#define __BoneFrame__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <GReference.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	// this class provides an object-oriented animation sequence interface.
	class BoneFrame: public GReference
	{
	public:
		#pragma pack(push,1)
		struct KEY 
		{ 
			Vector3 t;
			Quaternion r; 
			Vector3 s; 
		};		
		#pragma pack(pop)
	public:
		// constructor
		BoneFrame();

		// destructor
		virtual ~BoneFrame();

		// the sequence name
		VOID SetName(const CHAR* name);
		const CHAR* GetName() const;

		// the parent sequence
		VOID SetParent(const BoneFrame* s);
		const BoneFrame* GetParent() const;
		BoneFrame* GetParent();

		// the child sequence
		const BoneFrame* GetChild(U32 i) const;
		BoneFrame* GetChild(U32 i);

		// find a sequence
		const BoneFrame* Find(const CHAR* name) const;
		BoneFrame* Find(const CHAR* name);

	public:
		Str mName;
		BoneFrame*mParent;
		std::vector< GPtr<BoneFrame> >mChildren;
		std::vector<KEY>mKeys;

	private: friend class AnimSet;
	};	

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<class BoneFrame>BoneFramePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __BoneFrame__

//////////////////////////////////////////////////////////////////////////