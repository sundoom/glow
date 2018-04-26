//////////////////////////////////////////////////////////////////////////
// G3DSExport.cpp
//////////////////////////////////////////////////////////////////////////

#include "Local.h"

//////////////////////////////////////////////////////////////////////////

G3DSExport::G3DSExport()
{

}

G3DSExport::~G3DSExport() 
{

}

//////////////////////////////////////////////////////////////////////////

/*
====================
ExtCount
====================
*/
int G3DSExport::ExtCount()
{
	return 1;
}

/*
====================
Ext
====================
*/
const TCHAR* G3DSExport::Ext(int n)
{ 
	switch(n) 
	{
	case 0:
		return _T("G3DS");
	}
	return _T("");
}

/*
====================
LongDesc
====================
*/
const TCHAR* G3DSExport::LongDesc()
{ 
	return _T("Glow 3D Skeletal Mesh Export for 3D Studio Max");	
}

/*
====================
ShortDesc
====================
*/
const TCHAR* G3DSExport::ShortDesc()
{ 
	return _T("Glow 3D Skeletal Mesh Export");
}

/*
====================
AuthorName
====================
*/
const TCHAR* G3DSExport::AuthorName()
{ 
	return _T("SunJun");
}

/*
====================
CopyrightMessage
====================
*/
const TCHAR* G3DSExport::CopyrightMessage()
{ 
	return _T("Copyright (c) 2009 - 2010 , Glow");
}

/*
====================
OtherMessage1
====================
*/
const TCHAR* G3DSExport::OtherMessage1()
{ 
	return _T("");
}

/*
====================
OtherMessage2
====================
*/
const TCHAR* G3DSExport::OtherMessage2()
{
	return _T("");
}

/*
====================
Version
====================
*/
unsigned int G3DSExport::Version()
{
	return 101;
}

/*
====================
ShowAbout
====================
*/
void G3DSExport::ShowAbout(HWND hWnd)
{
	
}

/*
====================
ShowAbout
====================
*/
BOOL G3DSExport::SupportsOptions( int ext, DWORD options )
{
	return options == SCENE_EXPORT_SELECTED;
}

/*
====================
DoExport
====================
*/
int G3DSExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	// clear the old data
	mSkins.clear();

	// normalize the file path
	Str path = UnifySlashes(name);

	// get the export dir
	mPath = UnifySlashes(i->GetDir(APP_EXPORT_DIR));
	mPath += '/';
	if( !strstr( path.c_str(), mPath.c_str() ) )
	{
		G3DAssert("The export path(%s) is illegal!",name);
		return 1;
	}

	// this has to be done for edit normals modifier to work properly
	i->SetCommandPanelTaskMode(TASK_MODE_MODIFY);

	// get the animation range
	Interval inter = i->GetAnimRange();
	mTime = i->GetTime();

	// recursive export all of the mesh node
	if(options == SCENE_EXPORT_SELECTED)
	{
		ExportSelected(i->GetRootNode());
	}
	else
	{
		ExportVisible(i->GetRootNode());
	}	

	// save the mesh file
	if(Save(path) == 0) { G3DAssert("Fail to save the G3DS file : %s!", path.c_str() ); return 1; }

	// clear the old data
	mSkins.clear();

	return 1;
}

//////////////////////////////////////////////////////////////////////////
/*
====================
GetModifier
====================
*/
static Modifier* GetModifier( INode* i_node, Class_ID cid )
{
	// Get object from node. Abort if no object.
	Object* object = i_node->GetObjectRef();
	if(!object) return NULL;

	// Is derived object ?
	if( object->SuperClassID() == GEN_DERIVOB_CLASS_ID )
	{
		// Yes -> Cast.
		IDerivedObject* derived_object = static_cast< IDerivedObject* >( object );

		// Iterate over all entries of the modifier stack.
		int modStackIndex = 0;
		while( modStackIndex < derived_object->NumModifiers() )
		{
			// Get current modifier.
			Modifier* modifier = derived_object->GetModifier( modStackIndex );

			// Is this ?
			if( modifier->ClassID() == cid )
			{
				// Yes -> Exit.
				return modifier;
			}

			// Next modifier stack entry.
			modStackIndex++;
		}
	}

	// Not found.
	return NULL;
}

/*
====================
NormalAngle
====================
*/
static float NormalAngle( Point3 v1, Point3 v2 )
{
	float len = Length( v1 );
	if (len == 0) return 0;
	v1 /= len;

	len = Length( v2 );
	if (len == 0) return 0;
	v2 /= len;

	float normal_angle = DotProd( v1, v2 );

	normal_angle = min( 1.f, max( normal_angle, -1.f ) );

	return acosf( normal_angle );
}

//////////////////////////////////////////////////////////////////////////

/*
====================
ExportVisible
====================
*/
void G3DSExport::ExportVisible( INode* i_node )
{
	if(!i_node->IsHidden() && i_node->Renderable() )
	{		
		Modifier *modifier = GetModifier(i_node,SKIN_CLASSID);
		if(modifier)
		{
			// disables the modifier in the viewports 
			modifier->DisableMod();

			// gather the skin data
			GatherSkin(i_node);

			// enables the modifier in the history browser 
			modifier->EnableMod();
		}		
	}

	// process the child node
	for(int i = 0; i < i_node->NumberOfChildren(); i++)
	{
		ExportVisible(i_node->GetChildNode(i));
	}
}

/*
====================
ExportSelected
====================
*/
void G3DSExport::ExportSelected( INode* i_node )
{
	if(i_node->Selected() && i_node->Renderable())
	{
		Modifier *modifier = GetModifier(i_node,SKIN_CLASSID);
		if(modifier)
		{
			// disables the modifier in the viewports 
			modifier->DisableMod();

			// gather the skin data
			GatherSkin(i_node);

			// enables the modifier in the history browser 
			modifier->EnableMod();
		}
	}

	// process the child node
	for(int i = 0; i < i_node->NumberOfChildren(); i++)
	{
		ExportSelected(i_node->GetChildNode(i));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
====================
GatherSkin
====================
*/
void G3DSExport::GatherSkin(INode* i_node)
{
	SKIN skin;

	// get the name of the node
	skin.name = i_node->GetName();

	// get the skin interface
	Modifier *modifier = GetModifier(i_node,SKIN_CLASSID);
	ISkin* i_skin = (ISkin*)modifier->GetInterface(I_SKIN);
	MAX_CHECK(i_skin);

	// convert to the triangle type
	Mesh* i_mesh = NULL;
	Object* obj = i_node->EvalWorldState(mTime).obj;
	if(obj && ( obj->SuperClassID() == GEOMOBJECT_CLASS_ID ))
	{
		if(obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
		{ 
			TriObject *tri_obj = (TriObject*)obj->ConvertToType(mTime, Class_ID(TRIOBJ_CLASS_ID, 0)); MAX_CHECK(tri_obj);
			i_mesh = &tri_obj->mesh;
		}
	}
	MAX_CHECK(i_mesh&&i_mesh->getNumFaces()&&i_mesh->getNumVerts());

	// get the material
	skin.texture = "textures/default.tga";
	Mtl* mtl = i_node->GetMtl();
	if(mtl && (mtl->ClassID()==Class_ID(DMTL_CLASS_ID, 0)) && ((StdMat*)mtl)->MapEnabled(ID_DI)) 
	{
		Texmap *texmap = mtl->GetSubTexmap(ID_DI);
		if(texmap && texmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
		{
			skin.texture = UnifySlashes(((BitmapTex *)texmap)->GetMapName());
			if( !strstr( skin.texture.c_str(), mPath.c_str() ) )
			{
				G3DAssert("The material(%s) is error : the texture path(%s) is illegal!",mtl->GetName(), skin.texture.c_str());
			}
			else
			{
				skin.texture = strstr(skin.texture.c_str(),mPath.c_str()) + strlen(mPath.c_str());
			}
		}
	}

	// if it has uvs
	int map_count = i_mesh->getNumMaps();
	bool has_uvs = i_mesh->getNumTVerts() && i_mesh->tvFace;
	if(!(has_uvs&&map_count)) { G3DAssert("The skin(%s) has not the uv coordinates.",skin.name.c_str()); return; }

	// get the transform
	Matrix3 mesh_matrix = i_node->GetObjectTM(mTime);
	Matrix3 node_matrix = i_node->GetNodeTM(mTime);
	Matrix3 transform = mesh_matrix * Inverse(node_matrix);

	// get the points
	skin.points.assign(i_mesh->verts, i_mesh->verts+i_mesh->getNumVerts());

	// get the triangles
	for(int i = 0; i < i_mesh->getNumFaces(); i++)
	{
		Face& face = i_mesh->faces[i];

		TRIANGLE tri;		
		tri.smoothing = face.smGroup;
		for(int j = 0; j < 3; j++)
		{
			VPTNIS v;
			v.pos = transform * i_mesh->verts[face.v[j]];

			// get the uv
			UVVert * map_verts = i_mesh->mapVerts(1);
			TVFace * map_faces = i_mesh->mapFaces(1);
			v.uv = reinterpret_cast<Point2&>(map_verts[map_faces[i].t[j]]);
			v.uv.y = 1 - v.uv.y;

			// initialize the normal
			v.normal = Point3::Origin;

			// get the vertex index
			v.index = face.v[j];

			// get the smoothing group
			v.smoothing = face.smGroup;			

			// set the index for the triangle
			tri.index0[j] = v.index;

			// reassemble the vertex list
			tri.index1[j] = AddVertex(skin, v);
		}

		// add the triangle to the table
		skin.triangles.push_back(tri);
	}

	// build the index map
	for( int i = 0; i < skin.vertexes.size(); i++ )
	{
		skin.vertex_index_map[skin.vertexes[i].index].push_back(i);
	}

	// get the skin context data
	ISkinContextData* i_skin_context_data = i_skin->GetContextInterface(i_node);
	if(i_skin_context_data == NULL) { G3DAssert("The skin(%s) has not the weight.",skin.name.c_str()); return; }

	// gets the initial matrix of the skinned object 
	Matrix3 initial_object_transform;
	i_skin->GetSkinInitTM(i_node, initial_object_transform, true);

	// process the points
	int num_points = i_skin_context_data->GetNumPoints();
	for(int i = 0; i < num_points; i++)
	{
		MAX_CHECK(i < skin.points.size());

		VPIW viw;

		// get the initial point				
		viw.pos = initial_object_transform * skin.points[i];

		// process the weights		
		std::multimap< float, int > weights;

		// get the number of bones that control this vertex
		int num_bones = i_skin_context_data->GetNumAssignedBones(i);
		if(num_bones>0)
		{
			for (int j = 0; j < num_bones; j++)
			{
				Matrix3 transform;

				// get the assigned bone of the point 
				INode* i_bone_node = i_skin->GetBone(i_skin_context_data->GetAssignedBone(i, j));
				MAX_CHECK(i_bone_node != NULL);

				// get the weight of the bone
				float weight = i_skin_context_data->GetBoneWeight(i, j);

				// add the weight to the table
				weights.insert(std::make_pair(weight, AddBone(skin,i_bone_node)));
			}
		}
		else
		{
			// add the weight to the table
			weights.insert(std::make_pair(1.f, AddBone(skin,i_node)));
		}

		// recalculate the weights
		float weight0 = 0.f, weight1 = 0.f, weight2 = 0.f;
		int index0 = 0, index1 = 0, index2 = 0;
		std::multimap< float, int >::iterator it = weights.end();
		it--;
		weight0 = it->first;
		index0 = it->second;
		if(it != weights.begin())
		{
			it--;
			weight1 = it->first;
			index1 = it->second;
			if(it != weights.begin())
			{
				it--;
				weight2 = it->first;
				index2 = it->second;
			}
		}		
		float sum_weights = weight0 + weight1 + weight2;

		// store the skin weights	
		viw.weight[0]	= weight0/sum_weights;
		viw.index[0]	= index0;
		viw.weight[1]	= weight1/sum_weights;
		viw.index[1]	= index1;
		viw.weight[2]	= weight2/sum_weights;
		viw.index[2]	= index2;
		skin.weights.push_back(viw);
	}

	// get the initial transforms
	skin.transforms.resize(skin.bones.size());
	for(int i = 0; i < skin.bones.size(); i++)
	{
		INode* node = skin.bones[i];
		Matrix3 mat;
		if (SKIN_INVALID_NODE_PTR == i_skin->GetBoneInitTM( node, mat ))
		{
			if (SKIN_INVALID_NODE_PTR == i_skin->GetSkinInitTM( node, mat ))
			{
				mat.IdentityMatrix();
			}
		}
		skin.transforms[i] = Inverse(mat);
	}

	// there is a 75 bone limit for each skinned object.
	if(skin.bones.size()>75)
	{
		G3DAssert("There are more %d bones in the skin(%s).",skin.bones.size(), i_node->GetName());
		return;
	}

	// reset the skin vertex position
	for(int i = 0; i < skin.vertexes.size(); i++)
	{
		VPTNIS& v0 = skin.vertexes[i];
		VPIW& v1 = skin.weights[v0.index];
		v0.pos = v1.pos;
	}

	// build the normal space
	BuildNormal(skin);

	// calculate the bounding box	
	skin.box.Init();
	for(int i = 0; i < skin.vertexes.size(); i++)
	{
		Point3 pt = node_matrix * skin.vertexes[i].pos;
		skin.box += pt;
	}

	// add the skin to the table
	mSkins.push_back(skin);
}

/*
====================
addVertex
====================
*/
int G3DSExport::AddVertex(SKIN& skin, const VPTNIS& v)
{
	for(int k = 0; k < skin.vertexes.size(); k++)
	{
		VPTNIS& mv = skin.vertexes[k];
		if( mv.pos == v.pos && 
			mv.uv == v.uv && 
			mv.normal == v.normal && 
			mv.index == v.index && 
			(mv.smoothing & v.smoothing) )
		{ 
			return k;
		}
	}
	skin.vertexes.push_back(v);
	return skin.vertexes.size() - 1;
}

/*
====================
AddBone
====================
*/
int G3DSExport::AddBone(SKIN& skin, const INode* node)
{
	std::vector<INode*>::iterator it = std::find(skin.bones.begin(), skin.bones.end(), node);
	if(it==skin.bones.end()) 
	{ 
		skin.bones.push_back((INode*)node); 
		return skin.bones.size()-1;
	}
	return it - skin.bones.begin();
}

/*
====================
BuildNormal
====================
*/
void G3DSExport::BuildNormal(SKIN& skin)
{
	// clear the normals currently in there
	for(std::vector<VPTNIS>::iterator it = skin.vertexes.begin(); it != skin.vertexes.end(); it++)
	{ it->normal = Point3::Origin; }

	// build the normals
	for(std::vector<TRIANGLE>::iterator it = skin.triangles.begin(); it != skin.triangles.end(); it++)	
	{
		TRIANGLE& tri = *it;
		Point3 v1 = skin.vertexes[tri.index1[1]].pos - skin.vertexes[tri.index1[0]].pos;
		Point3 v2 = skin.vertexes[tri.index1[2]].pos - skin.vertexes[tri.index1[0]].pos;
		Point3 normal = v1^v2;
		normal = Normalize(normal);

		v1 = skin.vertexes[tri.index1[1]].pos - skin.vertexes[tri.index1[0]].pos;
		v2 = skin.vertexes[tri.index1[2]].pos - skin.vertexes[tri.index1[0]].pos;
		AddNormal( skin, normal * NormalAngle(v1, v2), tri.index0[0], tri.smoothing, tri.index1[0] );

		v1 = skin.vertexes[tri.index1[2]].pos - skin.vertexes[tri.index1[1]].pos;
		v2 = skin.vertexes[tri.index1[0]].pos - skin.vertexes[tri.index1[1]].pos;
		AddNormal( skin, normal * NormalAngle(v1, v2), tri.index0[1], tri.smoothing, tri.index1[1] );

		v1 = skin.vertexes[tri.index1[0]].pos - skin.vertexes[tri.index1[2]].pos;
		v2 = skin.vertexes[tri.index1[1]].pos - skin.vertexes[tri.index1[2]].pos;
		AddNormal( skin, normal * NormalAngle(v1, v2), tri.index0[2], tri.smoothing, tri.index1[2] );
	}

	// normalize all of the vertexes
	for(std::vector<VPTNIS>::iterator it = skin.vertexes.begin(); it != skin.vertexes.end(); it++)
	{ it->normal = Normalize(it->normal); }
}

/*
====================
AddNormal
====================
*/
void G3DSExport::AddNormal(SKIN& skin, const Point3& normal, int index0, int smoothing, int index1)
{
	if(skin.vertex_index_map.find( index0 ) != skin.vertex_index_map.end())
	{
		std::vector<int>& vertice_index_set = skin.vertex_index_map[index0];
		for(std::vector<int>::iterator it = vertice_index_set.begin(); it != vertice_index_set.end(); ++it)
		{
			VPTNIS& v = skin.vertexes[*it];
			if(v.smoothing & smoothing || ((*it) == index1)) v.normal += normal;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Save
====================
*/
int G3DSExport::Save(const Str& path)
{	
	//////////////////////////////////////////////////////////////////////////
	// merge the skin
	//////////////////////////////////////////////////////////////////////////
	
	struct PRIMITIVE
	{
		Str name;
		Str texture;
		std::vector<INode*>bones;
		std::vector<Matrix3>transforms;
		std::vector<VPTNIW>vertexes;
		std::vector<unsigned int>indexes;
		Box3 box;
		int AddBone(const INode* node, const Matrix3& mat)
		{
			std::vector<INode*>::iterator it = std::find(bones.begin(), bones.end(), node);
			if(it==bones.end()) { bones.push_back((INode*)node); transforms.push_back(mat); return bones.size()-1; }
			return it - bones.begin();
		}
	};

	// get all of the textures
	std::vector<Str>textures;
	for(std::vector<SKIN>::iterator it = mSkins.begin(); it != mSkins.end(); ++it)
	{
		if(std::find(textures.begin(),textures.end(),it->texture) == textures.end())
		{
			textures.push_back(it->texture);
		}
	}

	// merge all of the skin to the primitive by the textures
	std::vector<PRIMITIVE>primitives;
	for(int i = 0; i < textures.size(); i++)
	{
		PRIMITIVE primitive;
		primitive.texture = textures[i];
		for(int j = 0; j < mSkins.size(); j++)
		{
			SKIN& skin = mSkins[j];
			if(skin.texture != primitive.texture) continue;
			if(primitive.name.size() == 0) primitive.name = skin.name;

			// get the start vertex
			int start_vertex = primitive.vertexes.size();

			// set all of the vertexes
			for(int k = 0; k < skin.vertexes.size(); k++ )
			{
				VPTNIS& v1 = skin.vertexes[k];
				VPTNIW v2;
				v2.position[0] = v1.pos.x;
				v2.position[1] = v1.pos.y;
				v2.position[2] = v1.pos.z;
				v2.texcoord[0] = v1.uv.x;
				v2.texcoord[1] = v1.uv.y;
				v2.normal[0] = v1.normal.x;
				v2.normal[1] = v1.normal.y;
				v2.normal[2] = v1.normal.z;
				v2.index[0] = primitive.AddBone(skin.bones[skin.weights[v1.index].index[2]], skin.transforms[skin.weights[v1.index].index[2]]);
				v2.index[1] = primitive.AddBone(skin.bones[skin.weights[v1.index].index[1]], skin.transforms[skin.weights[v1.index].index[1]]);
				v2.index[2] = primitive.AddBone(skin.bones[skin.weights[v1.index].index[0]], skin.transforms[skin.weights[v1.index].index[0]]);
				v2.weight[0] = skin.weights[v1.index].weight[2];
				v2.weight[1] = skin.weights[v1.index].weight[1];
				v2.weight[2] = skin.weights[v1.index].weight[0];
				primitive.vertexes.push_back(v2);
			}

			// set the index
			for(int k = 0; k < skin.triangles.size(); k++)
			{
				TRIANGLE& tri = skin.triangles[k];
				primitive.indexes.push_back(start_vertex + tri.index1[0]);
				primitive.indexes.push_back(start_vertex + tri.index1[1]);
				primitive.indexes.push_back(start_vertex + tri.index1[2]);
			}

			// set the box
			primitive.box += skin.box;
		}

		// there is a 75 bone limit for each skinned object.
		if(primitive.bones.size()>75) { G3DAssert("There are more %d bones in the primitive and the texture is %s.",primitive.bones.size(), textures[i].c_str()); return 1; }

		// add the primitive to the table
		primitives.push_back(primitive);
	}

	//////////////////////////////////////////////////////////////////////////
	// save the skin
	//////////////////////////////////////////////////////////////////////////
	
	// open the skin file to save it
	FILE *output = fopen(path.c_str(), "wb");
	if(output==NULL) { G3DAssert("Can`t open the file : %s.", path.c_str()); return 1; }

	// begin to write the skin	
	fprintf( output, "<skeletal_mesh>\n" );

	// write all of the primitives
	for(int i = 0; i < primitives.size(); i++)
	{
		PRIMITIVE& primitive = primitives[i];

		// begin to write the primitive
		fprintf(output, "<primitive type=\"PT_TRIANGLES\">\n");

		// wirte all of the vertexes
		fprintf(output, "<vertex format=\"VT_3F VT_2F VT_3F VT_3F VT_3F\">\n");
		for(int j = 0; j < primitive.vertexes.size(); j++)
		{
			VPTNIW& v =  primitive.vertexes[j];
			fprintf(output,	"%f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
				v.position[0], v.position[1], v.position[2], 
				v.texcoord[0], v.texcoord[1],
				v.normal[0], v.normal[1], v.normal[2], 
				v.index[0], v.index[1], v.index[2],
				v.weight[0], v.weight[1], v.weight[2]
				);
		}
		fprintf(output, "</vertex>\n");

		// wirte all of the indexes
		fprintf(output, "<index stride=\"%d\">\n", primitive.vertexes.size()<=0xFFFF ? sizeof(unsigned short) : sizeof(unsigned int));
		for(int j = 0; j < primitive.indexes.size(); j++ ) fprintf(output, "%u ", primitive.indexes[j]);
		fprintf( output, "</index>\n" );

		// wirte the bones
		for(int j = 0; j < primitive.bones.size(); j++)
		{
			Str node_name = primitive.bones[j]->GetName();			
			Matrix3 m = primitive.transforms[j];
			Point3 r0 = m.GetRow(0); Point3 r1 = m.GetRow(1); Point3 r2 = m.GetRow(2); Point3 r3 = m.GetRow(3);
			fprintf(output, "<bone name=\"%s\">\n", node_name.c_str());
			fprintf(output, "%f %f %f %f\n", r0.x, r0.y, r0.z, 0.0);
			fprintf(output, "%f %f %f %f\n", r1.x, r1.y, r1.z, 0.0);
			fprintf(output, "%f %f %f %f\n", r2.x, r2.y, r2.z, 0.0);
			fprintf(output, "%f %f %f %f\n", r3.x, r3.y, r3.z, 1.0);
			fprintf(output, "</bone>\n" );
		}

		// write the shader info
		fprintf(output, "<shader name=\"shader/skeletal.xml\">\n");
		fprintf(output, "<constant name=\"gVP\" type = \"CT_MATRIX\">\n 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 \n </constant>\n");
		fprintf(output, "<constant name=\"gWorld\" type = \"CT_MATRIX\">\n 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 \n </constant>\n");
		fprintf(output, "<constant name=\"gBones\" type = \"CT_VECTOR_ARRAY\">\n 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 \n </constant>\n");
		fprintf(output, "<constant name=\"gBaseTex\" type = \"CT_TEXTURE\">\n %s\n </constant>\n", primitive.texture.c_str());
		fprintf(output, "</shader>\n" );

		// write the primitive box
		fprintf( output, "<bounding_box>\n %f %f %f %f %f %f\n </bounding_box>\n", 
			primitive.box.Min().x, primitive.box.Min().y, primitive.box.Min().z, primitive.box.Max().x, primitive.box.Max().y, primitive.box.Max().z );

		// end to write the primitive 
		fprintf(output, "</primitive>\n");
	}

	// end to wirte the skin
	fprintf(output, "</skeletal_mesh>\n");

	// close file
	fclose(output);	

	return 1;
}

//////////////////////////////////////////////////////////////////////////