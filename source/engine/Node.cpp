//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	Node::Node()
	{
		mFlags = NF_DRAW|NF_TRACE;
		mParent = NULL;
		mDirty = FALSE;
	}

	Node::~Node() 
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	name
	====================
	*/
	VOID Node::name(const CHAR* name)
	{
		GUARD(Node::name);

		CHECK(name);
		mName = name;

		UNGUARD;
	}

	const CHAR* Node::name() const
	{
		GUARD(Node::name);

		return mName.c_str();

		UNGUARD;
	}

	/*
	====================
	flags
	====================
	*/
	VOID Node::flags(U32 f)
	{
		GUARD(Node::flags);

		mFlags = f;

		UNGUARD;
	}

	U32 Node::flags() const
	{
		GUARD(Node::flags);

		return mFlags;

		UNGUARD;
	}

	/*
	====================
	parent
	====================
	*/
	VOID Node::parent(const Node* n)
	{
		GUARD(Node::parent);

		if(mParent==n) return;

		// erase this from the old parent
		if(mParent)
		{
			std::vector<NodePtr>::iterator it = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
			if(it!=mParent->mChildren.end())mParent->mChildren.erase(it);
		}

		// set the new parent
		mParent = (Node*)n;

		// add this to the parent
		if(mParent) mParent->mChildren.push_back(this);

		UNGUARD;
	}

	const Node* Node::parent() const
	{
		GUARD(Node::parent);

		return mParent;

		UNGUARD;
	}

	Node* Node::parent()
	{
		GUARD(Node::parent);

		return mParent;

		UNGUARD;
	}

	/*
	====================
	child
	====================
	*/
	const Node* Node::child(U32 i) const
	{
		GUARD(Node::child);

		if(i<mChildren.size()) return mChildren[i].Ptr();
		return NULL;

		UNGUARD;
	}

	Node* Node::child(U32 i)
	{
		GUARD(Node::child);

		if(i<mChildren.size()) return mChildren[i].Ptr();
		return NULL;

		UNGUARD;
	}
	
	/*
	====================
	find
	====================
	*/
	const Node* Node::find(const CHAR* name) const
	{
		if(mName == name) return this;
		const Node *ret = NULL;
		U32 size = mChildren.size();
		for( U32 i = 0; i < size && ret == NULL; i++ )
		{
			ret = mChildren[i]->find(name);
		}
		return ret;
	}

	Node* Node::find(const CHAR* name)
	{
		if(mName == name) return this;
		Node *ret = NULL;
		U32 size = mChildren.size();
		for( U32 i = 0; i < size && ret == NULL; i++ )
		{
			ret = mChildren[i]->find(name);
		}
		return ret;
	}

	/*
	====================
	local
	====================
	*/
	VOID Node::local(const Matrix& m)
	{
		GUARD(Node::local);

		mLocal = m;
		mDirty = TRUE;

		UNGUARD;
	}

	const Matrix& Node::local() const
	{
		GUARD(Node::local);

		return mLocal;

		UNGUARD;
	}	

	Matrix& Node::local()
	{
		GUARD(Node::local);

		return mLocal;

		UNGUARD;
	}

	/*
	====================
	world
	====================
	*/
	const Matrix& Node::world() const
	{
		GUARD(Node::world);

		if(mDirty)
		{
			if(mParent) mWorld = mLocal * mParent->world();
			else mWorld = mLocal;
			mDirty = FALSE;
		}
		
		return mWorld;

		UNGUARD;
	}	

	Matrix& Node::world()
	{
		GUARD(Node::world);

		if(mDirty)
		{
			if(mParent) mWorld = mLocal * mParent->world();
			else mWorld = mLocal;
			mDirty = FALSE;
		}

		return mWorld;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	box
	====================
	*/
	VOID Node::box(const BoundingBox& box)
	{
		GUARD(Node::box);

		mBox = box;

		UNGUARD;
	}

	const BoundingBox& Node::box() const
	{
		GUARD(Node::box);

		return mBox;

		UNGUARD;
	}	

	BoundingBox& Node::box()
	{
		GUARD(Node::box);

		return mBox;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	draw
	====================
	*/
	VOID Node::draw(Graph *gc)
	{
		GUARD(Node::draw);

		// if this is drawable?
		if((flags()&NF_DRAW)==0) return;

		// set the world space
		AUTO_LOCK_WORLD(gc);
		gc->world(local()*gc->world());

		// draw the children
		for(U32 i = 0; i < mChildren.size(); i++) mChildren[i]->draw(gc);

		UNGUARD;
	}

	/*
	====================
	trace
	====================
	*/
	F32 Node::trace(const Vector3& s, const Vector3& e) const
	{
		GUARD(Node::trace);

		// if this is traceable?
		if((flags()&NF_TRACE)==0) return 1.0f;

		// transform the line to world/object coordinate space.
		Matrix local_inv; local_inv.invert(mLocal);
		Vector3 ls = s * local_inv;
		Vector3 le = e * local_inv;

		F32 fraction = 1.0;

		// trace the children
		for(U32 i = 0; i < mChildren.size(); i++)
		{
			F32 f = mChildren[i]->trace(ls, le);
			fraction = (f < fraction)? f : fraction;
		}
		return fraction;

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
