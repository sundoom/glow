//////////////////////////////////////////////////////////////////////////
// $Id: Cylinder.cpp,v 1.1 2009/04/03 12:46:11 sunjun Exp $
//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	Cylinder::Cylinder()
	{
		// build the default cylinder
		mRadius = 1.0f;
		mHeight = 1.0f;
		build();
	}

	Cylinder::~Cylinder() 
	{
		
	}

	/*
	====================
	radius
	====================
	*/
	VOID Cylinder::radius( F32 r )
	{
		mRadius = r;
		build();
	}

	const F32 Cylinder::radius() const
	{
		return mRadius;
	}

	/*
	====================
	height
	====================
	*/
	VOID Cylinder::height( F32 h )
	{
		mHeight = h;
		build();
	}

	const F32 Cylinder::height() const
	{
		return mHeight;
	}

	/*
	====================
	trace
	====================
	*/
	F32 Cylinder::trace( const Vector3& s, const Vector3& e ) const
	{
		GUARD(Cylinder::trace);

		// if this is traceable?
		if((flags()&NF_TRACE)==0) return 1.0f;

		// transform the line to world/object coordinate space.
		Matrix local_inv; local_inv.invert(mLocal);
		Vector3 ls = s * local_inv;
		Vector3 le = e * local_inv;		

		// the fraction
		F32 fraction = 1.0;

		// clip the box			
		Vector3 start = ls, end = le;
		if(mBox.clip(start,end))
		{
			F32 f = (start - ls).length() / (le - ls).length();
			fraction = f < fraction ? f : fraction;
		}

		// trace the cylinder
		if(fraction<1.0f)
		{

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

	/*
	====================
	build
	====================
	*/
	VOID Cylinder::build()
	{
		GUARD(Cylinder::build);

		// clear the old keys
		mKeys.clear();

		#define CYLINDER_SEGMENTS	20

		std::vector<DVTN>vertexes;
		std::vector<U32>indexes;

		// create the cylinder body
		{
			const F32 angle_delta = 2.0f*PI/(F32)CYLINDER_SEGMENTS;
			const F32 texcoord_delta = 1.0f/(F32)CYLINDER_SEGMENTS;

			const F32 r = mRadius;
			const F32 h = mHeight;

			F32 base_z = -h*0.5f;
			F32 top_z = h*0.5f;

			F32 angle = 0.0f;
			F32 texcoord = 0.0f;

			for(U32 bodyi=0; bodyi<CYLINDER_SEGMENTS; ++bodyi,angle+=angle_delta,texcoord+=texcoord_delta)
			{
				F32 c = GMath::cos(angle);
				F32 s = GMath::sin(angle);

				DVTN v;
				v.normal[0]		= c;
				v.normal[1]		= s;
				v.normal[2]		= 0.0f;
				v.texcoord[0]	= texcoord;
				v.texcoord[1]	= 1.0f;
				v.point[0]		= c*r;
				v.point[1]		= s*r;
				v.point[2]		= top_z;
				vertexes.push_back(v);

				v.normal[0]		= c;
				v.normal[1]		= s;
				v.normal[2]		= 0.0f;
				v.texcoord[0]	= texcoord;
				v.texcoord[1]	= 0.0f;
				v.point[0]		= c*r;
				v.point[1]		= s*r;
				v.point[2]		= base_z;
				vertexes.push_back(v);
			}

			// do last point by hand to ensure no round off errors.
			DVTN v;
			v.normal[0]		= 1.0f;	
			v.normal[1]		= 0.0f;
			v.normal[2]		= 0.0f;
			v.texcoord[0]	= 1.0f;
			v.texcoord[1]	= 1.0f;
			v.point[0]		= r;
			v.point[1]		= 0.0f;
			v.point[2]		= top_z;
			vertexes.push_back(v);

			v.normal[0]		= 1.0f;
			v.normal[1]		= 0.0f;
			v.normal[2]		= 0.0f;
			v.texcoord[0]	= 1.0f;
			v.texcoord[1]	= 0.0f;
			v.point[0]		= r;
			v.point[1]		= 0.0f;
			v.point[2]		= base_z;
			vertexes.push_back(v);

			// QUAD_STRIP
			U32 first = 0;
			U32 count = vertexes.size();
			for(U32 i=3, pos=first;i<count;i+=2,pos+=2)
			{
				indexes.push_back(pos);
				indexes.push_back(pos+1);
				indexes.push_back(pos+2);
				indexes.push_back(pos+1);
				indexes.push_back(pos+3);
				indexes.push_back(pos+2);		
			}
		}

		// create the cylinder top and the cylinder bottom
		{
			F32 angle_delta = 2.0f*PI/(F32)CYLINDER_SEGMENTS;
			F32 texcoord_delta = 1.0f/(F32)CYLINDER_SEGMENTS;

			F32 r = mRadius;
			F32 h = mHeight;

			F32 base_z = -h*0.5f;
			F32 top_z = h*0.5f;

			F32 angle = 0.0f;
			F32 texcoord = 0.0f;

			// top
			{
				U32 count = vertexes.size();

				DVTN v;
				v.normal[0]		= 0.0f;
				v.normal[1]		= 0.0f;
				v.normal[2]		= 1.0f;
				v.texcoord[0]	= 0.5f;
				v.texcoord[1]	= 0.5f;
				v.point[0]		= 0.0f;
				v.point[1]		= 0.0f;
				v.point[2]		= top_z;
				vertexes.push_back(v);

				angle = 0.0f;
				texcoord = 0.0f;
				for(U32 topi=0; topi<CYLINDER_SEGMENTS; ++topi,angle+=angle_delta,texcoord+=texcoord_delta)
				{
					F32 c = GMath::cos(angle);
					F32 s = GMath::sin(angle);

					v.normal[0]		= 0.0f;
					v.normal[1]		= 0.0f;
					v.normal[2]		= 1.0f;
					v.texcoord[0]	= c*0.5f+0.5f;
					v.texcoord[1]	= s*0.5f+0.5f;
					v.point[0]		= c*r;
					v.point[1]		= s*r;
					v.point[2]		= top_z;
					vertexes.push_back(v);
				}

				v.normal[0]		= 0.0f;
				v.normal[1]		= 0.0f;
				v.normal[2]		= 1.0f;
				v.texcoord[0]	= 1.0f;
				v.texcoord[1]	= 0.5f;
				v.point[0]		= r;
				v.point[1]		= 0.0f;
				v.point[2]		= top_z;
				vertexes.push_back(v);

				// TRIANGLE_FAN
				U32 first = count;
				count = vertexes.size() - count;
				for(U32 i=2,pos=first+1;i<count;++i,++pos)
				{
					indexes.push_back(first);
					indexes.push_back(pos);
					indexes.push_back(pos+1);
				}
			}

			// bottom
			{
				U32 count = vertexes.size();

				DVTN v;
				v.normal[0]		= 0.0f;
				v.normal[1]		= 0.0f;
				v.normal[2]		= -1.0f;
				v.texcoord[0]	= 0.5f;
				v.texcoord[1]	= 0.5f;
				v.point[0]		= 0.0f;
				v.point[1]		= 0.0f;
				v.point[2]		= base_z;
				vertexes.push_back(v);

				angle = PI*2.0f;
				texcoord = 1.0f;
				for(U32 bottomi=0; bottomi<CYLINDER_SEGMENTS; ++bottomi,angle-=angle_delta,texcoord-=texcoord_delta)
				{
					F32 c = GMath::cos(angle);
					F32 s = GMath::sin(angle);

					v.normal[0]		= 0.0f;
					v.normal[1]		= 0.0f;
					v.normal[2]		= -1.0f;
					v.texcoord[0]	= c*0.5f+0.5f;
					v.texcoord[1]	= s*0.5f+0.5f;
					v.point[0]		= c*r;
					v.point[1]		= s*r;
					v.point[2]		= base_z;
					vertexes.push_back(v);
				}

				v.normal[0]		= 0.0f;
				v.normal[1]		= 0.0f;
				v.normal[2]		= -1.0f;
				v.texcoord[0]	= 1.0f;
				v.texcoord[1]	= 0.5f;
				v.point[0]		= r;
				v.point[1]		= 0.0f;
				v.point[2]		= base_z;
				vertexes.push_back(v);

				// TRIANGLE_FAN
				U32 first = count;
				count = vertexes.size() - count;
				for(U32 i=2,pos=first+1;i<count;++i,++pos)
				{
					indexes.push_back(first);
					indexes.push_back(pos);
					indexes.push_back(pos+1);
				}
			}
		}

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
			vd_ptr->Size(3*sizeof(U32) + 4*sizeof(U8) + vertexes.size()*sizeof(DVTN));
			U8*data_ptr = (U8*)vd_ptr->Ptr();
			*(U32*)data_ptr = MAKEFOURCC('G','V','B','O');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = vertexes.size();
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(DVTN); 
			data_ptr += sizeof(U32);
			*(U8*)data_ptr = 3;
			data_ptr += sizeof(U8);
			*(U8*)data_ptr = VertexBuffer::VT_3F;
			data_ptr += sizeof(U8);
			*(U8*)data_ptr = VertexBuffer::VT_2F;
			data_ptr += sizeof(U8);
			*(U8*)data_ptr = VertexBuffer::VT_3F;
			data_ptr += sizeof(U8);
			::memcpy(data_ptr, &vertexes[0], vertexes.size()*sizeof(DVTN));
			data_ptr += vertexes.size()*sizeof(DVTN);
			vb_ptr->Load(vd_ptr.Ptr());
		}
		mPrimitivePtr->SetVertexBuffer(vb_ptr.Ptr());

		// build the index
		IndexBufferPtr ib_ptr = GNEW(IndexBuffer); CHECK(ib_ptr);
		{
			GDataPtr id_ptr = GNEW(GData); CHECK(id_ptr);
			id_ptr->Size(3*sizeof(U32) + indexes.size()*sizeof(U32));
			U8*data_ptr = (U8*)id_ptr->Ptr();
			*(U32*)data_ptr = MAKEFOURCC('G','I','B','O');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = indexes.size(); 
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(U32); 
			data_ptr += sizeof(U32);
			::memcpy(data_ptr, &indexes[0], indexes.size()*sizeof(U32));
			data_ptr += indexes.size()*sizeof(U32);
			ib_ptr->Load(id_ptr.Ptr());
		}
		mPrimitivePtr->SetIndexBuffer(ib_ptr.Ptr());	

		// build the bounding box
		mBox.set(MAX_F32,MAX_F32,MAX_F32,MIN_F32,MIN_F32,MIN_F32);
		for(U32 i = 0; i < vertexes.size(); i++)mBox.expand(vertexes[i].point);
		mPrimitivePtr->SetBox(mBox);

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
