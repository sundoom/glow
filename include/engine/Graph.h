/*
 *  Graph.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Graph__
#define __Graph__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented graph interface.
	class Graph : public Context
	{
	public:
		// constructor
		Graph();

		// destructor
		virtual ~Graph();

	public:
		// cache a operation to the graph
		VOID draw(const Operation* op);

		// swap the back buffer of the graph
		VOID swap();
	
		// the viewport
		VOID viewport(const Vector4& v);
		const Vector4& viewport() const;
		Vector4& viewport();

		// set/get the projection
		VOID projection(const Matrix& m);
		const Matrix& projection() const;
		Matrix& projection();

		// set/get the view
		VOID view(const Matrix& m);
		const Matrix& view() const;
		Matrix& view();

		// set/get the world
		VOID world(const Matrix& m);
		const Matrix& world() const;
		Matrix& world();

		// set/get the vp
		const Matrix& vp() const;
		Matrix& vp();

		// set/get the wvp
		const Matrix& wvp() const;
		Matrix& wvp();

		// projection du point (objx,objy,obz) sur l'ecran (winx,winy,winz)
		VOID project(const Vector3& obj, Vector3& win);

		// transformation du point ecran (winx,winy,winz) en point objet
		VOID unproject(const Vector3& win, Vector3& obj);

	private:
		std::list<OperationPtr>mOperations;
		Vector4 mViewport;
		Matrix mProjection, mView, mWorld, mVP, mWVP;		
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<class Graph>GraphPtr;

	//////////////////////////////////////////////////////////////////////////

	#define AUTO_LOCK_WORLD(gc)	class LockWorld __lock_world_##__LINE__(gc);

	class LockWorld
	{
	public:
		LockWorld(const Graph*gc);
		~LockWorld();
	private:
		GraphPtr mGCPtr;
		Matrix mWorld;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Graph__

//////////////////////////////////////////////////////////////////////////