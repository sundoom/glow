//////////////////////////////////////////////////////////////////////////
// G3DMExport.h
//////////////////////////////////////////////////////////////////////////

#ifndef __G3DMExport__
#define __G3DMExport__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

class G3DMExport : public SceneExport
{
public:
	// constructor
	G3DMExport();
	
	// destructor
	virtual ~G3DMExport();
	
	// returns the number of file name extensions supported by the plug-in.
	int	ExtCount();

	// return the 'i-th' file name extension.
	const TCHAR* Ext(int n);

	// return a long ASCII description of the file type being exported.
	const TCHAR* LongDesc();

	// return a short ASCII description of the file type being exported.
	const TCHAR* ShortDesc();

	// return the ASCII Author name.
	const TCHAR* AuthorName();

	// return the ASCII Copyright message for the plug-in.
	const TCHAR* CopyrightMessage();

	// return the first message string that is displayed.
	const TCHAR* OtherMessage1();

	// return the second message string that is displayed.
	const TCHAR* OtherMessage2();

	// return the version number of the export plug-in.
	unsigned int Version();

	// this method is called to have the plug-in display its "About..." box.
	void ShowAbout(HWND hWnd);

	// This method is called by 3ds Max to determine if one or more export options are supported by a plug-in for a given extension.
	BOOL SupportsOptions( int ext, DWORD options );

	// this method is called for the plug-in to perform its file export.
	int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE,DWORD options=0);

private:
	#pragma pack(push,1)
	
	struct VTN
	{
		float position[3];
		float texcoord[2];
		float normal[3];
	};
	
	struct VTNIS
	{
		Point3	pos;
		Point2	uv;
		Point3	normal;
		int		index;
		int		smoothing;
	};

	struct TRIANGLE
	{
		int	index0[3];	// the original index
		int	index1[3];	// the optimized index
		int	smoothing;
	};
	
	struct MESH
	{
		Str name;
		Str texture;
		std::vector<Point3>points;
		std::vector<VTNIS>vertexes;
		std::map<int,std::vector<int>>vertex_index_map;
		std::vector<TRIANGLE>triangles;		
		Box3 box;
	};

	//////////////////////////////////////////////////////////////////////////

	struct VPTNIW
	{
		float position[3];
		float texcoord[2];
		float normal[3];
		unsigned char index[4];
		unsigned char weight[4];
	};

	struct VPTNIS
	{
		Point3	pos;
		Point2	uv;
		Point3	normal;		
		int		index;
		int		smoothing;
	};

	struct VPIW
	{
		Point3	pos;
		int		index[3];
		float	weight[3];
	};	

	struct SKIN
	{
		Str name;
		Str texture;
		std::vector<INode*>bones;
		std::vector<Matrix3>transforms;
		std::vector<Point3>points;
		std::vector<VPTNIS>vertexes;
		std::map<int,std::vector<int>>vertex_index_map;
		std::vector<TRIANGLE>triangles;
		std::vector<VPIW>weights;
		Box3 box;
	};

	struct SHAPE
	{
		Str type;
		Str name;
		Str parent;
		Matrix3 pose;
		float value[3];
	};

	#pragma pack(pop)

private:
	void ExportVisible(INode* i_node);
	void ExportSelected(INode* i_node);	
	
	void GatherMesh(INode* i_node);
	int AddVertex(MESH& mesh, const VTNIS& v);
	void BuildNormal(MESH& mesh);	
	void AddNormal(MESH& mesh, const Point3& normal, int index0, int smoothing, int index1);	

	void GatherShape(INode* i_node);

	int Save(const Str& path);

private:
	unsigned int mTime;
	Str mPath;
	std::vector<MESH>mMeshes;
	std::vector<SKIN>mSkins;
	std::vector<SHAPE>mShapes;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __G3DMExport__

//////////////////////////////////////////////////////////////////////////
