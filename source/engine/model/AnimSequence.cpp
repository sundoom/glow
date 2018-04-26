//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	AnimSequence::AnimSequence()
	{
		mFlags = 0;
		mFirstFrame = 0;
		mLastFrame = 1;
		mFrameRate = 30;
		mFrameScale = 1.0f;
		mInTime = 0.3f;
		mOutTime = 0.3f;
		mEventFrame = 0;
		mPassedTime = 0;
		mPlayedTime = 0;
		mFinishTime = -1.0f;
	}

	AnimSequence::~AnimSequence() 
	{

	}

	//////////////////////////////////////////////////////////////////////////	

	/*
	====================
	name
	====================
	*/
	VOID AnimSequence::SetName(const CHAR* name)
	{
		GUARD(AnimSequence::SetName);

		mName = name;

		UNGUARD;
	}

	const CHAR* AnimSequence::GetName() const
	{
		GUARD(AnimSequence::GetName);

		return mName.c_str();

		UNGUARD;
	}

	/*
	====================
	flags
	====================
	*/
	VOID AnimSequence::SetFlags(U32 f)
	{
		GUARD(AnimSequence::SetFlags);

		mFlags = f;

		UNGUARD;
	}

	U32 AnimSequence::GetFlags() const
	{
		GUARD(AnimSequence::GetFlags);

		return mFlags;

		UNGUARD;
	}

	/*
	====================
	FirstFrame
	====================
	*/
	VOID AnimSequence::SetFirstFrame(U32 f)
	{
		GUARD(AnimSequence::SetFirstFrame);

		mFirstFrame = f;

		UNGUARD;
	}

	U32 AnimSequence::GetFirstFrame() const
	{
		GUARD(AnimSequence::GetFirstFrame);

		return mFirstFrame;

		UNGUARD;
	}

	/*
	====================
	LastFrame
	====================
	*/
	VOID AnimSequence::SetLastFrame(U32 f)
	{
		GUARD(AnimSequence::SetLastFrame);

		mLastFrame = f;

		UNGUARD;
	}

	U32 AnimSequence::GetLastFrame() const
	{
		GUARD(AnimSequence::GetLastFrame);

		return mLastFrame;

		UNGUARD;
	}

	/*
	====================
	FrameRate
	====================
	*/
	VOID AnimSequence::SetFrameRate(F32 r)
	{
		GUARD(AnimSequence::SetFrameRate);

		mFrameRate = r;

		UNGUARD;
	}

	F32 AnimSequence::GetFrameRate() const
	{
		GUARD(AnimSequence::GetFrameRate);

		return mFrameRate;

		UNGUARD;
	}

	/*
	====================
	FrameScale
	====================
	*/
	VOID AnimSequence::SetFrameScale(F32 s)
	{
		GUARD(AnimSequence::SetFrameScale);

		mFrameScale = s;

		UNGUARD;
	}

	F32 AnimSequence::GetFrameScale() const
	{
		GUARD(AnimSequence::GetFrameScale);

		return mFrameScale;

		UNGUARD;
	}

	/*
	====================
	duration
	====================
	*/
	F32 AnimSequence::GetDuration() const
	{
		GUARD(AnimSequence::GetDuration);

		return (F32)GMath::abs((I32)(mLastFrame - mFirstFrame)) / (mFrameRate * mFrameScale);

		UNGUARD;
	}

	/*
	====================
	InTime
	====================
	*/
	VOID AnimSequence::SetInTime(F32 t)
	{
		GUARD(AnimSequence::SetInTime);

		mInTime = t;

		UNGUARD;
	}

	F32 AnimSequence::GetInTime() const
	{
		GUARD(AnimSequence::GetInTime);

		return mInTime;

		UNGUARD;
	}

	/*
	====================
	OutTime
	====================
	*/
	VOID AnimSequence::SetOutTime(F32 t)
	{
		GUARD(AnimSequence::SetOutTime);

		mOutTime = t;

		UNGUARD;
	}

	F32 AnimSequence::GetOutTime() const
	{
		GUARD(AnimSequence::GetOutTime);

		return mOutTime;

		UNGUARD;
	}

	/*
	====================
	ratio
	====================
	*/
	F32 AnimSequence::GetRatio() const
	{
		GUARD(AnimSequence::GetRatio);

		F32 sum = GMath::min( (mPassedTime) / mInTime, 
			mFinishTime > 0.f ? (mFinishTime - mPassedTime) / mOutTime : 1.f );
		return sum >= 1.f ? 1.f : sum > 0.f ? sum : 0.f;

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
