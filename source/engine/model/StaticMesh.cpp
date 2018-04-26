//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	StaticMesh::StaticMesh()
	{
		
	}

	StaticMesh::~StaticMesh()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID StaticMesh::Load(const GData* data)
	{
		GUARD(StaticMesh::load);

		CHECK(data);

		// get the mesh config
		GConfigPtr mesh_config_ptr = GConfig::Load((CHAR*)data->Ptr()); CHECK(mesh_config_ptr);

		// check it if it is a mesh?
		CHECK(Str(mesh_config_ptr->GetValue()) == "static_mesh");

		// prase the mesh`s children
		const GConfig* mesh_child_config_ptr = NULL;
		for( U32 i = 0; mesh_child_config_ptr = mesh_config_ptr->GetChild(i); i++ )
		{			
			// prase the primitive
			if( Str(mesh_child_config_ptr->GetValue()) == "primitive" )
			{
				const GConfig *primitive_config_ptr = (const GConfig *)mesh_child_config_ptr;

				// build a new primitive
				Primitive* primitive = GNEW(Primitive); CHECK(primitive);

				// get the primitive type
				Str type = primitive_config_ptr->GetAttribute("type");
				if(type=="PT_POINTS") primitive->SetType(Primitive::PT_POINTS);
				else if(type=="PT_LINES") primitive->SetType(Primitive::PT_LINES);
				else if(type=="PT_LINE_LOOP") primitive->SetType(Primitive::PT_LINE_LOOP);
				else if(type=="PT_LINE_STRIP") primitive->SetType(Primitive::PT_LINE_STRIP);
				else if(type=="PT_TRIANGLES") primitive->SetType(Primitive::PT_TRIANGLES);
				else if(type=="PT_TRIANGLE_STRIP") primitive->SetType(Primitive::PT_TRIANGLE_STRIP);
				else if(type=="PT_TRIANGLE_FAN") primitive->SetType(Primitive::PT_TRIANGLE_FAN);
				else if(type=="PT_QUADS") primitive->SetType(Primitive::PT_QUADS);
				else if(type=="PT_QUAD_STRIP") primitive->SetType(Primitive::PT_QUAD_STRIP);
				else if(type=="PT_POLYGON") primitive->SetType(Primitive::PT_POLYGON);
				else GAssert( VA("The primitive`s type(%s) is unknown!", type.c_str()) );

				const GConfig* primitive_child_config_ptr = NULL;
				for( U32 j = 0; primitive_child_config_ptr = primitive_config_ptr->GetChild(j); j++ )
				{
					if( Str(primitive_child_config_ptr->GetValue()) == "vertex" )
					{
						const GConfig *vertex_config_ptr = (const GConfig *)primitive_child_config_ptr;

						// get the vertex format
						Str format_string = vertex_config_ptr->GetAttribute("format"); CHECK(format_string!= "");
						std::vector<U8>formats;
						U32 elem_count = 0, elem_size = 0;
						{
							std::vector<Str>tokens = GTokenize(format_string);
							U32 size = tokens.size();
							for(U32 k = 0; k < size; k++)
							{
								if(tokens[k]=="VT_1I")		formats.push_back(VertexBuffer::VT_1I), elem_count+=1, elem_size+=1*sizeof(I32);
								else if(tokens[k]=="VT_2I") formats.push_back(VertexBuffer::VT_2I), elem_count+=2, elem_size+=2*sizeof(I32);
								else if(tokens[k]=="VT_3I") formats.push_back(VertexBuffer::VT_3I), elem_count+=3, elem_size+=3*sizeof(I32);
								else if(tokens[k]=="VT_4I") formats.push_back(VertexBuffer::VT_4I), elem_count+=4, elem_size+=4*sizeof(I32);
								else if(tokens[k]=="VT_1F") formats.push_back(VertexBuffer::VT_1F), elem_count+=1, elem_size+=1*sizeof(F32);
								else if(tokens[k]=="VT_2F") formats.push_back(VertexBuffer::VT_2F), elem_count+=2, elem_size+=2*sizeof(F32);
								else if(tokens[k]=="VT_3F") formats.push_back(VertexBuffer::VT_3F), elem_count+=3, elem_size+=3*sizeof(F32);
								else if(tokens[k]=="VT_4F") formats.push_back(VertexBuffer::VT_4F), elem_count+=4, elem_size+=4*sizeof(F32);
								else GAssert( VA("The vertex`s format(%s) is unknown!", tokens[k].c_str()) );;
							}
						}

						// get the vertex data
						std::vector<Str>tokens = GTokenize(vertex_config_ptr->GetText());
						U32 size = tokens.size();
						CHECK(size % elem_count == 0);
						GDataPtr vd_ptr = GNEW(GData); CHECK(vd_ptr);
						vd_ptr->Size(3*sizeof(U32)+(1+formats.size())*sizeof(U8)+size/elem_count*elem_size);
						U8*ptr = (U8*)vd_ptr->Ptr();
						(*((U32*)ptr)) = (MAKEFOURCC('G','V','B','O')); 
						ptr += sizeof(U32);
						U32 count = size/elem_count;
						(*((U32*)ptr)) = count;
						ptr += sizeof(U32);
						U32 stride = elem_size;
						(*((U32*)ptr)) = stride;
						ptr += sizeof(U32);
						U8 format_size = formats.size();
						(*((U8*)ptr)) = format_size;
						ptr += sizeof(U8);
						memcpy(ptr,&formats[0],formats.size());
						ptr += format_size;
						Str*token = &tokens[0];
						for(U32 k = 0; k < count; k++)
						{
							for( U32 m = 0; m < format_size; m++ )
							{
								switch(formats[m])
								{
								case VertexBuffer::VT_1I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VertexBuffer::VT_2I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VertexBuffer::VT_3I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VertexBuffer::VT_4I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VertexBuffer::VT_1F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;			   
								case VertexBuffer::VT_2F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;			   
								case VertexBuffer::VT_3F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;			   
								case VertexBuffer::VT_4F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;
								}
							}
						}

						// build the vertex buffer for the primitive
						VertexBuffer* vb_ptr = GNEW(VertexBuffer); CHECK(vb_ptr);
						vb_ptr->Load(vd_ptr.Ptr());
						primitive->SetVertexBuffer(vb_ptr);
					}
					// parse the index
					else if( Str(primitive_child_config_ptr->GetValue()) == "index" )
					{
						const GConfig *index_config_ptr = (const GConfig *)primitive_child_config_ptr;

						std::vector<Str>tokens = GTokenize(index_config_ptr->GetText());

						// get the index stride
						U32 index_stride = ::atoi(index_config_ptr->GetAttribute("stride"));

						// get the index count
						U32 index_count = tokens.size(); CHECK(index_count!=0);

						// get the index data
						GDataPtr id_ptr = GNEW(GData); CHECK(id_ptr);
						id_ptr->Size(3*sizeof(U32)+index_count*index_stride);
						U8*ptr = (U8*)id_ptr->Ptr();
						(*(U32*)ptr) = (MAKEFOURCC('G','I','B','O'));
						ptr += sizeof(U32);
						(*(U32*)ptr) = index_count;
						ptr += sizeof(U32);
						(*(U32*)ptr) = index_stride;
						ptr += sizeof(U32);						
						if(index_stride==sizeof(U16))
						{
							U16* data = (U16*)ptr;
							for(U32 k = 0; k < tokens.size(); k++)data[k] = ::atoi(tokens[k].c_str());
						}
						else if(index_stride==sizeof(U32))
						{
							U32* data = (U32*)ptr;
							for(U32 k = 0; k < tokens.size(); k++)data[k] = ::atoi(tokens[k].c_str());						
						}
						else
						{
							GAssert( VA("The index`s type(%s) is unknown!", type.c_str()) );
						}

						// build the index buffer of primitive
						IndexBuffer* ib_ptr = GNEW(IndexBuffer); CHECK(ib_ptr);
						ib_ptr->Load(id_ptr.Ptr());
						primitive->SetIndexBuffer(ib_ptr);
					}
					// parse the shader
					else if( Str(primitive_child_config_ptr->GetValue()) == "shader" )
					{
						const GConfig *shader_config_ptr = (const GConfig *)primitive_child_config_ptr;

						// set the shader
						Str shader_name = shader_config_ptr->GetAttribute("name"); CHECK(shader_name != "");
						Str pass_name = shader_config_ptr->GetAttribute("pass"); CHECK(pass_name != "");
						KeyPtr shader_key_ptr = Key::Find(shader_name.c_str());
						if(shader_key_ptr == NULL)
						{
							Shader*shader = GNEW(Shader); CHECK(shader);
							shader->Load(GLoad(shader_name.c_str()));
							shader_key_ptr = GNEW(Key(shader_name.c_str(), shader)); CHECK(shader_key_ptr);
						}
						mKeys.push_back(shader_key_ptr);
						primitive->SetShader(dynamic_cast<Shader*>(shader_key_ptr->Ptr()),pass_name.c_str());

						// parse the constant of the shader
						const GConfig *constant_config_ptr = NULL;
						for( U32 k = 0; constant_config_ptr = shader_config_ptr->GetChild(k); k++ )
						{
							CHECK( Str(constant_config_ptr->GetValue()) == "constant" );

							// get the name of the constant
							Str constant_name = constant_config_ptr->GetAttribute("name"); CHECK(constant_name!= "");

							// get the type of the constant
							Str constant_type = constant_config_ptr->GetAttribute("type"); CHECK(constant_type!= "");

							// get the value of the constant
							Str constant_value = constant_config_ptr->GetText(); CHECK(constant_value!= "");

							// the new constant
							ConstantPtr constant_ptr = GNEW(Constant); CHECK(constant_ptr);

							// get the constant type and the value
							if(constant_type == "CT_INT")
							{
								I32 value;
								value = ::atoi(constant_value.c_str());
								constant_ptr->SetInt(value);
							}
							else if(constant_type == "CT_INT_ARRAY") 
							{
								std::vector<I32>value;
								std::vector<Str>tokens = GTokenize(constant_value);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = ::atoi(tokens[l].c_str());
								constant_ptr->SetIntArray((I32*)(&value[0]), value.size());
							}
							else if(constant_type == "CT_FLOAT")
							{
								F32 value;
								value = (F32)::atof(constant_value.c_str());
								constant_ptr->SetFloat(value);
							}
							else if(constant_type == "CT_FLOAT_ARRAY")
							{
								std::vector<F32>value;
								std::vector<Str>tokens = GTokenize(constant_value);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetFloatArray((F32*)(&value[0]), value.size());
							}
							else if(constant_type == "CT_VECTOR")
							{
								Vector4 value;
								std::vector<Str>tokens = GTokenize(constant_value); CHECK(tokens.size() == 4);
								for(U32 l = 0; l < 4; l++) value.ptr()[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetVector(value);
							}
							else if( constant_type == "CT_VECTOR_ARRAY" )
							{
								std::vector<F32>value;
								std::vector<Str>tokens = GTokenize(constant_value); CHECK(tokens.size()%4 == 0);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetVectorArray((Vector4*)(&value[0]), value.size()/4);
							}
							else if( constant_type == "CT_MATRIX" ) 
							{
								Matrix value;
								std::vector<Str>tokens = GTokenize(constant_value); CHECK(tokens.size() == 16);
								for(U32 l = 0; l < 16; l++) value.ptr()[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetMatrix(value);
							}
							else if( constant_type == "CT_MATRIX_ARRAY" ) 
							{
								std::vector<F32>value;
								std::vector<Str>tokens = GTokenize(constant_value); CHECK(tokens.size()%16 == 0);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetMatrixArray((Matrix*)(&value[0]), value.size()/16);
							}
							else if( constant_type == "CT_TEXTURE" ) 
							{
								// build the texture
								Str tex_name = constant_value;
								KeyPtr tex_key_ptr = Key::Find(tex_name.c_str());
								if(tex_key_ptr == NULL)
								{
									const Image* image = Image::Load(GLoad(tex_name.c_str())); CHECK(image);
									BaseTexture *base_texture = GNEW(Texture); CHECK(base_texture);
									base_texture->Load(image);
									tex_key_ptr = GNEW(Key(tex_name.c_str(), base_texture)); CHECK(tex_key_ptr);
								}
								mKeys.push_back(tex_key_ptr);
								constant_ptr->SetTexture(dynamic_cast<BaseTexture*>(tex_key_ptr->Ptr()));
							}
							else
							{
								GAssert( VA("The type(%s) of the constant(%s) is error.\n", constant_type.c_str() , constant_name.c_str() ) );
							}

							// add the constant to the map
							primitive->SetConstant(constant_name.c_str(), constant_ptr.Ptr());
						}
					}
					// parse the bounding box
					else if( Str(primitive_child_config_ptr->GetValue()) == "bounding_box" )
					{
						const GConfig *box_config_ptr = (const GConfig *)primitive_child_config_ptr;
						std::vector<Str>tokens = GTokenize(box_config_ptr->GetText());
						CHECK(tokens.size() == 6);
						BoundingBox box;
						box.set((F32)::atof(tokens[0].c_str()),(F32)::atof(tokens[1].c_str()),(F32)::atof(tokens[2].c_str()),(F32)::atof(tokens[3].c_str()),(F32)::atof(tokens[4].c_str()),(F32)::atof(tokens[5].c_str()));
						primitive->SetBox(box);
					}
					else
					{
						GAssert( VA("The primitive`s keyword(%s) is unknown!", primitive_child_config_ptr->GetValue()) );
					}
				}

				// add the primitive to the table
				mPrimitives.push_back(primitive);
			}
			else
			{
				GAssert( VA("The static mesh`s keyword(%s) is unknown!", mesh_child_config_ptr->GetValue()) );
			}
		}

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Draw
	====================
	*/
	VOID StaticMesh::Draw(Graph *gc)
	{
		GUARD(StaticMesh::Draw);

		// get the frustum
		Frustum frustum(gc->wvp());

		// draw all of the primitive
		for(U32 i = 0; i < mPrimitives.size(); i++)
		{
			PrimitivePtr& primitive = mPrimitives[i];
			const BoundingBox& box = primitive->GetBox();
			if(!frustum.boxInFrustum(box.min(),box.max()))continue;
			primitive->GetConstant("gWVP")->SetMatrix(gc->wvp());
			gc->draw(primitive.Ptr());
		}

		UNGUARD;
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

