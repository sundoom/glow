//////////////////////////////////////////////////////////////////////////
// Local.h
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// MAXPATH = C:\Autodesk\3ds Max 2008
// MAXSDKPATH = C:\Autodesk\3dsMax2008SDK\maxsdk
//////////////////////////////////////////////////////////////////////////

#ifndef __Local__
#define __Local__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#	pragma warning(disable : 4786)	// identifier was truncated to 256 characters in the debug information
#	pragma warning(disable : 4244)  // conversion to float, possible loss of data
#	pragma warning(disable : 4699)	// creating precompiled header
#	pragma warning(disable : 4200)	// Zero-length array item at end of structure, a VC-specific extension
#	pragma warning(disable : 4100)	// unreferenced formal parameter
#	pragma warning(disable : 4514)	// unreferenced inline function has been removed
#	pragma warning(disable : 4201)	// nonstandard extension used : nameless struct/union
#	pragma warning(disable : 4710)	// inline function not expanded
#	pragma warning(disable : 4714)	// __forceinline function not expanded
#	pragma warning(disable : 4702)	// unreachable code in inline expanded function
#	pragma warning(disable : 4711)	// function selected for autmatic inlining
#	pragma warning(disable : 4725)	// Pentium fdiv bug
#	pragma warning(disable : 4127)	// Conditional expression is constant
#	pragma warning(disable : 4512)	// assignment operator could not be generated
#	pragma warning(disable : 4530)	// C++ exception handler used, but unwind semantics are not enabled
#	pragma warning(disable : 4245)	// conversion from 'enum ' to 'unsigned long', signed/unsigned mismatch
#	pragma warning(disable : 4389)	// signed/unsigned mismatch
#	pragma warning(disable : 4238)	// nonstandard extension used : class rvalue used as lvalue
#	pragma warning(disable : 4251)	// needs to have dll-interface to be used by clients of class 'ULinker'
#	pragma warning(disable : 4275)	// non dll-interface class used as base for dll-interface class
#	pragma warning(disable : 4511)	// copy constructor could not be generated
#	pragma warning(disable : 4284)	// return type is not a UDT or reference to a UDT
#	pragma warning(disable : 4355)	// this used in base initializer list
#	pragma warning(disable : 4097)	// typedef-name '' used as synonym for class-name ''
#	pragma warning(disable : 4291)	// typedef-name '' used as synonym for class-name ''
#	pragma warning(disable : 4731)	// frame pointer register 'ebp' modified by inline assembly code
#endif

#if _MSC_VER == 1400
#	pragma warning(disable : 4819)	// The file contains a character that cannot be represented in the current code page (number). Save the file in Unicode format to prevent data loss.
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////////

#include <MAX.H>
#include <iparamb2.h>
#include <iskin.h>
#include <istdplug.h>
#include <stdmat.h>
#include <decomp.h>
#include <shape.h>
#include <interpik.h>
#include <modstack.h>
#include <phyexp.h>
#include <simpobj.h>

//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
typedef std::string	Str;

//////////////////////////////////////////////////////////////////////////

#include "G3DMExport.h"
#include "G3DSExport.h"
#include "G3DAExport.h"

//////////////////////////////////////////////////////////////////////////

extern HINSTANCE gInstance;

//////////////////////////////////////////////////////////////////////////

#define MAX_CHECK(expr) {if(!(expr)){std::ostringstream error;error << "\nError detected in " << __FILE__ << " at line " << __LINE__ << ": " << #expr;::MessageBox( ::GetActiveWindow(), error.str().c_str(), "Glow Assert", MB_OK|MB_ICONERROR|MB_TASKMODAL );}}
void G3DAssert( const char *format, ... );

//////////////////////////////////////////////////////////////////////////

#define MAKE_FOURCC(ch0, ch1, ch2, ch3)  ((unsigned int)(unsigned char)(ch0)|((unsigned int)(unsigned char)(ch1)<<8)|((unsigned int)(unsigned char)(ch2)<<16)|((unsigned int)(unsigned char)(ch3)<<24))

//////////////////////////////////////////////////////////////////////////

Str UnifySlashes(const Str& s);
Str TrimWhitespace( const Str &s );

//////////////////////////////////////////////////////////////////////////

#pragma pack(push,1)
// vertex element semantics
enum SEMANTIC 
{
	VES_POSITION		= 0,
	VES_BLENDWEIGHT		= 1,
	VES_BLENDINDICES	= 2,
	VES_NORMAL			= 3,
	VES_PSIZE			= 4,
	VES_TEXCOORD		= 5,
	VES_TANGENT			= 6,
	VES_BINORMAL		= 7,
	VES_TESSFACTOR		= 8,
	VES_POSITIONT		= 9,
	VES_COLOR			= 10,
	VES_FOG				= 11,
	VES_DEPTH			= 12,
	VES_SAMPLE			= 13
};

/// vertex element type
enum TYPE
{
	VET_FLOAT1		=  0,
	VET_FLOAT2		=  1,
	VET_FLOAT3		=  2,
	VET_FLOAT4		=  3,
	VET_COLOR		=  4,
	VET_UBYTE4		=  5,
	VET_SHORT2		=  6,
	VET_SHORT4		=  7,
	VET_UBYTE4N		=  8,
	VET_SHORT2N		=  9,
	VET_SHORT4N		= 10,
	VET_USHORT2N	= 11,
	VET_USHORT4N	= 12,
};

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Local__

//////////////////////////////////////////////////////////////////////////
