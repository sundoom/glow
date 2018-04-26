//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	Chunk::Chunk()
	{
		
	}
	
	Chunk::~Chunk()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID Chunk::Load(const VOID* data)
	{
		GUARD(Chunk::Load);		

		// build the vertex buffer
		VertexBufferPtr vb_ptr = GNEW(VertexBuffer); CHECK(vb_ptr);
		{
			DVT vertexes[] = 
			{
				{0.0,			0.0,			0.0,	0.0,	1.0},
				{CHUNK_STRIDE,	0.0,			0.0,	1.0,	1.0},
				{CHUNK_STRIDE,	CHUNK_STRIDE,	0.0,	1.0,	0.0},
				{0.0,			CHUNK_STRIDE,	0.0,	0.0,	0.0},
			};	

			GDataPtr vd_ptr = GNEW(GData); CHECK(vd_ptr);
			vd_ptr->Size(3*sizeof(U32) + 3*sizeof(U8) + sizeof(vertexes));
			U8*data_ptr = (U8*)vd_ptr->Ptr();
			*(U32*)data_ptr = MAKEFOURCC('G','V','B','O');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(vertexes)/sizeof(DVT); 
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(DVT); 
			data_ptr += sizeof(U32);
			*(U8*)data_ptr = 2;
			data_ptr += sizeof(U8);
			*(U8*)data_ptr = VertexBuffer::VT_3F;
			data_ptr += sizeof(U8);
			*(U8*)data_ptr = VertexBuffer::VT_2F;
			data_ptr += sizeof(U8);
			::memcpy(data_ptr, vertexes, sizeof(vertexes));
			data_ptr += sizeof(vertexes);
			vb_ptr->Load(vd_ptr.Ptr());
		}		

		// build the index
		IndexBufferPtr ib_ptr = GNEW(IndexBuffer); CHECK(ib_ptr);
		{
			const U16 indexes[] = { 3, 0, 2, 2, 0, 1 };
			GDataPtr id_ptr = GNEW(GData); CHECK(id_ptr);
			id_ptr->Size(3*sizeof(U32) + sizeof(indexes));
			U8*data_ptr = (U8*)id_ptr->Ptr();
			*(U32*)data_ptr = MAKEFOURCC('G','I','B','O');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(indexes)/sizeof(U16); 
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(U16); 
			data_ptr += sizeof(U32);
			::memcpy(data_ptr, indexes, sizeof(indexes));
			data_ptr += sizeof(indexes);
			ib_ptr->Load(id_ptr.Ptr());
		}

		U8*data_ptr = (U8*)data;

		// check the chunk header		
		CHECK(*(U32*)data_ptr == (MAKEFOURCC('G','C','H','K')));
		data_ptr += sizeof(U32);

		// get the layer
		U32 num_layer = *(U32*)data_ptr;
		data_ptr += sizeof(U32);
		for(U32 i = 0; i < num_layer; i++)
		{			
			Primitive* primitive = GNEW(Primitive); CHECK(primitive);
			primitive->SetType(Primitive::PT_TRIANGLES);
			primitive->SetVertexBuffer(vb_ptr.Ptr());
			primitive->SetIndexBuffer(ib_ptr.Ptr());	
			primitive->SetConstant("gWVP",GNEW(Constant(Matrix())));
			
			// get the color texture			
			Str texture_name;
			U32 len = *(U32*)data_ptr;
			data_ptr += sizeof(U32);
			texture_name.resize(len, 0);
			memcpy(&texture_name[0], data_ptr, len*sizeof(CHAR));
			data_ptr += len*sizeof(CHAR);
			KeyPtr texture_key_ptr = Key::Find(texture_name.c_str());
			if(texture_key_ptr == NULL)
			{
				const Image* image = Image::Load(GLoad(texture_name.c_str())); CHECK(image);
				BaseTexture* base_texture = GNEW(Texture); CHECK(base_texture);
				base_texture->Load(image);
				texture_key_ptr = GNEW(Key(texture_name.c_str(), base_texture)); CHECK(texture_key_ptr);
			}
			mKeys.push_back(texture_key_ptr);
			primitive->SetConstant("gColorTex",GNEW(Constant((BaseTexture*)texture_key_ptr->Ptr())));						

			// get the scale st
			F32 s = *(F32*)data_ptr;
			data_ptr += sizeof(F32);
			F32 t = *(F32*)data_ptr;
			data_ptr += sizeof(F32);
			primitive->SetConstant("gScaleST",GNEW(Constant(Vector4(s,t,0,0))));

			// get the alpha
			U32 width = *(U32*)data_ptr;
			data_ptr += sizeof(U32);
			U32 height = *(U32*)data_ptr;
			data_ptr += sizeof(U32);
			Image* image = GNEW(Image); CHECK(image);
			image->Width(width);
			image->Height(height);
			image->PixelFormat(PF_ALPHA);
			image->DataType(DT_UNSIGNED_BYTE);
			image->MipmapCount(1);
			image->Mipmap(0,data_ptr,width*height*sizeof(U8));
			data_ptr += width*height*sizeof(U8);
			BaseTexture *alpha_texture = GNEW(Texture); CHECK(alpha_texture);
			alpha_texture->Load(image);
			primitive->SetConstant("gAlphaTex",GNEW(Constant(alpha_texture)));

			// load the shader
			Str shader_name = "shader/chunk.xml";
			KeyPtr shader_key_ptr = Key::Find(shader_name.c_str());
			if(shader_key_ptr == NULL)
			{
				Shader*shader = GNEW(Shader); CHECK(shader);
				shader->Load(GLoad(shader_name.c_str()));
				shader_key_ptr= GNEW(Key(shader_name.c_str(), shader)); CHECK(shader_key_ptr);
			}
			mKeys.push_back(shader_key_ptr);
			if(i==0) primitive->SetShader(dynamic_cast<Shader*>(shader_key_ptr->Ptr()), "base");
			else primitive->SetShader(dynamic_cast<Shader*>(shader_key_ptr->Ptr()), "layer");

			// add the primitive to the table
			mPrimitives.push_back(primitive);
		}	

		UNGUARD;
	}

	/*
	====================
	Draw
	====================
	*/
	VOID Chunk::Draw(Graph *gc)
	{
		GUARD(Chunk::Draw);

		// get the frustum
		Frustum frustum(gc->wvp());
		BoundingBox box(0.0,0.0,0.0,CHUNK_STRIDE,CHUNK_STRIDE,1.0);
		if(!frustum.boxInFrustum(box.min(),box.max())) return;

		// draw all of the primitive
		for(std::vector<PrimitivePtr>::iterator it = mPrimitives.begin(); it != mPrimitives.end(); ++it)
		{
			PrimitivePtr& primitive = (*it);
			primitive->GetConstant("gWVP")->SetMatrix(gc->wvp());
			gc->draw(primitive.Ptr());
		}

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
