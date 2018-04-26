/*
 *  Command.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Command__
#define __Command__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <RHI.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(RHI)

//////////////////////////////////////////////////////////////////////////

#pragma pack (push,1)

//////////////////////////////////////////////////////////////////////////

  typedef enum _CMD_TYPE
  {    
    CT_CREATE_VERTEX_BUFFER,
    CT_UPDATE_VERTEX_BUFFER,
    CT_DESTROY_VERTEX_BUFFER,
    CT_BIND_VERTEX_BUFFER,
    CT_CREATE_INDEX_BUFFER,
    CT_UPDATE_INDEX_BUFFER,
    CT_DESTROY_INDEX_BUFFER,
    CT_BIND_INDEX_BUFFER,
    CT_CREATE_TEXTURE,
    CT_UPDATE_TEXTURE,
    CT_DESTROY_TEXTURE,
    CT_BIND_TEXTURE,
    CT_CREATE_RENDER_TARGET,
    CT_DESTROY_RENDER_TARGET,
    CT_BIND_RENDER_TARGET,
    CT_CREATE_SHADER,
    CT_DESTROY_SHADER,
    CT_BIND_SHADER,
    CT_SET_SHADER_PARAMETER_1F,
    CT_SET_SHADER_PARAMETER_2F,
    CT_SET_SHADER_PARAMETER_3F,
    CT_SET_SHADER_PARAMETER_4F,
    CT_SET_SHADER_PARAMETER_1I,
    CT_SET_SHADER_PARAMETER_2I,
    CT_SET_SHADER_PARAMETER_3I,
    CT_SET_SHADER_PARAMETER_4I,
    CT_SET_SHADER_PARAMETER_1FV,
    CT_SET_SHADER_PARAMETER_2FV,
    CT_SET_SHADER_PARAMETER_3FV,
    CT_SET_SHADER_PARAMETER_4FV,
    CT_SET_SHADER_PARAMETER_1IV,
    CT_SET_SHADER_PARAMETER_2IV,
    CT_SET_SHADER_PARAMETER_3IV,
    CT_SET_SHADER_PARAMETER_4IV,
    CT_SET_SHADER_PARAMETER_MATRIX_2FV,
    CT_SET_SHADER_PARAMETER_MATRIX_3FV,
    CT_SET_SHADER_PARAMETER_MATRIX_4FV,
    CT_SET_SHADER_PARAMETER_SAMPLER,
    CT_RESET,
    CT_CLEAR,
    CT_SET_VIEWPORT,
    CT_SET_SCISSOR,
    CT_DRAW_PRIMITIVE,
  }CMD_TYPE;

  typedef struct _CMD_BASE
  {
    U16	type;
  }CMD_BASE; 

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_CREATE_VERTEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
    U32 count;
    U32 stride;
    U8 attrib[16];
  }CMD_CREATE_VERTEX_BUFFER;

  typedef struct _CMD_UPDATE_VERTEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
    U32 count;
    U32 stride;
    U8 attrib[16];
  }CMD_UPDATE_VERTEX_BUFFER;

  typedef struct _CMD_DESTROY_VERTEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_DESTROY_VERTEX_BUFFER; 

  typedef struct _CMD_BIND_VERTEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_BIND_VERTEX_BUFFER; 

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_CREATE_INDEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
    U32 count;
    U32 stride;
  }CMD_CREATE_INDEX_BUFFER;

  typedef struct _CMD_UPDATE_INDEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
    U32 count;
    U32 stride;
  }CMD_UPDATE_INDEX_BUFFER;

  typedef struct _CMD_DESTROY_INDEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_DESTROY_INDEX_BUFFER; 

  typedef struct _CMD_BIND_INDEX_BUFFER
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_BIND_INDEX_BUFFER; 

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_CREATE_TEXTURE
  {
    CMD_BASE base;
    HANDLE handle;
    U32 level;
    U32 width;
    U32 height;
    U32 format;
    U32 type;
  }CMD_CREATE_TEXTURE;

  typedef struct _CMD_UPDATE_TEXTURE
  {
    CMD_BASE base;
    HANDLE handle;
    U32 level;
    I32 x;
    I32 y;
    U32 width;
    U32 height;
    U32 format;
    U32 type;
  }CMD_UPDATE_TEXTURE;

  typedef struct _CMD_DESTROY_TEXTURE
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_DESTROY_TEXTURE; 

  typedef struct _CMD_BIND_TEXTURE
  {
    CMD_BASE base;
    HANDLE handle;
    U32 unit;
  }CMD_BIND_TEXTURE;

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_CREATE_RENDER_TARGET
  {
    CMD_BASE base;
    HANDLE handle;
    HANDLE color;
    HANDLE depth;    
  }CMD_CREATE_RENDER_TARGET;

  typedef struct _CMD_DESTROY_RENDER_TARGET
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_DESTROY_RENDER_TARGET; 

  typedef struct _CMD_BIND_RENDER_TARGET
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_BIND_RENDER_TARGET;

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_CREATE_SHADER
  {
    CMD_BASE base;
    HANDLE handle;
    U32 size;
  }CMD_CREATE_SHADER;

  typedef struct _CMD_DESTROY_SHADER
  {
    CMD_BASE base;
    HANDLE handle;
  }CMD_DESTROY_SHADER;

  typedef struct _CMD_BIND_SHADER
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pass[MAX_NAME];
  }CMD_BIND_SHADER;

  typedef struct _CMD_SET_SHADER_PARAMETER_1F
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    F32 v;
  }CMD_SET_SHADER_PARAMETER_1F;
  
  typedef struct _CMD_SET_SHADER_PARAMETER_2F
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    F32 v[2];
  }CMD_SET_SHADER_PARAMETER_2F;
  
  typedef struct _CMD_SET_SHADER_PARAMETER_3F
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    F32 v[3];
  }CMD_SET_SHADER_PARAMETER_3F;
  
  typedef struct _CMD_SET_SHADER_PARAMETER_4F
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    F32 v[4];
  }CMD_SET_SHADER_PARAMETER_4F;

  typedef struct _CMD_SET_SHADER_PARAMETER_1I
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    I32 v;
  }CMD_SET_SHADER_PARAMETER_1I;

  typedef struct _CMD_SET_SHADER_PARAMETER_2I
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    I32 v[2];
  }CMD_SET_SHADER_PARAMETER_2I;

  typedef struct _CMD_SET_SHADER_PARAMETER_3I
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    I32 v[3];
  }CMD_SET_SHADER_PARAMETER_3I;

  typedef struct _CMD_SET_SHADER_PARAMETER_4I
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    I32 v[4];
  }CMD_SET_SHADER_PARAMETER_4I;

  typedef struct _CMD_SET_SHADER_PARAMETER_1FV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_1FV;

  typedef struct _CMD_SET_SHADER_PARAMETER_2FV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_2FV;

  typedef struct _CMD_SET_SHADER_PARAMETER_3FV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_3FV;

  typedef struct _CMD_SET_SHADER_PARAMETER_4FV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_4FV;

  typedef struct _CMD_SET_SHADER_PARAMETER_1IV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_1IV;

  typedef struct _CMD_SET_SHADER_PARAMETER_2IV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_2IV;

  typedef struct _CMD_SET_SHADER_PARAMETER_3IV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_3IV;

  typedef struct _CMD_SET_SHADER_PARAMETER_4IV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_4IV;

  typedef struct _CMD_SET_SHADER_PARAMETER_MATRIX_2FV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_MATRIX_2FV;

  typedef struct _CMD_SET_SHADER_PARAMETER_MATRIX_3FV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_MATRIX_3FV;

  typedef struct _CMD_SET_SHADER_PARAMETER_MATRIX_4FV
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    U32 count;
  }CMD_SET_SHADER_PARAMETER_MATRIX_4FV;

  typedef struct _CMD_SET_SHADER_PARAMETER_SAMPLER
  {
    CMD_BASE base;
    HANDLE handle;
    CHAR pname[MAX_NAME];
    HANDLE texture;
  }CMD_SET_SHADER_PARAMETER_SAMPLER;

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_RESET
  {
    CMD_BASE base;
  }CMD_RESET;

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_CLEAR
  {
    CMD_BASE base;
    U32 flags;
    F32 red;
    F32 green;
    F32 blue;
    F32 alpha;
    F32 depth;
  }CMD_CLEAR;

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_SET_VIEWPORT
  {
    CMD_BASE base;
    I32 x;
    I32 y;
    I32 width;
    I32 height;
  }CMD_SET_VIEWPORT;

  typedef struct _CMD_SET_SCISSOR
  {
    CMD_BASE base;
    BOOL enable;
    I32 x;
    I32 y;
    I32 width;
    I32 height;
  }CMD_SET_SCISSOR;

  //////////////////////////////////////////////////////////////////////////

  typedef struct _CMD_DRAW_PRIMITIVE
  {
    CMD_BASE base;
    U32 mode;
    I32 count;
    U32 type;
  }CMD_DRAW_PRIMITIVE;

//////////////////////////////////////////////////////////////////////////

#pragma pack (pop)

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Command__

//////////////////////////////////////////////////////////////////////////