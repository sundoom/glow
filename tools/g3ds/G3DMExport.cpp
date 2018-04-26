//////////////////////////////////////////////////////////////////////////
// G3DMExport.cpp
//////////////////////////////////////////////////////////////////////////

#include "Local.h"

//////////////////////////////////////////////////////////////////////////

G3DMExport::G3DMExport()
{

}

G3DMExport::~G3DMExport() 
{

}

//////////////////////////////////////////////////////////////////////////

/*
====================
ExtCount
====================
*/
int G3DMExport::ExtCount()
{
	return 1;
}

/*
====================
Ext
====================
*/
const TCHAR* G3DMExport::Ext(int n)
{ 
	switch(n) 
	{
	case 0:
		return _T("G3DM");
	}
	return _T("");
}

/*
====================
LongDesc
====================
*/
const TCHAR* G3DMExport::LongDesc()
{ 
	return _T("Glow 3D Static Mesh Export for 3D Studio Max");	
}

/*
====================
ShortDesc
====================
*/
const TCHAR* G3DMExport::ShortDesc()
{ 
	return _T("Glow 3D Static Mesh Export");
}

/*
====================
AuthorName
====================
*/
const TCHAR* G3DMExport::AuthorName()
{ 
	return _T("SunJun");
}

/*
====================
CopyrightMessage
====================
*/
const TCHAR* G3DMExport::CopyrightMessage()
{ 
	return _T("Copyright (c) 2009 - 2010 , Glow");
}

/*
====================
OtherMessage1
====================
*/
const TCHAR* G3DMExport::OtherMessage1()
{ 
	return _T("");
}

/*
====================
OtherMessage2
====================
*/
const TCHAR* G3DMExport::OtherMessage2()
{
	return _T("");
}

/*
====================
Version
====================
*/
unsigned int G3DMExport::Version()
{
	return 101;
}

/*
====================
ShowAbout
====================
*/
void G3DMExport::ShowAbout(HWND hWnd)
{
	
}

/*
====================
ShowAbout
====================
*/
BOOL G3DMExport::SupportsOptions( int ext, DWORD options )
{
	return options == SCENE_EXPORT_SELECTED;
}

/*
====================
DoExport
====================
*/
int G3DMExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	// clear the old data
	mMeshes.clear();
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
	if(Save(path) == 0) { G3DAssert("Fail to save the G3DM file : %s!", path.c_str() ); return 1; }

	// clear the old data
	mMeshes.clear();
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
void G3DMExport::ExportVisible( INode* i_node )
{
	if(!i_node->IsHidden() && i_node->Renderable() )
	{
		// gather the mesh data
		GatherMesh(i_node);
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
void G3DMExport::ExportSelected( INode* i_node )
{
	if(i_node->Selected() && i_node->Renderable())
	{
		// gather the mesh data
		GatherMesh(i_node);
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
GatherMesh
====================
*/
void G3DMExport::GatherMesh(INode* i_node)
{
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
	if(i_mesh==NULL||i_mesh->getNumFaces()==0||i_mesh->getNumVerts()==0) return;

	MESH mesh;

	// get the mesh name
	mesh.name = i_node->GetName();

	// get the material
	mesh.texture = "textures/default.tga";
	Mtl* mtl = i_node->GetMtl();
	if(mtl && (mtl->ClassID()==Class_ID(DMTL_CLASS_ID, 0)) && ((StdMat*)mtl)->MapEnabled(ID_DI)) 
	{
		Texmap *texmap = mtl->GetSubTexmap(ID_DI);
		if(texmap && texmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
		{
			mesh.texture = UnifySlashes(((BitmapTex *)texmap)->GetMapName());
			if( !strstr( mesh.texture.c_str(), mPath.c_str() ) )
			{
				G3DAssert("The material(%s) is error : the texture path(%s) is illegal!",mtl->GetName(), mesh.texture.c_str());
				return;
			}
			else
			{
				mesh.texture = strstr(mesh.texture.c_str(),mPath.c_str()) + strlen(mPath.c_str());
			}
		}
	}

	// if it has uvs
	int map_count = i_mesh->getNumMaps();
	bool has_uvs = i_mesh->getNumTVerts() && i_mesh->tvFace;
	if(!(has_uvs&&map_count)) return;

	// get the transform
	Matrix3 transform = i_node->GetObjectTM(mTime);

	// get the points
	mesh.points.assign(i_mesh->verts, i_mesh->verts+i_mesh->getNumVerts());

	// get the triangles
	for(int i = 0; i < i_mesh->getNumFaces(); i++)
	{
		Face& face = i_mesh->faces[i];

		TRIANGLE tri;		
		tri.smoothing = face.smGroup;
		for(int j = 0; j < 3; j++)
		{
			VTNIS v;
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
			tri.index1[j] = AddVertex(mesh, v);
		}

		// add the triangle to the table
		mesh.triangles.push_back(tri);
	}

	// build the index map
	for( int i = 0; i < mesh.vertexes.size(); i++ )
	{
		mesh.vertex_index_map[mesh.vertexes[i].index].push_back(i);
	}

	// build the normal space
	BuildNormal(mesh);

	// calculate the bounding box	
	mesh.box.Init();
	for(int i = 0; i < mesh.vertexes.size(); i++)
	{
		mesh.box += mesh.vertexes[i].pos;
	}

	// add the mesh to the table
	mMeshes.push_back(mesh);
}

/*
====================
AddVertex
====================
*/
int G3DMExport::AddVertex( MESH& mesh, const VTNIS& v )
{
	for(int k = 0; k < mesh.vertexes.size(); k++)
	{
		VTNIS& mv = mesh.vertexes[k];
		if( mv.pos == v.pos && 
			mv.uv == v.uv && 
			mv.normal == v.normal && 
			mv.index == v.index && 
			(mv.smoothing & v.smoothing) )
		{ 
			return k;
		}
	}

	mesh.vertexes.push_back(v);
	return mesh.vertexes.size() - 1;
}


/*
====================
BuildNormal
====================
*/
void G3DMExport::BuildNormal( MESH& mesh )
{
	// clear the normals currently in there
	for(std::vector<VTNIS>::iterator it = mesh.vertexes.begin(); it != mesh.vertexes.end(); it++)
	{ it->normal = Point3::Origin; }

	// build the normals
	for(std::vector<TRIANGLE>::iterator it = mesh.triangles.begin(); it != mesh.triangles.end(); it++)	
	{
		TRIANGLE& tri = *it;
		Point3 v1 = mesh.vertexes[tri.index1[1]].pos - mesh.vertexes[tri.index1[0]].pos;
		Point3 v2 = mesh.vertexes[tri.index1[2]].pos - mesh.vertexes[tri.index1[0]].pos;
		Point3 normal = v1^v2;
		normal = Normalize(normal);

		v1 = mesh.vertexes[tri.index1[1]].pos - mesh.vertexes[tri.index1[0]].pos;
		v2 = mesh.vertexes[tri.index1[2]].pos - mesh.vertexes[tri.index1[0]].pos;
		AddNormal( mesh, normal * NormalAngle(v1, v2), tri.index0[0], tri.smoothing, tri.index1[0] );

		v1 = mesh.vertexes[tri.index1[2]].pos - mesh.vertexes[tri.index1[1]].pos;
		v2 = mesh.vertexes[tri.index1[0]].pos - mesh.vertexes[tri.index1[1]].pos;
		AddNormal( mesh, normal * NormalAngle(v1, v2), tri.index0[1], tri.smoothing, tri.index1[1] );

		v1 = mesh.vertexes[tri.index1[0]].pos - mesh.vertexes[tri.index1[2]].pos;
		v2 = mesh.vertexes[tri.index1[1]].pos - mesh.vertexes[tri.index1[2]].pos;
		AddNormal( mesh, normal * NormalAngle(v1, v2), tri.index0[2], tri.smoothing, tri.index1[2] );
	}

	// normalize all of the vertexes
	for(std::vector<VTNIS>::iterator it = mesh.vertexes.begin(); it != mesh.vertexes.end(); it++)
	{ it->normal = Normalize(it->normal); }
}

/*
====================
AddNormal
====================
*/
void G3DMExport::AddNormal( MESH& mesh, const Point3& normal, int index0, int smoothing, int index1 )
{
	if( mesh.vertex_index_map.find( index0 ) != mesh.vertex_index_map.end() )
	{
		std::vector<int>& vertice_index_set = mesh.vertex_index_map[index0];
		for( std::vector<int>::iterator it = vertice_index_set.begin(); it != vertice_index_set.end(); ++it )
		{
			VTNIS& v = mesh.vertexes[*it];
			if( v.smoothing & smoothing || ((*it) == index1) )
			{
				v.normal += normal;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

/*
====================
Save
====================
*/
int G3DMExport::Save(const Str& path)
{	
	//////////////////////////////////////////////////////////////////////////
	// merge the static mesh
	//////////////////////////////////////////////////////////////////////////

	struct PRIMITIVE
	{
		Str texture;
		std::vector<VTN>vertexes;
		std::vector<unsigned int>indexes;
		Box3 box;
	};

	// get all of the textures
	std::vector<Str>textures;
	for(std::vector<MESH>::iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
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
		for(int j = 0; j < mMeshes.size(); j++)
		{
			MESH& mesh = mMeshes[j];
			if(mesh.texture != primitive.texture) continue;

			// get the start vertex
			int start_vertex = primitive.vertexes.size();

			// set all of the vertexes
			for(int k = 0; k < mesh.vertexes.size(); k++ )
			{
				VTNIS& v1 = mesh.vertexes[k];
				VTN v2;
				v2.position[0] = v1.pos.x;
				v2.position[1] = v1.pos.y;
				v2.position[2] = v1.pos.z;
				v2.texcoord[0] = v1.uv.x;
				v2.texcoord[1] = v1.uv.y;
				v2.normal[0] = v1.normal.x;
				v2.normal[1] = v1.normal.y;
				v2.normal[2] = v1.normal.z;
				primitive.vertexes.push_back(v2);
			}

			// set the index
			for(int k = 0; k < mesh.triangles.size(); k++)
			{
				TRIANGLE& tri = mesh.triangles[k];
				primitive.indexes.push_back(start_vertex + tri.index1[0]);
				primitive.indexes.push_back(start_vertex + tri.index1[1]);
				primitive.indexes.push_back(start_vertex + tri.index1[2]);
			}

			// set the box
			primitive.box += mesh.box;
		}			

		// add the primitive to the table
		primitives.push_back(primitive);
	}

	//////////////////////////////////////////////////////////////////////////
	// save the static mesh
	//////////////////////////////////////////////////////////////////////////

	// open the mesh file to save it
	FILE *output = fopen(path.c_str(), "wb");
	if(output==NULL) { G3DAssert("Can`t open the file : %s.", path.c_str()); return 1; }

	// begin to write the static mesh	
	fprintf( output, "<static_mesh>\n" );

	// wirte all of the primitives of the mesh
	for(int i = 0; i < primitives.size(); i++)
	{
		PRIMITIVE& primitive = primitives[i];

		// begin to write the primitive
		fprintf(output, "<primitive type=\"PT_TRIANGLES\">\n");

		// wirte all of the vertexes
		fprintf(output, "<vertex format=\"VT_3F VT_2F VT_3F\">\n");
		for(int j = 0; j < primitive.vertexes.size(); j++)
		{
			VTN& v =  primitive.vertexes[j];
			fprintf(output, "%f %f %f %f %f %f %f %f\n", v.position[0], v.position[1], v.position[2], v.texcoord[0], v.texcoord[1],v.normal[0], v.normal[1], v.normal[2]);
		}
		fprintf(output, "</vertex>\n");

		// wirte all of the indexes
		fprintf(output, "<index stride=\"%d\">\n", primitive.vertexes.size()<=0xFFFF ? sizeof(unsigned short) : sizeof(unsigned int));
		for(int j = 0; j < primitive.indexes.size(); j++ ) fprintf(output, "%u ", primitive.indexes[j]);
		fprintf(output, "</index>\n");

		// write the shader info
		fprintf(output, "<shader name=\"shader/mesh.xml\">\n");
		fprintf(output, "<constant name=\"gWVP\" type = \"CT_MATRIX\">\n 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 \n </constant>\n");
		fprintf(output, "<constant name=\"gBaseTex\" type = \"CT_TEXTURE\">\n %s\n </constant>\n", primitive.texture.c_str());
		fprintf(output, "</shader>\n" );

		// write the primitive box
		fprintf( output, "<bounding_box>\n %f %f %f %f %f %f\n </bounding_box>\n", 
			primitive.box.Min().x, primitive.box.Min().y, primitive.box.Min().z, primitive.box.Max().x, primitive.box.Max().y, primitive.box.Max().z );

		// end to write the primitive 
		fprintf(output, "</primitive>\n");
	}

	// end to wirte the static mesh
	fprintf(output, "</static_mesh>\n");

	// close file
	fclose(output);	

	return 1;
}

//////////////////////////////////////////////////////////////////////////