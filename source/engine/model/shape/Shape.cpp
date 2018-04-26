//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	Shape::Shape()
	{
		mType = Primitive::PT_TRIANGLES;
		mColor.set(1,1,1,1);
	}

	Shape::~Shape() 
	{
		
	}

	/*
	====================
	type
	====================
	*/
	VOID Shape::type(U32 t)
	{
		GUARD(Shape::type);

		mType = t;

		UNGUARD;
	}

	U32 Shape::type() const
	{
		GUARD(Shape::type);

		return mType;

		UNGUARD;
	}

	/*
	====================
	color
	====================
	*/
	VOID Shape::color(const Vector4& c)
	{
		GUARD(Shape::color);

		mColor = c;

		UNGUARD;
	}

	const Vector4& Shape::color() const
	{
		GUARD(Shape::color);

		return mColor;

		UNGUARD;
	}

	/*
	====================
	draw
	====================
	*/
	VOID Shape::draw(Graph *gc)
	{
		GUARD(Shape::draw);

		// if this is drawable?
		if((flags()&NF_DRAW)==0) return;

		// set the world space
		AUTO_LOCK_WORLD(gc);
		gc->world(local()*gc->world());
		const Matrix& world = gc->world();
		Matrix world_inv; world_inv.invert(world);

		// cull the bounding box
		Frustum frustum(gc->wvp());
		if(frustum.boxInFrustum(mBox.min(),mBox.max()))
		{
			// draw the primitive
			if(mPrimitivePtr) 
			{
				mPrimitivePtr->SetType(mType);
				mPrimitivePtr->GetConstant("gWVP")->SetMatrix(gc->wvp());
				mPrimitivePtr->GetConstant("gColor")->SetVector(mColor);
				gc->draw(static_cast<Operation*>(mPrimitivePtr.Ptr()));
			}
		}

		// draw the children
		for(U32 i = 0; i < mChildren.size(); i++) mChildren[i]->draw(gc);

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
