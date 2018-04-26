/*
 *  GVar.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glow.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

GVar::GVar()
{
  val.vt = VT_VOID;
  val.vf64 = 0;
  val.vb = FALSE;
}

GVar::GVar(const GVar& var) 
{
	val.vt = var.val.vt;
	switch (var.val.vt)
	{
	case VT_VOID:
		break;
	case VT_NULL:
		break;
	case VT_I8:
		val.vi8 = var.val.vi8;
		break;
	case VT_U8:
		val.vu8 = var.val.vu8;
		break;
	case VT_I16:
		val.vi16 = var.val.vi16;
		break;
	case VT_U16:
		val.vu16 = var.val.vu16;
		break;
	case VT_I32:
		val.vi32 = var.val.vi32;
		break;
	case VT_U32:
		val.vu32 = var.val.vu32;
		break;
	case VT_I64:
		val.vi64 = var.val.vi64;
		break;
	case VT_U64:
		val.vu64 = var.val.vu64;
		break;
	case VT_F32:
		val.vf32 = var.val.vf32;
		break;
	case VT_F64:
		val.vf64 = var.val.vf64;
		break;
	case VT_BOOL:
		val.vb = var.val.vb;
		break;
	case VT_STR:
    val.str = var.val.str;
		break;
	case VT_WSTR:
    val.wstr = var.val.wstr;
		break;
  case VT_TABLE:
    val.tab = var.val.tab;
    break;
	default:
		GAssert(VA("Error : the type(%d) is unknown!",var.val.vt));
		break;
	}
}

GVar::GVar(I8 v)
{
	val.vt = VT_I8;
	val.vi8 = v;
}

GVar::GVar(U8 v)
{
	val.vt = VT_U8;
	val.vu8 = v;
}

GVar::GVar(I16 v)
{
	val.vt = VT_I16;
	val.vi16 = v;
}

GVar::GVar(U16 v) 
{
	val.vt = VT_U16;
	val.vu16 = v;
}

GVar::GVar(I32 v)
{
	val.vt = VT_I32;
	val.vi32  = v;
}

GVar::GVar(U32 v)
{
	val.vt = VT_U32;
	val.vu32 = v;
}

GVar::GVar(I64 v)
{
	val.vt = VT_I64;
	val.vi64 = v;
}

GVar::GVar(U64 v)
{
	val.vt = VT_U64;
	val.vu64 = v;
}

GVar::GVar(F32 v)
{
	val.vt = VT_F32;
	val.vf32 = v;
}

GVar::GVar(F64 v) 
{
	val.vt = VT_F64;
	val.vf64 = v;
}

GVar::GVar(BOOL v) 
{
	val.vt = VT_BOOL;
	val.vb = v;
}

GVar::GVar(const Str& s) 
{
	val.vt = VT_STR;
  val.str = s;
}

GVar::GVar(const WStr& s) 
{
  val.vt = VT_WSTR;
  val.wstr = s;
}

GVar::GVar(const GTable& t) 
{
  val.vt = VT_TABLE;
  val.tab = t;
}

GVar::~GVar()
{

}

GVar::operator I8() const  
{
	if (val.vt == VT_I8) return val.vi8;
	GVar var;
	var.convert(VT_I8, this);
	return var.val.vi8;
}

GVar::operator U8() const 
{
	if (val.vt == VT_U8) return val.vu8;
	GVar var;
	var.convert(VT_U8, this);
	return var.val.vu8;
}

GVar::operator I16() const 
{
	if (val.vt == VT_I16) return val.vi16;
	GVar var;
	var.convert(VT_I16, this);
	return var.val.vi16;
}

GVar::operator U16() const  
{
	if (val.vt == VT_U16) return val.vu16;
	GVar var;
	var.convert(VT_U16, this);
	return var.val.vu16;
}

GVar::operator I32() const  
{
	if (val.vt == VT_I32) return val.vi32;
	GVar var;
	var.convert(VT_I32, this);
	return var.val.vi32;
}

GVar::operator U32() const  
{
	if (val.vt == VT_U32) return val.vu32;
	GVar var;
	var.convert(VT_U32, this);
	return var.val.vu32;
}

GVar::operator I64() const  
{
	if (val.vt == VT_I64) return val.vi64;
	GVar var;
	var.convert(VT_I64, this);
	return var.val.vi64;
}

GVar::operator U64() const  
{
	if (val.vt == VT_U64) return val.vu64;
	GVar var;
	var.convert(VT_U64, this);
	return var.val.vu64;
}

GVar::operator F32() const 
{
	if (val.vt == VT_F32) return val.vf32;
	GVar var;
	var.convert(VT_F32, this);
	return var.val.vf32;
}

GVar::operator F64() const 
{
	if (val.vt == VT_F64) return val.vf64;
	GVar var;
	var.convert(VT_F64, this);
	return var.val.vf64;
}

GVar::operator BOOL() const 
{
	if (val.vt == VT_BOOL) return val.vb;
	GVar var;
	var.convert(VT_BOOL, this);
	return var.val.vb;
}

GVar::operator Str() const
{
	if (val.vt == VT_STR) return val.str;
	GVar var; 
  var.convert(VT_STR, this);
	return var.val.str;
}

GVar::operator WStr() const
{
	if (val.vt == VT_WSTR) return val.wstr;
  GVar var; 
  var.convert(VT_WSTR, this);
  return var.val.wstr;
}

GVar::operator GTable() const
{
  if (val.vt == VT_TABLE) return val.tab;
  GVar var;
  var.convert(VT_TABLE, this);
  return var.val.tab;
}


GVar& GVar::operator=(const GVar& var) 
{
	val.vt = var.val.vt;
	switch (var.val.vt) 
	{
	case VT_VOID:
		break;
	case VT_NULL:
		break;
	case VT_I8:
		val.vi8 = var.val.vi8;
		break;
	case VT_U8:
		val.vu8 = var.val.vu8;
		break;
	case VT_I16:
		val.vi16 = var.val.vi16;
		break;
	case VT_U16:
		val.vu16 = var.val.vu16;
		break;
	case VT_I32:
		val.vi32 = var.val.vi32;
		break;
	case VT_U32:
		val.vu32 = var.val.vu32;
		break;
	case VT_I64:
		val.vi64 = var.val.vi64;
		break;
	case VT_U64:
		val.vu64 = var.val.vu64;
		break;
	case VT_F32:
		val.vf32 = var.val.vf32;
		break;
	case VT_F64:
		val.vf64 = var.val.vf64;
		break;
	case VT_BOOL:
		val.vb = var.val.vb;
		break;
	case VT_STR:
    val.str = var.val.str;
		break;
	case VT_WSTR:
    val.wstr = var.val.wstr;
		break;
  case VT_TABLE:
    val.tab = var.val.tab;
    break;
	default:
		GAssert(VA("Error : the type(%d) is unknown!",var.val.vt));
		break;
	}
	return *this;
}

GVar& GVar::operator=(I8 v) 
{
	val.vt = VT_I8;
	val.vi8 = v;
	return *this;
}

GVar& GVar::operator=(U8 v) 
{
	val.vt = VT_U8;
	val.vu8 = v;
	return *this;
}

GVar& GVar::operator=(I16 v) 
{
	val.vt = VT_I16;
	val.vi16 = v;
	return *this;
}

GVar& GVar::operator=(U16 v) 
{
	val.vt = VT_U16;
	val.vu16 = v;
	return *this;
}

GVar& GVar::operator=(I32 v) 
{
	val.vt = VT_I32;
	val.vi32 = v;
	return *this;
}

GVar& GVar::operator=(U32 v) 
{
	val.vt = VT_U32;
	val.vu32 = v;
	return *this;
}

GVar& GVar::operator=(I64 v) 
{
	val.vt = VT_I8;
	val.vi64 = v;
	return *this;
}

GVar& GVar::operator=(U64 v) 
{
	val.vt = VT_U64;
	val.vu64 = v;
	return *this;
}

GVar& GVar::operator=(F32 v) 
{
	val.vt = VT_F32;
	val.vf32 = v;
	return *this;
}

GVar& GVar::operator=(F64 v) 
{
	val.vt = VT_F64;	
	val.vf64 = v;
	return *this;
}

GVar& GVar::operator=(BOOL v) 
{
	val.vt = VT_BOOL;
	val.vb = v;
	return *this;
}

GVar& GVar::operator=(const Str& s)
{
	val.vt = VT_STR;
	val.str = s;
	return *this;
}

GVar& GVar::operator=(const WStr& s) 
{
	val.vt = VT_WSTR;
	val.wstr = s;
	return *this;
}

GVar& GVar::operator=(const GTable& t) 
{
  val.vt = VT_TABLE;
  val.tab = t;
  return *this;
}

BOOL GVar::operator==(const GVar& var) const
{
	if (val.vt != var.val.vt) return FALSE;
	switch (var.val.vt) 
	{
	case VT_VOID:
	case VT_NULL:
		return TRUE;
	case VT_I8:
		return val.vi8 == var.val.vi8 ? TRUE : FALSE; 
	case VT_U8:
		return val.vu8 == var.val.vu8 ? TRUE : FALSE; 
	case VT_I16:
		return val.vi16 == var.val.vi16 ? TRUE : FALSE; 
	case VT_U16:
		return val.vu16 == var.val.vu16 ? TRUE : FALSE; 
	case VT_I32:
		return val.vi32 == var.val.vi32 ? TRUE : FALSE; 
	case VT_U32:
		return val.vu32 == var.val.vu32 ? TRUE : FALSE; 
	case VT_I64:
		return val.vi64 == var.val.vi64 ? TRUE : FALSE; 
	case VT_U64:
		return val.vu64 == var.val.vu64 ? TRUE : FALSE; 
	case VT_F32:
		return val.vf32 == var.val.vf32 ? TRUE : FALSE; 
	case VT_F64:
		return val.vf64 == var.val.vf64 ? TRUE : FALSE; 
	case VT_BOOL:
		return val.vb == var.val.vb ? TRUE : FALSE; 
	case VT_STR:
		return val.str == var.val.str ? TRUE : FALSE;
	case VT_WSTR:
		return val.wstr == var.val.wstr ? TRUE : FALSE;
  case VT_TABLE:
    return val.tab == var.val.tab ? TRUE : FALSE;
	default:
		GAssert(VA("Error : the type(%d) is unknown!",var.val.vt));
		return FALSE;
	}
	return FALSE;
}

BOOL GVar::operator!=(const GVar& var) const
{
	return !(*this == var) ? TRUE : FALSE; 
}

U32 GVar::type() const
{
	return val.vt;
}

VOID GVar::convert(VT vt, const GVar* var) 
{
	if (var == NULL) var = this;
	if ((this != var) || (vt != val.vt))
	{
		val.vt = vt;
		switch (vt) 
		{
		case VT_NULL:
			break;
		case VT_I8:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vi8 = var->val.vi8;
					break;
				case VT_U8:
					val.vi8 = var->val.vu8;
					break;
				case VT_I16:
					val.vi8 = var->val.vi16;
					break;
				case VT_U16:
					val.vi8 = var->val.vu16;
					break;
				case VT_I32:
					val.vi8 = var->val.vi32;
					break;
				case VT_U32:
					val.vi8 = var->val.vu32;
					break;
				case VT_I64:
					val.vi8 = var->val.vi64;
					break;
				case VT_U64:
					val.vi8 = var->val.vu64;
					break;
				case VT_F32:
					val.vi8 = var->val.vf32;
					break;
				case VT_F64:
					val.vi8 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vi8 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_I8!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_I8!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_I8!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_U8:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vu8 = var->val.vi8;
					break;
				case VT_U8:
					val.vu8 = var->val.vu8;
					break;
				case VT_I16:
					val.vu8 = var->val.vi16;
					break;
				case VT_U16:
					val.vu8 = var->val.vu16;
					break;
				case VT_I32:
					val.vu8 = var->val.vi32;
					break;
				case VT_U32:
					val.vu8 = var->val.vu32;
					break;
				case VT_I64:
					val.vu8 = var->val.vi64;
					break;
				case VT_U64:
					val.vu8 = var->val.vu64;
					break;
				case VT_F32:
					val.vu8 = var->val.vf32;
					break;
				case VT_F64:
					val.vu8 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vu8 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_U8!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_U8!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_U8!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_I16:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vi16 = var->val.vi8;
					break;
				case VT_U8:
					val.vi16 = var->val.vu8;
					break;
				case VT_I16:
					val.vi16 = var->val.vi16;
					break;
				case VT_U16:
					val.vi16 = var->val.vu16;
					break;
				case VT_I32:
					val.vi16 = var->val.vi32;
					break;
				case VT_U32:
					val.vi16 = var->val.vu32;
					break;
				case VT_I64:
					val.vi16 = var->val.vi64;
					break;
				case VT_U64:
					val.vi16 = var->val.vu64;
					break;
				case VT_F32:
					val.vi16 = var->val.vf32;
					break;
				case VT_F64:
					val.vi16 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vi16 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_I16!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_I16!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_I16!");
          break;
        default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_U16:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vu16 = var->val.vi8;
					break;
				case VT_U8:
					val.vu16 = var->val.vu8;
					break;
				case VT_I16:
					val.vu16 = var->val.vi16;
					break;
				case VT_U16:
					val.vu16 = var->val.vu16;
					break;
				case VT_I32:
					val.vu16 = var->val.vi32;
					break;
				case VT_U32:
					val.vu16 = var->val.vu32;
					break;
				case VT_I64:
					val.vu16 = var->val.vi64;
					break;
				case VT_U64:
					val.vu16 = var->val.vu64;
					break;
				case VT_F32:
					val.vu16 = var->val.vf32;
					break;
				case VT_F64:
					val.vu16 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vu16 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_U16!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_U16!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_U16!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_I32:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vi32 = var->val.vi8;
					break;
				case VT_U8:
					val.vi32 = var->val.vu8;
					break;
				case VT_I16:
					val.vi32 = var->val.vi16;
					break;
				case VT_U16:
					val.vi32 = var->val.vu16;
					break;
				case VT_I32:
					val.vi32 = var->val.vi32;
					break;
				case VT_U32:
					val.vi32 = var->val.vu32;
					break;
				case VT_I64:
					val.vi32 = var->val.vi64;
					break;
				case VT_U64:
					val.vi32 = var->val.vu64;
					break;
				case VT_F32:
					val.vi32 = var->val.vf32;
					break;
				case VT_F64:
					val.vi32 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vi32 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_I32!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_I32!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_I32!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_U32:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vu32 = var->val.vi8;
					break;
				case VT_U8:
					val.vu32 = var->val.vu8;
					break;
				case VT_I16:
					val.vu32 = var->val.vi16;
					break;
				case VT_U16:
					val.vu32 = var->val.vu16;
					break;
				case VT_I32:
					val.vu32 = var->val.vi32;
					break;
				case VT_U32:
					val.vu32 = var->val.vu32;
					break;
				case VT_I64:
					val.vu32 = var->val.vi64;
					break;
				case VT_U64:
					val.vu32 = var->val.vu64;
					break;
				case VT_F32:
					val.vu32 = var->val.vf32;
					break;
				case VT_F64:
					val.vu32 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vu32 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_U32!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_U32!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_U32!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_I64:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vi64 = var->val.vi8;
					break;
				case VT_U8:
					val.vi64 = var->val.vu8;
					break;
				case VT_I16:
					val.vi64 = var->val.vi16;
					break;
				case VT_U16:
					val.vi64 = var->val.vu16;
					break;
				case VT_I32:
					val.vi64 = var->val.vi32;
					break;
				case VT_U32:
					val.vi64 = var->val.vu32;
					break;
				case VT_I64:
					val.vi64 = var->val.vi64;
					break;
				case VT_U64:
					val.vi64 = var->val.vu64;
					break;
				case VT_F32:
					val.vi64 = var->val.vf32;
					break;
				case VT_F64:
					val.vi64 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vi64 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_I64!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_I64!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_I64!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_U64:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vu64 = var->val.vi8;
					break;
				case VT_U8:
					val.vu64 = var->val.vu8;
					break;
				case VT_I16:
					val.vu64 = var->val.vi16;
					break;
				case VT_U16:
					val.vu64 = var->val.vu16;
					break;
				case VT_I32:
					val.vu64 = var->val.vi32;
					break;
				case VT_U32:
					val.vu64 = var->val.vu32;
					break;
				case VT_I64:
					val.vu64 = var->val.vi64;
					break;
				case VT_U64:
					val.vu64 = var->val.vu64;
					break;
				case VT_F32:
					val.vu64 = var->val.vf32;
					break;
				case VT_F64:
					val.vu64 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vu64 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_U64!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_U64!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_U64!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_F32:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vf32 = var->val.vi8;
					break;
				case VT_U8:
					val.vf32 = var->val.vu8;
					break;
				case VT_I16:
					val.vf32 = var->val.vi16;
					break;
				case VT_U16:
					val.vf32 = var->val.vu16;
					break;
				case VT_I32:
					val.vf32 = var->val.vi32;
					break;
				case VT_U32:
					val.vf32 = var->val.vu32;
					break;
				case VT_I64:
					val.vf32 = var->val.vi64;
					break;
				case VT_U64:
					val.vf32 = (I64)var->val.vu64;
					break;
				case VT_F32:
					val.vf32 = var->val.vf32;
					break;
				case VT_F64:
					val.vf32 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vf32 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_F32!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_F32!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_F32!");
          break;
        default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_F64:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vf64 = var->val.vi8;
					break;
				case VT_U8:
					val.vf64 = var->val.vu8;
					break;
				case VT_I16:
					val.vf64 = var->val.vi16;
					break;
				case VT_U16:
					val.vf64 = var->val.vu16;
					break;
				case VT_I32:
					val.vf64 = var->val.vi32;
					break;
				case VT_U32:
					val.vf64 = var->val.vu32;
					break;
				case VT_I64:
					val.vf64 = var->val.vi64;
					break;
				case VT_U64:
					val.vf64 = (I64)var->val.vu64;
					break;
				case VT_F32:
					val.vf64 = var->val.vf32;
					break;
				case VT_F64:
					val.vf64 = var->val.vf64;
					break;
				case VT_BOOL:
					val.vf64 = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_F64!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_F64!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_F64!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_BOOL:
			{
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
					val.vb = var->val.vi8 ? TRUE : FALSE;
					break;
				case VT_U8:
					val.vb = var->val.vu8 ? TRUE : FALSE;
					break;
				case VT_I16:
					val.vb = var->val.vi16 ? TRUE : FALSE;
					break;
				case VT_U16:
					val.vb = var->val.vu16 ? TRUE : FALSE;
					break;
				case VT_I32:
					val.vb = var->val.vi32 ? TRUE : FALSE;
					break;
				case VT_U32:
					val.vb = var->val.vu32 ? TRUE : FALSE;
					break;
				case VT_I64:
					val.vb = var->val.vi64 ? TRUE : FALSE;
					break;
				case VT_U64:
					val.vb = (I64)var->val.vu64 ? TRUE : FALSE;
					break;
				case VT_F32:
					val.vb = var->val.vf32 ? TRUE : FALSE;
					break;
				case VT_F64:
					val.vb = var->val.vf64 ? TRUE : FALSE;
					break;
				case VT_BOOL:
					val.vb = var->val.vb;
					break;
				case VT_STR:
					GAssert("Error : can`t convert the type from VT_STR to VT_BOOL!");
					break;
				case VT_WSTR:
					GAssert("Error : can`t convert the type from VT_WSTR to VT_BOOL!");
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_BOOL!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_STR:
			{
        std::ostringstream oss;
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
          oss.clear(); oss << var->val.vi8; val.str = oss.str();
					break;
				case VT_U8:
          oss.clear(); oss << var->val.vu8; val.str = oss.str();
					break;
				case VT_I16:
          oss.clear(); oss << var->val.vi16; val.str = oss.str();
					break;
				case VT_U16:
          oss.clear(); oss << var->val.vu16; val.str = oss.str();
					break;
				case VT_I32:
          oss.clear(); oss << var->val.vi32; val.str = oss.str();
					break;
				case VT_U32:
          oss.clear(); oss << var->val.vu32; val.str = oss.str();
					break;
				case VT_I64:
          oss.clear(); oss << var->val.vi64; val.str = oss.str();
					break;
				case VT_U64:
          oss.clear(); oss << var->val.vu64; val.str = oss.str();
					break;
				case VT_F32:
          oss.clear(); oss << var->val.vf32; val.str = oss.str();
					break;
				case VT_F64:
          oss.clear(); oss << var->val.vf64; val.str = oss.str();
					break;
				case VT_BOOL:
          oss.clear(); oss << var->val.vb; val.str = oss.str();
					break;
				case VT_STR:
          val.str = var->val.str;
					break;
				case VT_WSTR:
          val.str.assign(var->val.wstr.begin(), var->val.wstr.end());
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_STR!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		case VT_WSTR:
			{
        std::wostringstream oss;
				switch (var->val.vt) 
				{
				case VT_NULL:
					break;
				case VT_I8:
          oss.clear(); oss << var->val.vi8; val.wstr = oss.str();
					break;
				case VT_U8:
          oss.clear(); oss << var->val.vu8; val.wstr = oss.str();
					break;
				case VT_I16:
          oss.clear(); oss << var->val.vi16; val.wstr = oss.str();
					break;
				case VT_U16:
          oss.clear(); oss << var->val.vu16; val.wstr = oss.str();
					break;
				case VT_I32:
          oss.clear(); oss << var->val.vi32; val.wstr = oss.str();
					break;
				case VT_U32:
          oss.clear(); oss << var->val.vu32; val.wstr = oss.str();
					break;
				case VT_I64:
          oss.clear(); oss << var->val.vi64; val.wstr = oss.str();
					break;
				case VT_U64:
          oss.clear(); oss << var->val.vu64; val.wstr = oss.str();
					break;
				case VT_F32:
          oss.clear(); oss << var->val.vf32; val.wstr = oss.str();
					break;
				case VT_F64:
          oss.clear(); oss << var->val.vf64; val.wstr = oss.str();
					break;
				case VT_BOOL:
          oss.clear(); oss << var->val.vb; val.wstr = oss.str();
					break;
				case VT_STR:
          val.wstr.assign(var->val.str.begin(), var->val.str.end());
					break;
				case VT_WSTR:
          val.wstr = var->val.wstr;
					break;
        case VT_TABLE:
          GAssert("Error : can`t convert the type from VT_TABLE to VT_WSTR!");
          break;
				default:
					GAssert(VA("Error : the type(%d) is unknown!",vt));
					break;
				}
			}
			break;
		default:
			GAssert(VA("Error : the type(%d) is unknown!",vt));
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
