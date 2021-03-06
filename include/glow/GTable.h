/*
 *  GTable.h
 *  glow
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __GTable__
#define __GTable__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

class GVar;

//////////////////////////////////////////////////////////////////////////

class GTable
{
public:
  GTable();
  GTable(const GTable& t);
  ~GTable();
  
  GTable& operator=(const GTable& t);
  BOOL operator==(const GTable& t) const;
  BOOL operator!=(const GTable& t) const;

  // add/del a value to the table
  VOID add(const Str& key, const GVar& var);
  VOID del(const Str& key);

  // get a value to the table
  const GVar& get(const Str& key) const;

public:
  std::map<Str, GVar>mTable;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GTable__

//////////////////////////////////////////////////////////////////////////