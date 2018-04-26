//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	BoneFrame::BoneFrame()
	{
		mParent = NULL;
	}

	BoneFrame::~BoneFrame() 
	{
		
	}

	/*
	====================
	name
	====================
	*/
	VOID BoneFrame::SetName(const CHAR* name)
	{
		GUARD(BoneFrame::SetName);

		CHECK(name);
		mName = name;

		UNGUARD;
	}

	const CHAR* BoneFrame::GetName() const
	{
		GUARD(BoneFrame::GetName);

		return mName.c_str();

		UNGUARD;
	}

	/*
	====================
	parent
	====================
	*/
	VOID BoneFrame::SetParent(const BoneFrame* s)
	{
		GUARD(BoneFrame::SetParent);

		if(mParent==s) return;

		// erase this from the old parent
		if(mParent)
		{
			std::vector<BoneFramePtr>::iterator it = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
			if(it!=mParent->mChildren.end())mParent->mChildren.erase(it);
		}

		// set the new parent
		mParent = (BoneFrame*)s;

		// add this to the parent
		if(mParent) (mParent)->mChildren.push_back(this);

		UNGUARD;
	}

	const BoneFrame* BoneFrame::GetParent() const
	{
		GUARD(BoneFrame::GetParent);

		return mParent;

		UNGUARD;
	}

	BoneFrame* BoneFrame::GetParent()
	{
		GUARD(BoneFrame::GetParent);

		return mParent;

		UNGUARD;
	}

	/*
	====================
	child
	====================
	*/
	const BoneFrame* BoneFrame::GetChild( U32 i ) const
	{
		GUARD(BoneFrame::GetChild);

		if(i<mChildren.size()) return mChildren[i].Ptr();
		return NULL;

		UNGUARD;
	}

	BoneFrame* BoneFrame::GetChild( U32 i )
	{
		GUARD(BoneFrame::GetChild);

		if(i<mChildren.size()) return mChildren[i].Ptr();
		return NULL;

		UNGUARD;
	}

	/*
	====================
	find
	====================
	*/
	const BoneFrame* BoneFrame::Find( const CHAR* name ) const
	{
		if(mName == name) return this;
		const BoneFrame *ret = NULL;
		U32 size = mChildren.size();
		for( U32 i = 0; i < size && ret == NULL; i++ )
		{
			ret = mChildren[i]->Find(name);
		}
		return ret;
	}

	BoneFrame* BoneFrame::Find( const CHAR* name )
	{
		if(mName == name) return this;
		BoneFrame *ret = NULL;
		U32 size = mChildren.size();
		for( U32 i = 0; i < size && ret == NULL; i++ )
		{
			ret = mChildren[i]->Find(name);
		}
		return ret;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
