//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	AnimMixer::AnimMixer()
	{
		mRoot = NULL;
		mPresent = NULL;
	}

	AnimMixer::~AnimMixer() 
	{

	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Bind
	====================
	*/
	VOID AnimMixer::Bind(const AnimBone* root)
	{
		CHECK(root);
		mRoot = (AnimBone*)root;
	}

	/*
	====================
	Play
	====================
	*/
	VOID AnimMixer::Play(const CHAR* name)
	{
		GUARD(AnimMixer::Play);

		// get the sequence
		std::map<Str,AnimSequencePtr>::iterator it0 = mSequences.find(name);
		CHECK(it0!=mSequences.end());
		AnimSequence*sequence = it0->second.Ptr();

		// if there are primary actions or
		// if there is already an sequence on same track and
		// they are not finished playing
		// then matched sequence must be NULL
		for(std::list<AnimSequence*>::iterator it = mQueue.begin(); it != mQueue.end(); ++it )
		{
			AnimSequence* e = (*it);
			// if(e->mFinishTime < 0.f || (e->mPassedTime + Timer::instance().frame()) < (e->mFinishTime - e->mOutTime)) 
			if(e->mFinishTime < 0.f || e->mPassedTime < (e->mFinishTime - e->mOutTime)) 
			{ sequence = NULL; break; }
		}

		// is this the same sequence as last time?
		if(sequence == mPresent) return;

		F32	cycle = 0.0f;

		// they're different, so start blending out the old one
		if(mPresent)
		{
			// copy the cycle position of the old animation if it was movement
			if( (mPresent->mFlags&AnimSequence::ASF_MOVE) && (mPresent->mFlags&AnimSequence::ASF_LOOP) )
			{
				cycle = GMath::mod( (F32)(mPresent->mPlayedTime/mPresent->GetDuration()), 1.0f );
			}

			// calculate the finish time
			mPresent->mFinishTime = mPresent->mPassedTime + mPresent->mOutTime * mPresent->GetRatio();

			// now whack it on the back
			mQueue.push_back(mPresent);
		}

		// set up the new sequence
		mPresent = sequence;
		if(mPresent)
		{
			// calculate what point in the animation it should start at
			F32 played_time = (mPresent->mFlags&AnimSequence::ASF_MOVE) ? (cycle*mPresent->GetDuration()) : 0.0f;
			F32 passed_time = 0.0f;

			// see if this matched sequence is still playing somewhere else.
			// this will be the case if it's 'finish' time isn't -1, since
			// we set an sequence's finish time back to -1 when we erase it
			if(mPresent->mFinishTime != -1.0f)
			{
				// it's played for the same amount of time
				played_time = mPresent->mPlayedTime;
				
				// whatever blend ratio it has now, that's how far we put it
				// from the beginning (to keep the blend ratio the same)
				passed_time = mPresent->GetRatio() * mPresent->mInTime;

				// figure out which entry to erase from the queue
				for(std::list<AnimSequence*>::iterator it = mQueue.begin(); it != mQueue.end(); ++it )
				{ if((*it) == mPresent) { mQueue.erase(it); break; } }
			}

			// set the present
			mPresent->mPlayedTime = played_time;
			mPresent->mPassedTime = passed_time;
			mPresent->mFinishTime = -1.0f;
		}

		UNGUARD;
	}

	/*
	====================
	Mix
	====================
	*/
	VOID AnimMixer::Mix(const CHAR* name)
	{
		GUARD(AnimMixer::Mix);

		// get the sequence
		std::map<Str,AnimSequencePtr>::iterator it0 = mSequences.find(name);
		CHECK(it0!=mSequences.end());
		AnimSequence*sequence = it0->second.Ptr();

		// see if this matched sequence is still playing somewhere else.
		// this will be the case if it's 'finish' time isn't -1, since
		// we set an sequence's finish time back to -1 when we erase it
		if(sequence->mFinishTime == -1.0f)
		{
			// set the present
			sequence->mPlayedTime = 0.0;
			sequence->mPassedTime = 0.0;
			sequence->mFinishTime = sequence->GetDuration();
			mQueue.push_back(sequence);
		}

		UNGUARD;
	}

	/*
	====================
	Tick

		updates the state all of the actions
	====================
	*/
	VOID AnimMixer::Tick(F32 delta_time)
	{
		GUARD(AnimMixer::Tick);

		// see if there's anyone due to stop
		std::list<AnimSequence*>::iterator it = mQueue.begin();
		while(it!=mQueue.end())
		{
			AnimSequence* sequence = *it;

			// move on the time passed and played
			sequence->mPassedTime += delta_time;
			sequence->mPlayedTime += delta_time;

			// see if we're getting rid of it now
			if(sequence->mFinishTime >= 0.f && sequence->mFinishTime < sequence->mPassedTime)
			{
				// end the sequence
				sequence->mFinishTime = -1.0f;
				
				// erase the whole sequence
				it = mQueue.erase(it);
			}
			else
			{
				++it;
			}
		}

		// see what's up with the sequence
		if(mPresent)
		{
			mPresent->mPassedTime += delta_time;
			mPresent->mPlayedTime += delta_time;
		}

		// set the cookie
		static U32 s_cookie = 0; s_cookie++;

		// play the present 
		if(mPresent)
		{
			// cache blend ratio
			F32 blend_ratio = mPresent->GetRatio();

			// play corresponding animations
			F32 time = mPresent->mPlayedTime;
			if(time > mPresent->GetDuration()) time = (mPresent->mFlags&AnimSequence::ASF_LOOP) ? 
				GMath::mod(time, mPresent->GetDuration()) : mPresent->GetDuration();

			F32 frame = time * (mPresent->mFrameRate * mPresent->mFrameScale);
			frame = (mPresent->mFirstFrame <= mPresent->mLastFrame) ?
				(mPresent->mFirstFrame + frame) : (mPresent->mFirstFrame - frame);

			if(blend_ratio>0.0f && mRoot)mRoot->blend(frame, blend_ratio, s_cookie);
			
		}

		// play the queue
		for(it = mQueue.begin(); it != mQueue.end(); ++it)
		{
			AnimSequence* sequence = *it;

			// cache blend ratio
			F32 blend_ratio = sequence->GetRatio();

			// play corresponding animations
			F32 time = sequence->mPlayedTime;
			if(time > sequence->GetDuration()) time = (sequence->mFlags&AnimSequence::ASF_LOOP) ? 
				GMath::mod(time, sequence->GetDuration()) : sequence->GetDuration();

			F32 frame = time * (sequence->mFrameRate * sequence->mFrameScale);
			frame = (sequence->mFirstFrame <= sequence->mLastFrame) ?
				(sequence->mFirstFrame + frame) : (sequence->mFirstFrame - frame);

			if(blend_ratio>0.0f && mRoot)mRoot->blend(frame, blend_ratio, s_cookie);
		}

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
