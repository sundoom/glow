/*
 *  Node.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Node__
#define __Node__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	#define NF_DRAW			0x00000001l
	#define NF_TRACE		0x00000002l
	#define NF_PICK			0x00000004l

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<class Node>NodePtr;

	//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented node interface.
	class Node: public virtual GReference
	{	
	public:
		// constructor
		Node();
		
		// destructor
		virtual ~Node();

		// the node name
		VOID name(const CHAR* name);
		const CHAR* name() const;

		// the node name
		VOID flags(U32 f);
		U32 flags() const;

		// the parent node
		VOID parent(const Node* n);
		const Node* parent() const;
		Node* parent();

		// the child node
		const Node* child(U32 i) const;
		Node* child(U32 i);

		// find a node
		const Node* find(const CHAR* name) const;
		Node* find(const CHAR* name);

		// the local space
		VOID local(const Matrix& m);
		virtual const Matrix& local() const;
		virtual Matrix& local();

		// the world space
		virtual const Matrix& world() const;
		virtual Matrix& world();

		// set/get the bounding box
		VOID box(const BoundingBox& box);
		const BoundingBox& box() const;
		BoundingBox& box();

		// draw the node
		virtual VOID draw(Graph *gc);

		// trace the node
		virtual F32 trace(const Vector3& s, const Vector3& e) const;

	protected:
		Str mName;
		U32 mFlags;
		Node*mParent;
		std::vector<NodePtr>mChildren;
		mutable BOOL mDirty;
		mutable Matrix mLocal, mWorld;
		BoundingBox mBox;
	};	

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Node__

//////////////////////////////////////////////////////////////////////////