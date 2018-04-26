//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	Graph::Graph()
	{
		
	}
	
	Graph::~Graph()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////	

	/*
	====================
	draw
	====================
	*/
	VOID Graph::draw(const Operation* op)
	{
		GUARD(Graph::draw);

		mOperations.push_back((Operation*)op);

		UNGUARD;
	}

	/*
	====================
	swap
	====================
	*/
	VOID Graph::swap()
	{
		GUARD(Graph::swap);
		
		for(std::list<OperationPtr>::iterator it=mOperations.begin(); it!=mOperations.end(); ++it)
		{
			Context::Exec((*it).Ptr());
		}
		mOperations.clear();
		Context::Free();

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	viewport
	====================
	*/
	VOID Graph::viewport(const Vector4& v)
	{
		GUARD(Graph::viewport);

		mViewport = v;

		UNGUARD;
	}

	const Vector4& Graph::viewport() const
	{
		GUARD(Graph::viewport);

		return mViewport;

		UNGUARD;
	}

	Vector4& Graph::viewport()
	{
		GUARD(Graph::viewport);

		return mViewport;

		UNGUARD;
	}

	/*
	====================
	projection
	====================
	*/
	VOID Graph::projection(const Matrix& m)
	{
		GUARD(Graph::projection);

		mProjection = m;
		mVP = mView*mProjection;
		mWVP = mWorld*mView*mProjection;

		UNGUARD;
	}

	const Matrix& Graph::projection() const
	{
		GUARD(Graph::projection);

		return mProjection;

		UNGUARD;
	}

	Matrix& Graph::projection()
	{
		GUARD(Graph::projection);

		return mProjection;

		UNGUARD;
	}

	/*
	====================
	view
	====================
	*/
	VOID Graph::view(const Matrix& m)
	{
		GUARD(Graph::view);

		mView = m;
		mVP = mView*mProjection;
		mWVP = mWorld*mView*mProjection;

		UNGUARD;
	}

	const Matrix& Graph::view() const
	{
		GUARD(Graph::view);

		return mView;

		UNGUARD;
	}	

	Matrix& Graph::view()
	{
		GUARD(Graph::view);

		return mView;

		UNGUARD;
	}	

	/*
	====================
	world
	====================
	*/
	VOID Graph::world(const Matrix& m)
	{
		GUARD(Graph::world);

		mWorld = m;
		mWVP = mWorld*mView*mProjection;

		UNGUARD;
	}

	const Matrix& Graph::world() const
	{
		GUARD(Graph::world);

		return mWorld;

		UNGUARD;
	}	

	Matrix& Graph::world()
	{
		GUARD(Graph::world);

		return mWorld;

		UNGUARD;
	}

	/*
	====================
	vp
	====================
	*/
	const Matrix& Graph::vp() const
	{
		GUARD(Graph::vp);

		return mVP;

		UNGUARD;
	}	

	Matrix& Graph::vp()
	{
		GUARD(Graph::vp);

		return mVP;

		UNGUARD;
	}

	/*
	====================
	wvp
	====================
	*/
	const Matrix& Graph::wvp() const
	{
		GUARD(Graph::wvp);

		return mWVP;

		UNGUARD;
	}	

	Matrix& Graph::wvp()
	{
		GUARD(Graph::wvp);

		return mWVP;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	project
	====================
	*/
	VOID Graph::project(const Vector3& obj, Vector3& win)
	{
		GUARD(Graph::project);

		Matrix wm = Matrix::makeTranslate(1.0f,1.0f,1.0f)*Matrix::makeScale(0.5f*mViewport[2],0.5f*mViewport[3],0.5f)*Matrix::makeTranslate(mViewport[0],mViewport[1],0.0f);
		Matrix vpwm = (mView * mProjection) * wm;
		win = obj * vpwm;

		UNGUARD;
	}

	/*
	====================
	unproject
	====================
	*/
	VOID Graph::unproject(const Vector3& win, Vector3& obj)
	{
		GUARD(Graph::unproject);

		Matrix wm = Matrix::makeTranslate(1.0f,1.0f,1.0f)*Matrix::makeScale(0.5f*mViewport[2],0.5f*mViewport[3],0.5f)*Matrix::makeTranslate(mViewport[0],mViewport[1],0.0f);
		Matrix vpwm = (mView * mProjection) * wm;
		Matrix vpwm_inv; vpwm_inv.invert(vpwm);
		obj = win * vpwm_inv;

		UNGUARD;
	}
	
	//////////////////////////////////////////////////////////////////////////

	LockWorld::LockWorld(const Graph*gc)
	{
		CHECK(gc);
		mWorld = gc->world();
		mGCPtr = (Graph*)gc;
	}

	LockWorld::~LockWorld()
	{
		mGCPtr->world(mWorld);
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
