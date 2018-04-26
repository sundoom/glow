/*
 *  Param.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include "Param.h"
#include "Script.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

Param::Param()
{

}

Param::~Param()
{

}

VOID Param::add(const GVar& var)
{
  switch (var.type())
  {
  case GVar::VT_VOID:
    break;
  case GVar::VT_NULL:
    {
      U32 size = mData.size();
      U32 var_size = sizeof(U8);
      mData.resize(size + var_size);
      U8* data = &mData[size];
      (*((U8*)data)) = LUA_TNIL;
    }
    break;
  case GVar::VT_I8:
  case GVar::VT_U8:
  case GVar::VT_I16:
  case GVar::VT_U16:
  case GVar::VT_I32:
  case GVar::VT_U32:
  case GVar::VT_I64:
  case GVar::VT_U64:
  case GVar::VT_F32:
  case GVar::VT_F64:
    {
      U32 size = mData.size();
      U32 var_size = sizeof(U8) + sizeof(LUA_NUMBER);
      mData.resize(size + var_size);
      U8* data = &mData[size];
      U32 pos = 0;
      (*((U8*)(data + pos))) = LUA_TNUMBER;
      pos += sizeof(U8);
      (*((LUA_NUMBER*)(data + pos))) = (LUA_NUMBER)var;
      pos += sizeof(LUA_NUMBER);
    }
    break;
  case GVar::VT_BOOL:
    {
      U32 size = mData.size();
      U32 var_size = sizeof(U8) + sizeof(I32);
      mData.resize(size + var_size);
      U8* data = &mData[size];
      U32 pos = 0;
      (*((U8*)(data + pos))) = LUA_TBOOLEAN;
      pos += sizeof(U8);
      (*((I32*)(data + pos))) = ((BOOL)var) ? 1 : 0;
      pos += sizeof(I32);
    }
    break;
  case GVar::VT_STR:
    {
      U32 size = mData.size();
      U32 var_size = sizeof(U8) + ((Str)var).size() * sizeof(CHAR);
      mData.resize(size + var_size);
      U8* data = &mData[size];
      U32 pos = 0;
      (*((U8*)(data + pos))) = LUA_TSTRING;
      pos += sizeof(U8);
      ::memcpy(data + pos, ((Str)var).c_str(), ((Str)var).size() * sizeof(CHAR));
      pos += ((Str)var).size() * sizeof(CHAR);
    }
    break;
  case GVar::VT_WSTR:
    {
      U32 size = mData.size();
      U32 var_size = sizeof(U8) + ((WStr)var).size() * sizeof(WCHAR);
      mData.resize(size + var_size);
      U8* data = &mData[size];
      U32 pos = 0;
      (*((U8*)(data + pos))) = LUA_TSTRING;
      pos += sizeof(U8);
      ::memcpy(data + pos, ((WStr)var).c_str(), ((WStr)var).size() * sizeof(WCHAR));
      pos += ((WStr)var).size() * sizeof(WCHAR);
    }
    break;
  case GVar::VT_TABLE:
    {
      U32 size = mData.size();
      U32 var_size = sizeof(U8) + sizeof(U32);
      mData.resize(size + var_size);
      U8* data = &mData[size];
      U32 pos = 0;
      (*((U8*)(data + pos))) = LUA_TTABLE;
      pos += sizeof(U8);
      const GTable& tab = (GTable)var;
      (*((U32*)(data + pos))) = tab.mTable.size();
      pos += sizeof(U32);
      for (std::map<Str, GVar>::const_iterator it = tab.mTable.begin(); it != tab.mTable.end(); ++it)
      {
        // key
        this->add(it->first);
        // value
        this->add(it->second);
      }
    }
    break;
  default:
    GAssert(VA("Error : the type(%d) is unknown!",var.type()));
    break;
  }
}

/*
====================
data
====================
*/
VOID* Param::data() const
{
  if (mData.size()) return (VOID*)&mData[0];
  return NULL;
}

/*
====================
size
====================
*/
U32 Param::size() const
{
  return mData.size();
}

//////////////////////////////////////////////////////////////////////////