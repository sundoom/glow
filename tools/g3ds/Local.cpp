//////////////////////////////////////////////////////////////////////////
// $Id: Local.cpp,v 1.1 2008/12/20 12:20:36 sunjun Exp $
//////////////////////////////////////////////////////////////////////////

#include "Local.h"

//////////////////////////////////////////////////////////////////////////

HINSTANCE gInstance = NULL;

//////////////////////////////////////////////////////////////////////////

class G3DMClassDesc : public ClassDesc 
{
public:
	int 			IsPublic()						{ return 1; }
	void *			Create(BOOL loading = FALSE)	{ return new G3DMExport; }
	const TCHAR *	ClassName()						{ return _T("Glow 3D Static Mesh"); }
	SClass_ID		SuperClassID()					{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()						{ return Class_ID(0x30008001, 0x30008001); }
	const TCHAR* 	Category()						{ return _T("Glow 3D Static Mesh Export");  }
};

class G3DSClassDesc : public ClassDesc 
{
public:
	int 			IsPublic()						{ return 1; }
	void *			Create(BOOL loading = FALSE)	{ return new G3DSExport; }
	const TCHAR *	ClassName()						{ return _T("Glow 3D Skeletal Mesh"); }
	SClass_ID		SuperClassID()					{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()						{ return Class_ID(0x30008002, 0x30008002); }
	const TCHAR* 	Category()						{ return _T("Glow 3D Skeletal Mesh Export");  }
};

class G3DAClassDesc : public ClassDesc 
{
public:
	int 			IsPublic()						{ return 1; }
	void *			Create(BOOL loading = FALSE)	{ return new G3DAExport; }
	const TCHAR *	ClassName()						{ return _T("Glow 3D Animation"); }
	SClass_ID		SuperClassID()					{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()						{ return Class_ID(0x30008003, 0x30008003); }
	const TCHAR* 	Category()						{ return _T("Glow 3D Animation Export");  }
};

//////////////////////////////////////////////////////////////////////////

/*
====================
DllMain
====================
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) 
{
	switch(fdwReason) 
	{
	case DLL_PROCESS_ATTACH:
		gInstance = hinstDLL;
		DisableThreadLibraryCalls(hinstDLL);
		break;
	}

	return TRUE;
}

/*
====================
LibDescription
====================
*/
__declspec( dllexport ) const TCHAR * LibDescription() 
{ 
	return _T("Glow 3D Scene Tool.");
}

/*
====================
LibNumberClasses
====================
*/
__declspec( dllexport ) int LibNumberClasses() 
{ 
	return 3; 
}

/*
====================
LibClassDesc
====================
*/
__declspec( dllexport ) ClassDesc * LibClassDesc(int i) 
{
	static G3DMClassDesc s_g3dm_cd;
	static G3DSClassDesc s_g3ds_cd;
	static G3DAClassDesc s_g3da_cd;
	switch(i) 
	{
		case 0: return &s_g3dm_cd;
		case 1: return &s_g3ds_cd;
		case 2: return &s_g3da_cd;
		default: return 0;
	}
}

/*
====================
LibVersion
====================
*/
__declspec( dllexport ) ULONG LibVersion() 
{ 
	return VERSION_3DSMAX; 
}

//////////////////////////////////////////////////////////////////////////

Str UnifySlashes(const Str& s)
{
	Str ret = s;
	int n;
	while ((n = ret.find_first_of( "\\" )) > 0)
	{
		ret[n] = '/';
	}
	return ret;
}

Str TrimWhitespace(const Str& s)
{
	int f = s.find_first_not_of( ' ' );
	int l = s.find_last_not_of( ' ' );
	return s.substr( f, l - f + 1 );
}

//////////////////////////////////////////////////////////////////////////

void G3DAssert( const char *format, ... )
{
	va_list	argptr;
	static CHAR	string[32000];	// in case va is called by nested functions
	va_start (argptr, format);
	_vsnprintf(string,sizeof(string), format,argptr);
	va_end (argptr);	
	::MessageBox(::GetActiveWindow(), string, "G3D Assert!", MB_OK|MB_ICONERROR|MB_TASKMODAL);
}

//////////////////////////////////////////////////////////////////////////