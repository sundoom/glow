/*
 *  GVar.h
 *  glow
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __GVar__
#define __GVar__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

class GVar
{
public:
  enum VT
  {	
    VT_VOID,
    VT_NULL,
    VT_I8,
    VT_I16,
    VT_I32,
    VT_I64,
    VT_U8,
    VT_U16,
    VT_U32,
    VT_U64,
    VT_F32,
    VT_F64,
    VT_BOOL,
    VT_STR,
    VT_WSTR,
    VT_TABLE
  };

public:
  GVar();
  GVar(const GVar& var);
  GVar(I8 v);
  GVar(U8 v);		
  GVar(I16 v);
  GVar(U16 v);
  GVar(I32 v);
  GVar(U32 v);
  GVar(I64 v);
  GVar(U64 v);		
  GVar(F32 v);
  GVar(F64 v);
  GVar(BOOL v);
  GVar(const Str& s);
  GVar(const WStr& s);
  GVar(const GTable& t);
  ~GVar();

  operator I8() const;
  operator U8() const;
  operator I16() const;
  operator U16() const;
  operator I32() const;
  operator U32() const;
  operator I64() const;
  operator U64() const;
  operator F32() const;
  operator F64() const;
  operator BOOL() const;
  operator Str() const;
  operator WStr() const;
  operator GTable() const;

  GVar& operator=(const GVar& var);

  GVar& operator=(I8 v);
  GVar& operator=(U8 v);
  GVar& operator=(I16 v);
  GVar& operator=(U16 v);
  GVar& operator=(I32 v) ;
  GVar& operator=(U32 v);
  GVar& operator=(I64 v);
  GVar& operator=(U64 v);
  GVar& operator=(F32 v);
  GVar& operator=(F64 v);
  GVar& operator=(BOOL v);
  GVar& operator=(const Str& s);
  GVar& operator=(const WStr& s);
  GVar& operator=(const GTable& t);

  BOOL operator==(const GVar& var) const;
  BOOL operator!=(const GVar& var) const;

public:
  U32 type() const;
  VOID convert(VT vt, const GVar* var = NULL);

private:
  struct VAL
  {
    VT vt;
    union 
    {
      I8		vi8;
      U8		vu8;
      I16		vi16;
      U16		vu16;
      I32		vi32;
      U32		vu32;
      I64		vi64;
      U64		vu64;
      F32		vf32;
      F64   vf64;
    };
    BOOL    vb;
    Str     str;
    WStr    wstr;
    GTable  tab;
  }val;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GVar__

//////////////////////////////////////////////////////////////////////////