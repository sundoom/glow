/*
 *  GTable.cpp
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

GTable::GTable()
{

}

GTable::GTable(const GTable& t) 
{

}

GTable::~GTable()
{

}

GTable& GTable::operator=(const GTable& t) 
{
  mTable = t.mTable;
  return *this;
}

BOOL GTable::operator==(const GTable& t) const
{
  return mTable == t.mTable ? TRUE : FALSE;
}

BOOL GTable::operator!=(const GTable& t) const
{
  return !(*this == t) ? TRUE : FALSE;
}

VOID GTable::add(const Str& key, const GVar& var)
{
  mTable.insert(std::make_pair(key, var));
}

VOID GTable::del(const Str& key)
{
  std::map<Str, GVar>::iterator it = mTable.find(key);
  if (it == mTable.end())
  {
    mTable.erase(it);
  }
}

const GVar& GTable::get(const Str& key) const
{
  static GVar null_var;
  std::map<Str, GVar>::const_iterator it = mTable.find(key);
  if (it != mTable.end())
  {
    return it->second;
  }
  return null_var;
}

//////////////////////////////////////////////////////////////////////////
