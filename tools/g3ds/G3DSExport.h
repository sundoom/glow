//////////////////////////////////////////////////////////////////////////
// G3DSExport.h
//////////////////////////////////////////////////////////////////////////

#ifndef __G3DSExport__
#define __G3DSExport__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

class G3DSExport : public SceneExport
{
public:
	// constructor
	G3DSExport();
	
	// destructor
	virtual ~G3DSExport();
	
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

	struct VPTNIW
	{
		float position[3];
		float texcoord[2];
		float normal[3];
		float index[3];
		float weight[3];
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

	struct TRIANGLE
	{
		int	index0[3];	// the original index
		int	index1[3];	// the optimized index
		int	smoothing;
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

	#pragma pack(pop)

private:
	void ExportVisible(INode* i_node);
	void ExportSelected(INode* i_node);

	void GatherSkin(INode* i_node);
	int AddVertex(SKIN& skin, const VPTNIS& v);
	int AddBone(SKIN& skin, const INode* node);
	void BuildNormal(SKIN& skin);
	void AddNormal(SKIN& skin, const Point3& normal, int index0, int smoothing, int index1);

	void GatherShape(INode* i_node);

	int Save(const Str& path);

private:
	unsigned int mTime;
	Str mPath;	
	std::vector<SKIN>mSkins;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __G3DSExport__

//////////////////////////////////////////////////////////////////////////
