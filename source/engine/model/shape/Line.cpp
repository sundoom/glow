//////////////////////////////////////////////////////////////////////////
// $Id: Line.cpp,v 1.1 2009/04/03 12:46:11 sunjun Exp $
//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	Line::Line()
	{
		mType = Primitive::PT_LINES;
	}

	Line::~Line() 
	{
		
	}

	/*
	====================
	add a point
	====================
	*/ 
	VOID Line::add(const Vector3& p)
	{
		if(mVB.size()==0)mBox.set(MAX_F32,MAX_F32,MAX_F32,MIN_F32,MIN_F32,MIN_F32);
		mVB.push_back(p);
		mBox.expand(p);
	}

	/*
	====================
	add a index
	====================
	*/ 
	VOID Line::add(U32 i)
	{
		mIB.push_back(i);
	}

	/*
	====================
	build
	====================
	*/
	VOID Line::build()
	{
		GUARD(Line::build);	

		// build
		if(mVB.size() || mIB.size())
		{
			// build the primitive
			mPrimitivePtr = GNEW(Primitive); CHECK(mPrimitivePtr);
			mPrimitivePtr->SetType(mType);
			
			// set the wvp
			Constant* wvp_constant_ptr = GNEW(Constant); CHECK(wvp_constant_ptr);
			wvp_constant_ptr->SetMatrix(Matrix());
			mPrimitivePtr->SetConstant("gWVP",wvp_constant_ptr);

			// set the color
			Constant* color_constant_ptr = GNEW(Constant); CHECK(color_constant_ptr);
			color_constant_ptr->SetVector(Vector4(0,0,0,0));
			mPrimitivePtr->SetConstant("gColor",color_constant_ptr);

			// set the shader
			Str key_name = "shader/color.xml";
			KeyPtr key_ptr = Key::Find(key_name.c_str());
			if(key_ptr == NULL)
			{
				Shader*shader = GNEW(Shader); CHECK(shader);
				shader->Load(GLoad(key_name.c_str()));
				key_ptr = GNEW(Key(key_name.c_str(), shader)); CHECK(key_ptr);
			}
			mKeys.push_back(key_ptr);
			mPrimitivePtr->SetShader(dynamic_cast<Shader*>(key_ptr->Ptr()), "p0");

			// build the vertex buffer
			VertexBufferPtr vb_ptr = GNEW(VertexBuffer); CHECK(vb_ptr);
			{
				GDataPtr vd_ptr = GNEW(GData); CHECK(vd_ptr);
				vd_ptr->Size(3*sizeof(U32) + 2*sizeof(U8) + mVB.size()*sizeof(Vector3));
				U8*data_ptr = (U8*)vd_ptr->Ptr();
				*(U32*)data_ptr = MAKEFOURCC('G','V','B','O');
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = mVB.size();
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = sizeof(Vector3); 
				data_ptr += sizeof(U32);
				*(U8*)data_ptr = 1;
				data_ptr += sizeof(U8);
				*(U8*)data_ptr = VertexBuffer::VT_3F;
				data_ptr += sizeof(U8);
				::memcpy(data_ptr, &mVB[0], mVB.size()*sizeof(Vector3));
				data_ptr += mVB.size()*sizeof(Vector3);
				vb_ptr->Load(vd_ptr.Ptr());
			}
			mPrimitivePtr->SetVertexBuffer(vb_ptr.Ptr());

			// build the index
			IndexBufferPtr ib_ptr = GNEW(IndexBuffer); CHECK(ib_ptr);
			{
				GDataPtr id_ptr = GNEW(GData); CHECK(id_ptr);
				id_ptr->Size(3*sizeof(U32) + mIB.size()*sizeof(U32));
				U8*data_ptr = (U8*)id_ptr->Ptr();
				*(U32*)data_ptr = MAKEFOURCC('G','I','B','O');
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = mIB.size(); 
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = sizeof(U32); 
				data_ptr += sizeof(U32);
				::memcpy(data_ptr, &mIB[0], mIB.size()*sizeof(U32));
				data_ptr += mIB.size()*sizeof(U32);
				ib_ptr->Load(id_ptr.Ptr());
			}
			mPrimitivePtr->SetIndexBuffer(ib_ptr.Ptr());	

			// build the bounding box
			mBox.set(MAX_F32,MAX_F32,MAX_F32,MIN_F32,MIN_F32,MIN_F32);
			for(U32 i = 0; i < mVB.size(); i++)mBox.expand(mVB[i]);
			mPrimitivePtr->SetBox(mBox);

			// clean the original data
			mVB.clear(); mIB.clear();
		}

		UNGUARD;
	}	

	/*
	====================
	trace
	====================
	*/
	F32 Line::trace(const Vector3& s, const Vector3& e) const
	{
		GUARD(Line::trace);	

		// if this is traceable?
		if((flags()&NF_TRACE)==0) return 1.0f;

		// transform the line to world/object coordinate space.
		Matrix local_inv; local_inv.invert(mLocal);
		Vector3 ls = s * local_inv;
		Vector3 le = e * local_inv;	

		// recalculate the box of the line
		BoundingBox box(mBox);
		F32 epsilon = 0.01f;
		box.min().x() -= epsilon;
		box.min().y() -= epsilon;
		box.min().z() -= epsilon;
		box.max().x() += epsilon;
		box.max().y() += epsilon;
		box.max().z() += epsilon;

		// the fraction
		F32 fraction = 1.0;

		// clip the box			
		Vector3 start = ls, end = le;
		if(mBox.clip(start,end))
		{
			F32 f = (start - ls).length() / (le - ls).length();
			fraction = f < fraction ? f : fraction;
		}

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
