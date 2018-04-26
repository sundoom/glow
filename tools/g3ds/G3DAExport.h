//////////////////////////////////////////////////////////////////////////
// G3DAExport.h
//////////////////////////////////////////////////////////////////////////

#ifndef __G3DAExport__
#define __G3DAExport__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

class G3DAExport : public SceneExport
{
public:
	// constructor
	G3DAExport();
	
	// destructor
	virtual ~G3DAExport();
	
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

	// this method is called for the plug-in to perform its file export.
	int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE,DWORD options=0);

private:
#pragma pack(push,1)
	struct NODE
	{
		Str		name;
		bool	include;
		INode	*i_node;		
		NODE	*parent;
		std::vector<Matrix3>sequence;
		std::vector<NODE*>children;
		NODE() {i_node = NULL; parent = NULL;}
		~NODE() {for( std::vector<NODE*>::iterator it = children.begin(); it != children.end(); ++it ) delete (*it);}
	};
#pragma pack(pop)

private:
	void exportNode(INode* i_node, NODE* parent = NULL);
	bool exportSkin(INode* i_node);
	void exportAnimation(NODE* node);

	static Matrix3 GetTransform(NODE* node, TimeValue t = 0);
	static NODE* FindNode(NODE* node, INode* i_node);

	void wirteFrame( FILE* file, NODE* node );

	void check(NODE* node, std::set<Str>& names, std::set<Str>& duplicates);

private:
	unsigned int mTime, mStart, mEnd;
	Str mPath;
	NODE* mRoot;
	unsigned int mNodeCount;	
	std::vector<INode*>mSkins;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __G3DAExport__

//////////////////////////////////////////////////////////////////////////
