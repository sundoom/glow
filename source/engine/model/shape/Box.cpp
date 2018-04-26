//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	Box::Box()
	{
		// build the default box
		mDimension.set(1,1,1);
		build();
	}

	Box::~Box() 
	{
		
	}	

	/*
	====================
	dimension
	====================
	*/
	VOID Box::dimension(const Vector3& d)
	{
		mDimension = d;
		build();
	}

	const Vector3& Box::dimension() const
	{
		return mDimension;
	}

	/*
	====================
	trace
	====================
	*/
	F32 Box::trace(const Vector3& s, const Vector3& e) const
	{
		GUARD(Box::trace);

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
	VOID Box::build()
	{
		GUARD(Box::build);

		// clear the old keys
		mKeys.clear();

		Vector3& d = mDimension;		

		// build the vertexes
		DVTN vertexes[] = 
		{
			{-d[0], -d[1],  d[2], 0.0f, 1.0f, 0.0, -1.0,  0.0},
			{-d[0], -d[1], -d[2], 0.0f, 0.0f, 0.0, -1.0,  0.0},
			{ d[0], -d[1], -d[2], 1.0f, 0.0f, 0.0, -1.0,  0.0},
			{ d[0], -d[1],  d[2], 1.0f, 1.0f, 0.0, -1.0,  0.0},
			{ d[0],  d[1],  d[2], 0.0f, 1.0f, 0.0,  1.0,  0.0},
			{ d[0],  d[1], -d[2], 0.0f, 0.0f, 0.0,  1.0,  0.0},
			{-d[0],  d[1], -d[2], 1.0f, 0.0f, 0.0,  1.0,  0.0},
			{-d[0],  d[1],  d[2], 1.0f, 1.0f, 0.0,  1.0,  0.0},
			{ d[0], -d[1],  d[2], 0.0f, 1.0f, 1.0,  0.0,  0.0},
			{ d[0], -d[1], -d[2], 0.0f, 0.0f, 1.0,  0.0,  0.0},
			{ d[0],  d[1], -d[2], 1.0f, 0.0f, 1.0,  0.0,  0.0},
			{ d[0],  d[1],  d[2], 1.0f, 1.0f, 1.0,  0.0,  0.0},
			{-d[0],  d[1],  d[2], 0.0f, 1.0f, 1.0,  0.0,  0.0},
			{-d[0],  d[1], -d[2], 0.0f, 0.0f, 1.0,  0.0,  0.0},
			{-d[0], -d[1], -d[2], 1.0f, 0.0f, 1.0,  0.0,  0.0},
			{-d[0], -d[1],  d[2], 1.0f, 1.0f, 1.0,  0.0,  0.0},
			{-d[0],  d[1],  d[2], 0.0f, 1.0f, 0.0,  0.0,  1.0},
			{-d[0], -d[1],  d[2], 0.0f, 0.0f, 0.0,  0.0,  1.0},
			{ d[0], -d[1],  d[2], 1.0f, 0.0f, 0.0,  0.0,  1.0},
			{ d[0],  d[1],  d[2], 1.0f, 1.0f, 0.0,  0.0,  1.0},
			{ d[0],  d[1], -d[2], 0.0f, 1.0f, 0.0,  0.0, -1.0},
			{ d[0], -d[1], -d[2], 0.0f, 0.0f, 0.0,  0.0, -1.0},
			{-d[0], -d[1], -d[2], 1.0f, 0.0f, 0.0,  0.0, -1.0},
			{-d[0],  d[1], -d[2], 1.0f, 1.0f, 0.0,  0.0, -1.0},
		};	

		// build the indexes
		std::vector<U16>indexes;
		U32 count = sizeof(vertexes)/sizeof(DVTN);
		for(U32 i=3,pos=0;i<count;i+=4,pos+=4)
		{
			indexes.push_back(pos);
			indexes.push_back(pos+1);
			indexes.push_back(pos+2);
			indexes.push_back(pos);
			indexes.push_back(pos+2);
			indexes.push_back(pos+3);
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
			vd_ptr->Size(3*sizeof(U32) + 4*sizeof(U8) + sizeof(vertexes));
			U8*data_ptr = (U8*)vd_ptr->Ptr();
			*(U32*)data_ptr = MAKEFOURCC('G','V','B','O');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(vertexes)/sizeof(DVTN); 
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
			::memcpy(data_ptr, vertexes, sizeof(vertexes));
			data_ptr += sizeof(vertexes);
			vb_ptr->Load(vd_ptr.Ptr());
		}
		mPrimitivePtr->SetVertexBuffer(vb_ptr.Ptr());

		// build the index
		IndexBufferPtr ib_ptr = GNEW(IndexBuffer); CHECK(ib_ptr);
		{
			GDataPtr id_ptr = GNEW(GData); CHECK(id_ptr);
			id_ptr->Size(3*sizeof(U32) + indexes.size()*sizeof(U16));
			U8*data_ptr = (U8*)id_ptr->Ptr();
			*(U32*)data_ptr = MAKEFOURCC('G','I','B','O');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = indexes.size(); 
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(U16); 
			data_ptr += sizeof(U32);
			::memcpy(data_ptr, &indexes[0], indexes.size()*sizeof(U16));
			data_ptr += indexes.size()*sizeof(U16);
			ib_ptr->Load(id_ptr.Ptr());
		}		
		mPrimitivePtr->SetIndexBuffer(ib_ptr.Ptr());	

		// build the bounding box
		mBox.set(MAX_F32,MAX_F32,MAX_F32,MIN_F32,MIN_F32,MIN_F32);
		for(U32 i = 0; i < sizeof(vertexes)/sizeof(DVTN); i++)mBox.expand(vertexes[i].point);
		mPrimitivePtr->SetBox(mBox);

		UNGUARD;
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
