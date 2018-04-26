//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	AnimBone::AnimBone()
	{
		mParent = NULL;
		mRatio = 0;
		mCookie = 0;
		mDirty = FALSE;
	}

	AnimBone::~AnimBone() 
	{

	}

	/*
	====================
	name
	====================
	*/
	VOID AnimBone::name(const CHAR* name)
	{
		GUARD(AnimBone::name);

		CHECK(name);
		mName = name;

		UNGUARD;
	}

	const CHAR* AnimBone::name() const
	{
		GUARD(AnimBone::name);

		return mName.c_str();

		UNGUARD;
	}

	/*
	====================
	parent
	====================
	*/
	VOID AnimBone::parent(const AnimBone* b)
	{
		GUARD(AnimBone::parent);

		if(mParent==b) return;

		// erase this from the old parent
		if(mParent)
		{
			std::vector<AnimBonePtr>::iterator it = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
			if(it!=mParent->mChildren.end())mParent->mChildren.erase(it);
		}

		// set the new parent
		mParent = (AnimBone*)b;

		// add this to the parent
		if(mParent) mParent->mChildren.push_back(this);

		UNGUARD;
	}

	const AnimBone* AnimBone::parent() const
	{
		GUARD(AnimBone::parent);

		return mParent;

		UNGUARD;
	}

	AnimBone* AnimBone::parent()
	{
		GUARD(AnimBone::parent);

		return mParent;

		UNGUARD;
	}

	/*
	====================
	child
	====================
	*/
	const AnimBone* AnimBone::child( U32 i ) const
	{
		GUARD(AnimBone::child);

		if(i<mChildren.size()) return mChildren[i].Ptr();
		return NULL;

		UNGUARD;
	}

	AnimBone* AnimBone::child( U32 i )
	{
		GUARD(AnimBone::child);

		if(i<mChildren.size()) return mChildren[i].Ptr();
		return NULL;

		UNGUARD;
	}
	
	/*
	====================
	find
	====================
	*/
	const AnimBone* AnimBone::find( const CHAR* name ) const
	{
		if(mName == name) return this;
		const AnimBone *ret = NULL;
		U32 size = mChildren.size();
		for( U32 i = 0; i < size && ret == NULL; i++ )
		{
			ret = mChildren[i]->find(name);
		}
		return ret;
	}

	AnimBone* AnimBone::find( const CHAR* name )
	{
		if(mName == name) return this;
		AnimBone *ret = NULL;
		U32 size = mChildren.size();
		for( U32 i = 0; i < size && ret == NULL; i++ )
		{
			ret = mChildren[i]->find(name);
		}
		return ret;
	}

	/*
	====================
	blend
	====================
	*/
	VOID AnimBone::blend( F32 frame, F32 ratio, U32 cookie )
	{
		GUARD(AnimBone::blend);

		CHECK(mFramePtr);
		std::vector<BoneFrame::KEY>&keys = mFramePtr->mKeys;
		U32 start = (U32)frame;
		U32 end = start+1;
		F32 fraction = (frame-start)/(end-start);
		start = start % keys.size();
		end = end % keys.size();

		// compute the frame
		if(ratio > 0.0f)
		{
			// blend it between two frame
			BoneFrame::KEY key;
			const BoneFrame::KEY&k1 = keys[start];
			const BoneFrame::KEY&k2 = keys[end];
			key.t = k1.t + (k2.t-k1.t)*fraction;
			key.r.slerp(fraction, k1.r, k2.r);
			key.s = k1.s + (k2.s-k1.s)*fraction;

			// blend it with the last
			F32 old_ratio = (cookie==mCookie ? mRatio : 0.0f);
			F32 sum_ratio = old_ratio + ratio;
			if(old_ratio == 0.0f)
			{
				mKey.t = key.t; mKey.r = key.r; mKey.s = key.s;
			}
			else
			{
				F32 f = ratio / sum_ratio;
				if(f > 0.0 && f < 1.0)
				{
					mKey.t = mKey.t + (key.t-mKey.t)*f;
					mKey.r.slerp(f, mKey.r, key.r);
					mKey.s = mKey.s + (key.s-mKey.s)*f;
				}
				else if ( f >= 1.0 )
				{
					mKey.t = key.t; mKey.r = key.r; mKey.s = key.s;
				}
			}
			mRatio = sum_ratio;
			mCookie = cookie;

			// set the state is dirty
			mDirty = TRUE;
		}

		// compute the children
		AnimBone*child = NULL;
		for(U32 i = 0; (child=dynamic_cast<AnimBone*>(this->child(i)))!=NULL; i++) 
		{ child->blend(frame, ratio, cookie); }

		UNGUARD;
	}

	/*
	====================
	local
	====================
	*/
	const Matrix& AnimBone::local() const
	{
		GUARD(AnimBone::local);

		if(mDirty)
		{
			mLocal.setRotate(mKey.r);
			mLocal.preMultScale(mKey.s);
			mLocal.postMultTranslate(mKey.t);
			if(mParent) mWorld = mLocal * mParent->world();
			else mWorld = mLocal;
			mDirty = FALSE;
		}

		return mLocal;

		UNGUARD;
	}	

	Matrix& AnimBone::local()
	{
		GUARD(AnimBone::local);

		if(mDirty)
		{
			mLocal.setRotate(mKey.r);
			mLocal.preMultScale(mKey.s);
			mLocal.postMultTranslate(mKey.t);
			if(mParent) mWorld = mLocal * mParent->world();
			else mWorld = mLocal;
			mDirty = FALSE;
		}

		return mLocal;

		UNGUARD;
	}

	/*
	====================
	world
	====================
	*/
	const Matrix& AnimBone::world() const
	{
		GUARD(AnimBone::world);

		if(mDirty)
		{
			mLocal.setRotate(mKey.r);
			mLocal.preMultScale(mKey.s);
			mLocal.postMultTranslate(mKey.t);
			if(mParent) mWorld = mLocal * mParent->world();
			else mWorld = mLocal;
			mDirty = FALSE;
		}

		return mWorld;

		UNGUARD;
	}	

	Matrix& AnimBone::world()
	{
		GUARD(AnimBone::world);

		if(mDirty)
		{
			mLocal.setRotate(mKey.r);
			mLocal.preMultScale(mKey.s);
			mLocal.postMultTranslate(mKey.t);
			if(mParent) mWorld = mLocal * mParent->world();
			else mWorld = mLocal;
			mDirty = FALSE;
		}

		return mWorld;

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
