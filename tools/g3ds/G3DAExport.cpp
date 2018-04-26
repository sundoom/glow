//////////////////////////////////////////////////////////////////////////
// G3DAExport.cpp
//////////////////////////////////////////////////////////////////////////

#include "Local.h"

//////////////////////////////////////////////////////////////////////////

G3DAExport::G3DAExport()
{
	mRoot = NULL;
}

G3DAExport::~G3DAExport() 
{
	if(mRoot){delete mRoot, mRoot = NULL;}
}

//////////////////////////////////////////////////////////////////////////

/*
====================
ExtCount
====================
*/
int G3DAExport::ExtCount()
{
	return 1;
}

/*
====================
Ext
====================
*/
const TCHAR* G3DAExport::Ext(int n)
{ 
	switch(n) 
	{
	case 0:
		return _T("G3DA");
	}
	return _T("");
}

/*
====================
LongDesc
====================
*/
const TCHAR* G3DAExport::LongDesc()
{ 
	return _T("Glow 3D Animation Export for 3D Studio Max");	
}

/*
====================
ShortDesc
====================
*/
const TCHAR* G3DAExport::ShortDesc()
{ 
	return _T("Glow 3D Animation Export");
}

/*
====================
AuthorName
====================
*/
const TCHAR* G3DAExport::AuthorName()
{ 
	return _T("SunJun");
}

/*
====================
CopyrightMessage
====================
*/
const TCHAR* G3DAExport::CopyrightMessage()
{ 
	return _T("Copyright (c) 2009 - 2010 , Glow");
}

/*
====================
OtherMessage1
====================
*/
const TCHAR* G3DAExport::OtherMessage1()
{ 
	return _T("");
}

/*
====================
OtherMessage2
====================
*/
const TCHAR* G3DAExport::OtherMessage2()
{
	return _T("");
}

/*
====================
Version
====================
*/
unsigned int G3DAExport::Version()
{
	return 101;
}

/*
====================
ShowAbout
====================
*/
void G3DAExport::ShowAbout(HWND hWnd)
{
	
}

/*
====================
DoExport
====================
*/
int G3DAExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	// clear the old data
	if(mRoot){delete mRoot, mRoot = NULL;}
	mNodeCount = 0;
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

	// get the animation range
	Interval inter = i->GetAnimRange();
	mTime = i->GetTime();
	mStart = inter.Start();
	mEnd = inter.End();

	// recursive export all of the node
	exportNode(i->GetRootNode());

	// export all of the skin
	for(int k = 0; k < mSkins.size(); k++) exportSkin(mSkins[k]);
	
	// recursive export all of the animation
	exportAnimation(mRoot);

	// check all of the nodes
	std::set<Str> names, duplicates;
	check(mRoot, names, duplicates);
	for(std::set<Str>::iterator it = duplicates.begin(); it != duplicates.end(); ++it)
	{
		G3DAssert( "Duplicate node name : %s", (*it).c_str() );
		return 1;
	}

#if 0
	// write all of the data	
	FILE *output = fopen(path.c_str(), "wb");
	if(output==NULL) { G3DAssert("Fail to open the file : %s", path.c_str()); return 1; }

	// write count of the frame
	unsigned int frame_count = (mEnd-mStart)/GetTicksPerFrame()+1;
	fwrite( &frame_count, sizeof(unsigned int), 1, output );

	// write number of the node
	fwrite( &mNodeCount, sizeof(unsigned int), 1, output );

	// write count of the action
	unsigned int action_count = 0;
	fwrite( &action_count, sizeof(unsigned int), 1, output );

	// write all of the node animation
	wirteAnimation(output,mRoot);

	// close file
	fclose(output);
#else
	// write all of the data	
	FILE *output = fopen(path.c_str(), "wb");
	if(output==NULL) { G3DAssert("Fail to open the file : %s", path.c_str()); return 1; }

	// get the count of the frame
	unsigned int frame_count = (mEnd-mStart)/GetTicksPerFrame()+1;

	// begin to write the animation
	fprintf(output, "<animation>\n");

	// begin to write the frame
	fprintf(output, "<frame count=\"%d\">\n", frame_count);

	// write all of the node frame
	wirteFrame(output,mRoot);

	// end to wirte the frame
	fprintf(output, "</frame>\n");

	// end to wirte the animation
	fprintf(output, "</animation>\n");

	// close file
	fclose(output);
#endif
	// clear all of the data
	mSkins.clear();
	if(mRoot){delete mRoot, mRoot = NULL;}

	return 1;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
getModifier
====================
*/
static Modifier* getModifier( INode* i_node, Class_ID cid )
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
exportNode
====================
*/
void G3DAExport::exportNode( INode* i_node, NODE* parent )
{
	// if it is hidden?
	bool include = false;
	if(!i_node->IsHidden()) include = true;

	// get the result of the pipeline at the current time 	
	Object *obj = i_node->EvalWorldState(mTime).obj;
	if(obj)
	{
		if(obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0)) include = false;
		if(obj->ClassID() == Class_ID(0x74f93b07, 0x1eb34300)) include = false;
		if(obj->ClassID() == Class_ID(BOXOBJ_CLASS_ID, 0)) include = false;
		if(obj->ClassID() == Class_ID(SPHERE_CLASS_ID, 0)) include = false;
		if(obj->ClassID() == Class_ID(CYLINDER_CLASS_ID, 0)) include = false;
		if(obj->ClassID() == Class_ID(CONE_CLASS_ID, 0)) include = false;
		if(obj->SuperClassID() == CAMERA_CLASS_ID) include = false;
		if(obj->SuperClassID() == LIGHT_CLASS_ID) include = false;
		if(obj->SuperClassID() == SHAPE_CLASS_ID) include = false;
	}

	// if this node is renderable?
	if(include)
	{
		if(getModifier(i_node,Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)) != NULL)
		{
			include = false;
		}

		if(getModifier(i_node,SKIN_CLASSID) != NULL)
		{
			mSkins.push_back(i_node);
			include = false;
		}
	}

	// the new node
	NODE* this_node = new NODE;
	MAX_CHECK(this_node);
	if(parent==NULL)
	{
		mRoot = this_node;		
	}
	else
	{
		parent->children.push_back(this_node);
		this_node->parent = parent;
	}
	this_node->i_node = i_node;
	this_node->include = include;
	this_node->name = i_node->GetName();
	if(this_node->include)
	{
		NODE* parent = this_node->parent;
		while(parent)
		{
			parent->include = true;
			parent = parent->parent;
		}
	}	

	// process the child node
	for(int i = 0; i < i_node->NumberOfChildren(); i++)
	{
		exportNode(i_node->GetChildNode(i),this_node);
	}
}

/*
====================
exportSkin
====================
*/
bool G3DAExport::exportSkin(INode* i_node)
{
	// get the modifier
	Modifier *modifier = getModifier(i_node,SKIN_CLASSID);
	if(modifier==NULL)return false;

	// get the skin interface
	ISkin* i_skin = (ISkin*)modifier->GetInterface(I_SKIN);
	if(i_skin == NULL) return false;

	// get the skin context data
	ISkinContextData* i_skin_context_data = i_skin->GetContextInterface(i_node);
	if(i_skin_context_data == NULL) return false;

	// process the bones
	std::vector<INode*>bones;
	int num_points = i_skin_context_data->GetNumPoints();
	for (int i = 0; i < num_points; i++)
	{
		// get the number of bones that control this vertex
		int num_bones = i_skin_context_data->GetNumAssignedBones(i);
		for (int j = 0; j < num_bones; j++)
		{
			// get the assigned bone of the point 
			INode* i_bone_node = i_skin->GetBone(i_skin_context_data->GetAssignedBone(i, j));
			MAX_CHECK(i_bone_node != NULL);

			// add the bone node to the table
			std::vector<INode*>::iterator it = std::find(bones.begin(), bones.end(), i_bone_node);
			if(it==bones.end())bones.push_back(i_bone_node);
		}
	}

	// include all of the bone
	for (int i = 0; i < bones.size(); i++)
	{
		NODE *x = FindNode(mRoot, bones[i]) ;
		x->include = true;
		if(x->include)
		{
			NODE* parent = x->parent;
			while(parent) 
			{ 
				parent->include = true;
				parent = parent->parent;
			}
		}
	}

	return true;
}

/*
====================
exportAnimation
====================
*/
void G3DAExport::exportAnimation(NODE* node)
{
	// get the animation 
	if(node->include)
	{
		int first_frame = mStart/GetTicksPerFrame();
		int last_frame = mEnd/GetTicksPerFrame();
		for(int i = first_frame; i <= last_frame; i++)
		{
			// does this node have a max node?
			TimeValue t = i*GetTicksPerFrame();
			Matrix3 m(true);
			if(node->parent)
			{
				m = GetTransform(node, t) * Inverse(GetTransform(node->parent, t));
			}
			else
			{
				m = GetTransform(node, t);
			} 

			// add the transform to the sequence
			node->sequence.push_back(m);
		}
		mNodeCount++;
	}

	// process the child node
	for(int i = 0; i < node->children.size(); i++)
	{
		exportAnimation(node->children[i]);
	}
}

//////////////////////////////////////////////////////////////////////////

/*
====================
GetTransform
====================
*/
Matrix3 G3DAExport::GetTransform( NODE* node, TimeValue t )
{
	// Is there a max node attached to this node, if there is, return its world transform
	if(node->i_node) return node->i_node->GetNodeTM(t);

	// do we have a parent, if so return the parents worldtransform
	if(node->parent) return GetTransform(node->parent, t);

	// return the stored transform
	return Matrix3(true);
}

/*
====================
FindNode
====================
*/
G3DAExport::NODE* G3DAExport::FindNode( NODE* node, INode* i_node )
{
	if(node->i_node == i_node) return node;
	NODE *ret = NULL;
	for(int i = 0; i < node->children.size() && ret==NULL; i++)
	{
		ret = FindNode(node->children[i], i_node);
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/*
====================
wirteFrame
====================
*/
void G3DAExport::wirteFrame( FILE* file, NODE* node )
{
#if 0
	if(node->include)
	{
		// write the node name
		unsigned int len = node->name.size();
		fwrite(&len, sizeof(unsigned int), 1, file);
		fwrite(node->name.c_str(), sizeof(char), len, file);

		// write the parent name
		if(node->parent)
		{
			len = node->parent->name.size();
			fwrite(&len, sizeof(unsigned int), 1, file);
			fwrite(node->parent->name.c_str(), sizeof(char), len, file);
		}
		else
		{
			len = 0;
			fwrite(&len, sizeof(unsigned int), 1, file);
		}

		// write the animation sequence
		for(int i = 0; i < node->sequence.size(); i++)
		{
			Matrix3& transform = node->sequence[i];
			Point3 r0 = transform.GetRow(0);
			Point3 r1 = transform.GetRow(1);
			Point3 r2 = transform.GetRow(2);
			Point3 r3 = transform.GetRow(3);
			float m[4][4];
			m[0][0] = r0.x; m[0][1] = r0.y; m[0][2] = r0.z; m[0][3] = 0.0;
			m[1][0] = r1.x; m[1][1] = r1.y; m[1][2] = r1.z; m[1][3] = 0.0;
			m[2][0] = r2.x; m[2][1] = r2.y; m[2][2] = r2.z; m[2][3] = 0.0;
			m[3][0] = r3.x; m[3][1] = r3.y; m[3][2] = r3.z; m[3][3] = 1.0;
			fwrite( m, sizeof(m), 1, file );
		}
	}

	for(int i = 0; i < node->children.size(); i++)
	{
		wirteFrame(file, node->children[i]);
	}
#else
	if(node->include)
	{
		// get the name
		Str node_name = node->name;
		Str parent_name = "NULL";
		if(node->parent) parent_name = node->parent->name;

		// begin to write the node
		fprintf( file, "<bone name=\"%s\" parent=\"%s\">\n", node_name.c_str(), parent_name.c_str() );

		// write the animation sequence
		for(int i = 0; i < node->sequence.size(); i++)
		{
			Matrix3& transform = node->sequence[i];
			Point3 r0 = transform.GetRow(0), r1 = transform.GetRow(1), r2 = transform.GetRow(2), r3 = transform.GetRow(3);
			fprintf( file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", r0.x, r0.y, r0.z, 0.0, r1.x, r1.y, r1.z, 0.0, r2.x, r2.y, r2.z, 0.0, r3.x, r3.y, r3.z, 1.0 );
		}
		
		// end to write the node
		fprintf( file, "</bone>\n" );
	}

	for(int i = 0; i < node->children.size(); i++)
	{
		wirteFrame( file, node->children[i] );
	}
#endif
}

//////////////////////////////////////////////////////////////////////////

/*
====================
check
====================
*/
void G3DAExport::check(NODE* node, std::set<Str>& names, std::set<Str>& duplicates)
{
	MAX_CHECK(node);
	Str name = node->name;
	if (std::find( names.begin(), names.end(), name ) == names.end())
	{
		names.insert(name);
	}
	else
	{
		duplicates.insert(name);
	}

	for(int i = 0; i < node->children.size(); i++)
	{
		check(node->children[i], names, duplicates);
	}
}

//////////////////////////////////////////////////////////////////////////