/*
 *  AnimSequence.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __AnimSequence__
#define __AnimSequence__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <GReference.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	// this class provides an object-oriented AnimSequence interface.
	class AnimSequence : public GReference
	{
	public:
		#pragma pack(push,1)
		enum AS_FLAGS
		{
			ASF_LOOP	= 0x00000001l,
			ASF_MOVE	= 0x00000002l,
			ASF_IMPACT	= 0x00000004l,
		};
		#pragma pack(pop)
	public:
		// constructor
		AnimSequence();
		
		// destructor
		virtual ~AnimSequence();

		// set/get the action name
		VOID SetName(const CHAR* name);
		const CHAR* GetName() const;

		// set/get the action flags
		VOID SetFlags(U32 f);
		U32 GetFlags() const;

		// set/get the action first frame
		VOID SetFirstFrame(U32 f);
		U32 GetFirstFrame() const;

		// set/get the action last frame
		VOID SetLastFrame(U32 f);
		U32 GetLastFrame() const;

		// set/get the action frame rate
		VOID SetFrameRate(F32 r);
		F32 GetFrameRate() const;

		// set/get the action frame scale
		VOID SetFrameScale(F32 s);
		F32 GetFrameScale() const;

		// get the action duration
		F32 GetDuration() const;

		// set/get the action in time
		VOID SetInTime(F32 t);
		F32 GetInTime() const;

		// set/get the action out time
		VOID SetOutTime(F32 t);
		F32 GetOutTime() const;		

		// get the action ratio
		F32 GetRatio() const;

	private:
		Str mName;
		U32 mFlags;
		U32 mFirstFrame, mLastFrame;
		F32 mFrameRate, mFrameScale;
		F32 mInTime, mOutTime;
		U32 mEventFrame;
		F32 mPassedTime, mPlayedTime, mFinishTime;

	private: friend class AnimMixer;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<class AnimSequence>AnimSequencePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __AnimSequence__

//////////////////////////////////////////////////////////////////////////