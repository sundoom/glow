/*
 *  Script.c
 *  Nova
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include "Script.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

#define LUA_DEBUG 
#ifdef LUA_DEBUG
#define LUA_STACK	const CHAR* _lua_stack;
#define LUA_WATCH(ls)	_lua_stack = __stack(ls);
#else
#define LUA_STACK
#define LUA_WATCH(ls)
#endif

//////////////////////////////////////////////////////////////////////////
enum LuaType
{
  LT_CLASS,
  LT_OBJECT,
  LT_RPC,
};

typedef struct _LuaClass
{
  U8  type;
  I32	table_ref;
}LuaClass;

typedef struct _LuaObject
{
  U8  type;
  I32 class_table_ref;
}LuaObject;

typedef struct _LuaRpc
{
  U8  type;
  I32 id;
}LuaRpc;

//////////////////////////////////////////////////////////////////////////

static lua_State* s_lua_state = NULL;

//////////////////////////////////////////////////////////////////////////

static const CHAR*	__stack(lua_State* ls);

static I32	__class(lua_State *ls);
static I32	__base_class(lua_State *ls);

static I32	__class_call(lua_State *ls);
static I32	__class_index(lua_State *ls);
static I32	__class_newindex(lua_State *ls);

static I32	__object_index(lua_State *ls);
static I32	__object_newindex(lua_State *ls);
static I32	__object_gc(lua_State *ls);

static I32	__print(lua_State* ls);
static I32	__error(lua_State* ls);

static I32	__copy_class_table(lua_State* ls);

static I32	__timer_create(lua_State* ls);
static I32	__timer_destroy(lua_State* ls);

static I32	__rpc_create(lua_State *ls);
static I32	__rpc_index(lua_State *ls);
static I32	__rpc_call(lua_State *ls);

static VOID	__push_value(lua_State* ls, const U8* data, U16* pos);
static VOID	__pop_value(lua_State* ls, I32 idx, U8* data, U16* pos);
static VOID	__push_table(lua_State* ls, const U8* data, U16* pos);
static VOID	__pop_table(lua_State* ls, I32 idx, U8* data, U16* pos);

//////////////////////////////////////////////////////////////////////////

/*
====================
ScriptInit
====================
*/
VOID ScriptInit()
{
  lua_State* ls;
  LUA_STACK;

  CHECK(s_lua_state==NULL);

  // openg lua
  ls = lua_open(); 
  CHECK(ls!=NULL);
  luaL_openlibs(ls);
  s_lua_state = ls;

  // create the class closure
  lua_pushstring(ls, "class");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __class, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);

  // the new print function
  lua_pushstring(ls, "print");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __print, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);

  // the new error function
  lua_pushstring(ls, "error");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __error, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);

  // the copy table function
  lua_pushstring(ls, "copy_class_table");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __copy_class_table, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);

  // the timer create function
  lua_pushstring(ls, "timer_create");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __timer_create, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);

  // the timer destroy function
  lua_pushstring(ls, "timer_destroy");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __timer_destroy, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);

  // the rpc create function
  lua_pushstring(ls, "rpc_create");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __rpc_create, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);
}

/*
====================
ScriptExit
====================
*/
VOID __clear_time_list();
VOID ScriptExit()
{
  if(s_lua_state) { lua_close(s_lua_state); s_lua_state = NULL; }
  __clear_time_list();
}

/*
====================
ScriptLoad
====================
*/
BOOL ScriptLoad(const CHAR* file_name)
{
  lua_State* ls;
  LUA_STACK;

  ls = (lua_State*)s_lua_state;
  CHECK(ls);
  LUA_WATCH(ls);
  if(luaL_loadfile(ls, file_name) == 0)
  {
    LUA_WATCH(ls);
    if(lua_pcall(ls, 0, LUA_MULTRET, 0) == 0)
    {
      LUA_WATCH(ls);
      return TRUE;
    }
  }

  LUA_WATCH(ls);
  __error(ls);

  return FALSE;
}

/*
====================
ScriptExec
====================
*/
BOOL ScriptExec(const CHAR* name, const CHAR* string)
{
  lua_State* ls;
  LUA_STACK;

  ls = (lua_State*)s_lua_state;
  CHECK(ls);
  LUA_WATCH(ls);
  if(luaL_loadbuffer(ls, string, strlen(string), name) == 0)
  {
    LUA_WATCH(ls);
    if(lua_pcall(ls, 0, LUA_MULTRET, 0) == 0)
    {
      LUA_WATCH(ls);
      return TRUE;
    }
  }

  LUA_WATCH(ls);
  __error(ls);

  return FALSE;
}

/*
====================
ScriptFunc
====================
*/
VOID ScriptFunc(const CHAR* name, lua_CFunction func)
{
  lua_State* ls;
  LUA_STACK;

  ls = (lua_State*)s_lua_state;
  CHECK(ls);
  LUA_WATCH(ls);

  // check the function if it has been registered?
  lua_pushstring(ls, name);
  LUA_WATCH(ls);
  lua_gettable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);
  if(!lua_isnil(ls, -1))
  {
    lua_pushstring(ls, VA("The C function(%s) had been registered!\n",lua_tostring(ls, 1)));
    __error(ls);
  }
  lua_pop(ls, 1);
  LUA_WATCH(ls);

  // register the new function
  lua_pushstring(ls, name);
  LUA_WATCH(ls);
  lua_pushcclosure(ls, (lua_CFunction)func, 0);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);
}

/*
====================
ScriptCall
====================
*/
BOOL ScriptCall(const CHAR* name, const U8* data, U16 size)
{
  lua_State* ls;
  I32 argc = 0;
  I32 i;
  U16 pos = 0;
  LUA_STACK;

  ls = s_lua_state; 
  CHECK(ls);
  LUA_WATCH(ls);

  // check the function
  lua_pushstring(ls, name);
  LUA_WATCH(ls);
  lua_gettable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);
  if(!lua_isfunction(ls, -1))
  {
    lua_pushstring(ls, VA("ScriptCall : can`t find the function : %s.\n", name));
    __error(ls);
    return FALSE;
  }

  // push the param
  argc = *((U16*)(data+pos));
  pos += sizeof(U16);
  for(i = 0; i < argc; i++)
  {
    __push_value(ls, data, &pos);
  }
  CHECK(pos == size);

  // call the function
  if(lua_pcall(ls, argc, 0, 0) != 0)
  {
    LUA_WATCH(ls);
    return FALSE;
  }
  LUA_WATCH(ls);

  return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
====================
__class
====================
*/
I32 __class(lua_State *ls)
{
  LuaClass* lua_class;

  LUA_STACK;
  LUA_WATCH(ls);

  // check the class
  if(lua_gettop(ls) != 1 || lua_type(ls, 1) != LUA_TSTRING || lua_isnumber(ls, 1))
  {
    lua_pushstring(ls, "__class : Invalid construct, expected class name!\n");
    __error(ls);
  }

  if(strlen(lua_tostring(ls, 1)) != lua_strlen(ls, 1))
  {
    lua_pushstring(ls, "__class : Does not support class names with extra nulls!\n");
    __error(ls);
  }

  // check the class name
  lua_pushvalue(ls, 1);
  LUA_WATCH(ls);
  lua_gettable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);
  if(!lua_isnil(ls, -1))
  {
    lua_pushstring(ls, VA("__class :  the class(%s) had been registered!\n",lua_tostring(ls, 1)));
    __error(ls);
  }
  lua_pop(ls, 1);
  LUA_WATCH(ls);

  // create the class data
  lua_class = (LuaClass*)(lua_newuserdata(ls, sizeof(LuaClass)));
  LUA_WATCH(ls);
  lua_class->type = LT_CLASS;

  // create the metatable for the lua class
  lua_newtable(ls);
  LUA_WATCH(ls);
  lua_pushvalue(ls, -1);
  LUA_WATCH(ls);
  lua_class->table_ref = luaL_ref(ls, LUA_REGISTRYINDEX);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__call");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __class_call, 0);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__index");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __class_index, 0);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__newindex");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __class_newindex, 0);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_setmetatable(ls, -2);
  LUA_WATCH(ls);

  // make the class globally available
  lua_pushvalue(ls, -2);
  LUA_WATCH(ls);
  lua_pushvalue(ls, -2);
  LUA_WATCH(ls);
  lua_settable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);

  // also add it to the closure as return value
  lua_pushcclosure(ls, &__base_class, 1);
  LUA_WATCH(ls);

  return 1;
}

/*
====================
__base_class
====================
*/
I32 __base_class(lua_State *ls)
{
  LuaClass *lua_class;
  LuaClass *lua_base_class;

  LUA_STACK;
  LUA_WATCH(ls);

  // get the lua class
  lua_class = (LuaClass*)(lua_touserdata(ls, lua_upvalueindex(1))); 
  CHECK(lua_class && lua_class->type==LT_CLASS);
  LUA_WATCH(ls);		

  // get the base class
  lua_base_class = (LuaClass*)(lua_touserdata(ls, 1)); 
  CHECK(lua_base_class && lua_base_class->type==LT_CLASS);
  LUA_WATCH(ls);

  // get the metatable of the class
  lua_getmetatable(ls, lua_upvalueindex(1));
  LUA_WATCH(ls);

  // get the metatable of the base class
  lua_getmetatable(ls, 1);
  LUA_WATCH(ls);

  // copy base class members
  lua_pushnil(ls);
  LUA_WATCH(ls);
  while(lua_next(ls, -2))
  {
    LUA_WATCH(ls);
    lua_pushvalue(ls, -2);
    LUA_WATCH(ls);
    lua_insert(ls, -2);
    LUA_WATCH(ls);
    lua_rawset(ls, -5);
    LUA_WATCH(ls);
  }

  return 0;
}

/*
====================
__class_call
====================
*/
I32 __class_call(lua_State *ls)
{
  LuaClass* lua_class;
  LuaObject* lua_object;
  I32 nargs;

  LUA_STACK;
  LUA_WATCH(ls);

  // get the class
  lua_class = (LuaClass*)(lua_touserdata(ls, 1)); 
  CHECK(lua_class && lua_class->type==LT_CLASS);
  LUA_WATCH(ls);

  // get the number of the args
  nargs = lua_gettop(ls);
  LUA_WATCH(ls);

  // alloc a memory for the lua object
  lua_object = (LuaObject*)(lua_newuserdata(ls, sizeof(LuaObject)));
  CHECK(lua_object);
  LUA_WATCH(ls);
  lua_object->type = LT_OBJECT;
  lua_object->class_table_ref = lua_class->table_ref;

  // create the metatable for the lua object
  lua_newtable(ls);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__index");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __object_index, 0);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__newindex");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __object_newindex, 0);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__gc");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __object_gc, 0);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_setmetatable(ls, -2);
  LUA_WATCH(ls);

  lua_pushvalue(ls, -1);
  LUA_WATCH(ls);
  lua_replace(ls, 1);
  LUA_WATCH(ls);

  // call the __init function
  lua_rawgeti(ls, LUA_REGISTRYINDEX, lua_object->class_table_ref);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__init");
  LUA_WATCH(ls);
  lua_rawget(ls, -2);
  LUA_WATCH(ls);
  lua_remove(ls, -2);
  LUA_WATCH(ls);
  if(lua_isfunction(ls, -1)) 
  {
    lua_insert(ls, 1);
    LUA_WATCH(ls);			
    lua_insert(ls, 1);
    LUA_WATCH(ls);
    lua_call(ls, nargs, 0);
    LUA_WATCH(ls);
  }
  else
  { 
    lua_pop(ls, 1);
    LUA_WATCH(ls);
  }
  
  return 1;
}

/*
====================
__class_index
====================
*/
I32 __class_index(lua_State *ls)
{
  LuaClass* lua_class;

  LUA_STACK;
  LUA_WATCH(ls);

  // get the class
  lua_class = (LuaClass*)(lua_touserdata(ls, 1)); 
  CHECK(lua_class && lua_class->type==LT_CLASS);
  LUA_WATCH(ls);

  // look in the member function from the class table		
  lua_getmetatable(ls, 1);
  LUA_WATCH(ls);
  lua_pushvalue(ls, 2);
  LUA_WATCH(ls);
  lua_rawget(ls, -2);
  LUA_WATCH(ls);
  if(lua_isfunction(ls, -1))
  {
    return 1;
  }

  // can`t find the member function
  lua_pushstring(ls, VA("__class_index : can`t find the member function(%s) from the class.\n", lua_tostring(ls, 2))); 
  __error(ls);

  return 0;
}

/*
====================
__class_newindex
====================
*/
I32 __class_newindex(lua_State *ls)
{
  LUA_STACK;
  LUA_WATCH(ls);

  // check the member if it is a function?
  if(lua_isfunction(ls, -1))
  {
    // get the table of the class and add the new member function to it
    LuaClass* lua_class = (LuaClass*)(lua_touserdata(ls, 1)); 
    CHECK(lua_class && lua_class->type==LT_CLASS);
    lua_getmetatable(ls, 1);
    LUA_WATCH(ls);
    lua_replace(ls, 1);
    LUA_WATCH(ls);
    lua_rawset(ls, -3);
    LUA_WATCH(ls);
  }
  else
  {
    lua_pushstring(ls, VA("__class_newindex : the member(%s) isn`t a function.\n", lua_tostring(ls, 2))); 
    __error(ls);
  }

  return 0;
}

/*
====================
__object_index
====================
*/
I32 __object_index(lua_State *ls)
{
  LuaObject* lua_object;
  const CHAR* key;

  LUA_STACK;
  LUA_WATCH(ls);

  // if key is nil, return nil
  if(lua_isnil(ls, 2))
  {
    lua_pushnil(ls);
    LUA_WATCH(ls);
    return 1;
  }

  // get the object
  lua_object = (LuaObject*)(lua_touserdata(ls, 1)); 
  CHECK(lua_object && lua_object->type==LT_OBJECT);

  // we have to ignore the first argument since this may point to
  // a method that is not present in this class (but in a subclass)
  key = lua_tostring(ls, 2);
  if(strlen(key) != lua_strlen(ls, 2)) 
  { 
    lua_pushstring(ls, VA("__object_index : don`t support keys with embedded nulls : '%s'.\n",lua_tostring(ls, 2)) );
    __error(ls);
  }

  // first, look in the object's metatable..
  lua_getmetatable(ls,1);
  LUA_WATCH(ls);
  lua_pushvalue(ls, 2);
  LUA_WATCH(ls);
  lua_rawget(ls, -2);
  LUA_WATCH(ls);

  // if it's not found in the instance, look in the class table.
  if(lua_isnil(ls, -1))
  {
    lua_pop(ls, 1);
    LUA_WATCH(ls);
    lua_rawgeti(ls, LUA_REGISTRYINDEX, lua_object->class_table_ref);
    LUA_WATCH(ls);
    lua_pushvalue(ls, 2);
    LUA_WATCH(ls);
    lua_rawget(ls, -2);
    LUA_WATCH(ls);
  }

  return 1;
}

/*
====================
__object_newindex
====================
*/
I32 __object_newindex(lua_State *ls)
{
  LuaObject* lua_object;
  const CHAR* key;

  LUA_STACK;
  LUA_WATCH(ls);

  // if the key is 'nil' fail
  if(lua_isnil(ls, 2))
  { 
    lua_pushstring(ls, "__object_newindex : the key is nil!\n");
    __error(ls);
  }

  // get the object
  lua_object = (LuaObject*)(lua_touserdata(ls, 1)); 
  CHECK(lua_object && lua_object->type==LT_OBJECT);

  // we have to ignore the first argument since this may point to
  // a method that is not present in this class (but in a subclass)
  key = lua_tostring(ls, 2);
  if(strlen(key) != lua_strlen(ls, 2)) 
  { 
    lua_pushstring(ls, VA("__object_newindex : don`t support keys with embedded nulls : '%s'.\n",lua_tostring(ls, 2)) );
    __error(ls);
  }

  // get the member from the meta table
  lua_getmetatable(ls,1);
  LUA_WATCH(ls);
  lua_replace(ls, 1);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);

  return 0;
}

/*
====================
__object_gc
====================
*/
I32 __object_gc(lua_State *ls)
{
  LuaObject* lua_object;

  LUA_STACK;
  LUA_WATCH(ls);

  lua_object = (LuaObject*)(lua_touserdata(ls, 1)); 
  CHECK(lua_object && lua_object->type==LT_OBJECT);

  // call the __exit function
  lua_rawgeti(ls, LUA_REGISTRYINDEX, lua_object->class_table_ref);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__exit");
  LUA_WATCH(ls);
  lua_rawget(ls, -2);
  LUA_WATCH(ls);
  lua_remove(ls, -2);
  LUA_WATCH(ls);
  if(lua_isfunction(ls, -1)) 
  {
    lua_pushvalue(ls, -2);
    LUA_WATCH(ls);
    lua_call(ls, 1, 0);
    LUA_WATCH(ls);
  }

  return 0;
}

/*
====================
__print
====================
*/
I32	__print(lua_State* ls)
{
	I32 n = lua_gettop(ls);
	I32 i;
	lua_getglobal(ls, "tostring");
	for (i = 1; i <= n; i++) 
	{
		const CHAR *s;
		lua_pushvalue(ls, -1);
		lua_pushvalue(ls, i);
		lua_call(ls, 1, 1);
		s = lua_tostring(ls, -1);
		if (s == NULL)
			return luaL_error(ls, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
		if (i > 1)
      GPrint("\t");
		GPrint(s);
		lua_pop(ls, 1);
	}
	GPrint("\n");
  return 0;
}

/*
====================
__error
====================
*/
I32	__error(lua_State* ls)
{
  LUA_STACK;
  LUA_WATCH(ls);
  luaL_where(ls, 1);
  LUA_WATCH(ls);
  if(lua_isstring(ls, -2)) 
  {
    lua_pushvalue(ls, -2);
    LUA_WATCH(ls);
    lua_concat(ls, 2);
    LUA_WATCH(ls);
  }
  GAssert(lua_tostring(ls, -1));
  return lua_error(ls);
}

//////////////////////////////////////////////////////////////////////////
/*
====================
__push_value
====================
*/
VOID __push_value(lua_State* ls, const U8* data, U16* pos)
{
  U8 type;
  LUA_STACK;
  LUA_WATCH(ls);

  type = *((U8*)(data+(*pos)));
  (*pos) += sizeof(U8);
  switch(type) 
  {
  case LUA_TNIL:
    {
      lua_pushnil(ls);
      LUA_WATCH(ls);
    }
    break;
  case LUA_TBOOLEAN:
    {
      I32 b = *((I32*)(data+(*pos)));
      (*pos) += sizeof(I32);
      lua_pushboolean(ls, b);
      LUA_WATCH(ls);
    }
    break;
  case LUA_TNUMBER:
    {
      LUA_NUMBER number = *((LUA_NUMBER*)(data+(*pos)));
      (*pos) += sizeof(LUA_NUMBER);
      lua_pushnumber(ls, number);
      LUA_WATCH(ls);
    }
    break;    
  case LUA_TSTRING:
    {
      static CHAR str[MAX_DATA];
      U16 len = 0;
      len = *((U16*)(data+(*pos)));
      (*pos) += sizeof(U16);
      memcpy(str, (data+(*pos)), len*sizeof(CHAR));
      (*pos) += len*sizeof(CHAR);
      str[len] = 0;
      lua_pushstring(ls, str);
      LUA_WATCH(ls);
    }
    break;
  case LUA_TTABLE:
    __push_table(ls, data, pos);
    LUA_WATCH(ls);
    break;
  default:
    lua_pushstring(ls, VA("__push_value : the arg type(%s) of the variable is not support.\n", type));
    __error(ls);
  }
}

/*
====================
__pop_value
====================
*/
VOID __pop_value(lua_State* ls, I32 idx, U8* data, U16* pos)
{
  I32 type;
  LUA_STACK;

  type = lua_type(ls, idx);
  LUA_WATCH(ls);
  switch(type)
  {
  case LUA_TNIL:
    {
      (*((U8*)(data+(*pos)))) = LUA_TNIL;
      (*pos) += sizeof(U8);
    }
    break;
  case LUA_TBOOLEAN:
    {
      (*((U8*)(data+(*pos)))) = LUA_TBOOLEAN;
      (*pos) += sizeof(U8);
      (*((I32*)(data+(*pos)))) = lua_toboolean(ls, idx);
      (*pos) += sizeof(I32);
    }
    break;
  case LUA_TNUMBER:
    {
      (*((U8*)(data+(*pos)))) = LUA_TNUMBER;
      (*pos) += sizeof(U8);
      (*((LUA_NUMBER*)(data+(*pos)))) = (LUA_NUMBER)lua_tonumber(ls, idx);
      (*pos) += sizeof(LUA_NUMBER);
    }
    break;
  case LUA_TSTRING:
    {
      const CHAR* str = NULL;
      U16 len = 0;
      (*((U8*)(data+(*pos)))) = LUA_TSTRING;
      (*pos) += sizeof(U8);
      str = (CHAR*)lua_tostring(ls, idx);
      len = strlen(str);
      (*((U16*)(data+(*pos)))) = len;
      (*pos) += sizeof(U16);
      memcpy(data+(*pos), str, len*sizeof(CHAR));
      (*pos) += len*sizeof(CHAR);
    }      
    break;
  case LUA_TTABLE:
      __pop_table(ls, idx, data, pos);
    break;
  default:
    lua_pushstring(ls, VA("__pop_value : the type(%s) of the variable is not support.\n",lua_typename(ls,idx)));
    __error(ls);
    break;
  }
  CHECK((*pos) < MAX_DATA);
}

/*
====================
__push_table
====================
*/
VOID __push_table(lua_State* ls, const U8* data, U16* pos)
{
  U16 count;
  I32 i;
  LUA_STACK;
  LUA_WATCH(ls);

  lua_newtable(ls);
  LUA_WATCH(ls);

  count = *((U16*)(data+(*pos)));
  (*pos) += sizeof(U16);
  for(i = 0; i < count; i++)
  {
    // push the key
    __push_value(ls, data, pos);
    // push the value
    __push_value(ls, data, pos);
    // set the table
    lua_rawset(ls, -3);
    LUA_WATCH(ls);
  }
  LUA_WATCH(ls);
}

/*
====================
__pop_table
====================
*/
VOID __pop_table(lua_State* ls, I32 idx, U8* data, U16* pos)
{
  U16* count;
  LUA_STACK;
  LUA_WATCH(ls);

  (*((U8*)(data+(*pos)))) = LUA_TTABLE;
  (*pos) += sizeof(U8);
  count = (U16*)(data+(*pos));
  (*pos) += sizeof(U16);
  *count = 0;

  LUA_WATCH(ls);
  lua_pushnil(ls);
  LUA_WATCH(ls);
  while(lua_next(ls, idx))
  {
    LUA_WATCH(ls);
    // pop the key
    __pop_value(ls, lua_gettop(ls)-1, data, pos);
    // pop the value
    __pop_value(ls, lua_gettop(ls), data, pos);
    lua_pop(ls, 1);
    LUA_WATCH(ls);
    (*count)++;
  }
  LUA_WATCH(ls);
}

//////////////////////////////////////////////////////////////////////////
/*
====================
__stack
====================
*/
const CHAR*	__stack(lua_State* ls)
{
  static CHAR buffer[2048];
  I32 i, j = 0;

  memset(buffer, 0, sizeof(buffer));
  for(i = 1; i <= lua_gettop(ls); i++)
  {
    I32 type = lua_type( ls, i );
    switch(type)
    {
    case LUA_TNONE:
      j += sprintf(buffer+j, "id=%d; type=NONE; value=none \n", i);
      break;
    case LUA_TNIL:
      j += sprintf(buffer+j, "id=%d; type=NIL; value=nil \n", i);
      break;
    case LUA_TBOOLEAN:
      if((lua_toboolean(ls, i)!=0))
        j += sprintf(buffer+j, "id=%d; type=BOOL; value=true \n", i);
      else
        j += sprintf(buffer+j, "id=%d; type=BOOL; value=false \n", i);
      break;
    case LUA_TLIGHTUSERDATA:
      j += sprintf(buffer+j, "id=%d; type=LIGHTUSERDATA; value=%X \n", i, lua_touserdata(ls, i));
      break;
    case LUA_TNUMBER:
      j += sprintf(buffer+j, "id=%d; type=NUMBER; value=%f \n", i, lua_tonumber(ls, i));
      break;
    case LUA_TSTRING:
      j += sprintf(buffer+j, "id=%d; type=STRING; value=%s \n", i, lua_tostring(ls, i));
      break;
    case LUA_TTABLE:
      j += sprintf(buffer+j, "id=%d; type=TABLE; value=%X \n", i, lua_topointer(ls, i));
      break;
    case LUA_TFUNCTION:
      j += sprintf(buffer+j, "id=%d; type=FUNCTION; value=%X \n", i, lua_topointer(ls, i));
      break;
    case LUA_TUSERDATA:
      j += sprintf(buffer+j, "id=%d; type=USERDATA; value=%X \n", i, lua_touserdata(ls, i));
      break;
    case LUA_TTHREAD:
      j += sprintf(buffer+j, "id=%d; type=THREAD; value=%X \n", i, lua_topointer(ls, i));
      break;
    default:
      break;
    }
  }

  return buffer;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
__copy_class_table
====================
*/
I32 __copy_class_table(lua_State *ls)
{
  LuaClass *lua_dst_class;
  LuaClass *lua_src_class;  

  LUA_STACK;
  LUA_WATCH(ls);

  // get the dst class
  lua_dst_class = (LuaClass*)(lua_touserdata(ls, 1)); 
  CHECK(lua_dst_class && lua_dst_class ->type==LT_CLASS);
  LUA_WATCH(ls);

  // get the src class
  lua_src_class = (LuaClass*)(lua_touserdata(ls, 2)); 
  CHECK(lua_src_class && lua_src_class->type==LT_CLASS);
  LUA_WATCH(ls);

  // get the metatable of the dst class
  lua_getmetatable(ls, 1);
  LUA_WATCH(ls);

  // get the metatable of the src class
  lua_getmetatable(ls, 2);
  LUA_WATCH(ls);

  // copy all of the members
  lua_pushnil(ls);
  LUA_WATCH(ls);
  while(lua_next(ls, -2))
  {
    LUA_WATCH(ls);
    lua_pushvalue(ls, -2);
    LUA_WATCH(ls);
    lua_insert(ls, -2);
    LUA_WATCH(ls);
    lua_rawset(ls, -5);
    LUA_WATCH(ls);
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////

typedef struct _TimerNode
{
  I32	table_ref;
  I32 argc;
  F64 interval;
  F64 time;
  struct _TimerNode* next;
}TimerNode;

static TimerNode* s_timer_list = NULL;

VOID __add_time_node(TimerNode* node)
{
  TimerNode* tail_node;
  if (s_timer_list == NULL)
  {
    s_timer_list = node;
  }
  else 
  {
    tail_node = s_timer_list;
    while (tail_node && tail_node->next)
    {
      tail_node = tail_node->next;
    }
    tail_node->next = node;
  }
}

VOID __del_time_node(I32 table_ref)
{
  TimerNode* prev_node = NULL;
  TimerNode* curr_node = s_timer_list;
  while (curr_node)
  {
    if (curr_node->table_ref == table_ref)
    {
      if (prev_node)
      {
        prev_node->next = curr_node->next;
      }
      else
      {
        s_timer_list = curr_node->next;
      }
      free(curr_node);
      return;
    }
    else
    {
      prev_node = curr_node;
      curr_node = curr_node->next;
    }
  }
}

VOID __clear_time_list()
{
  TimerNode* node = s_timer_list;
  
  while (node)
  {
    TimerNode* curr_node = node;
    node = node->next;
    if(curr_node) free(curr_node);
  }
}

/*
====================
__timer_create
====================
*/
I32 __timer_create(lua_State *ls)
{
  F64 interval = 0.0;
  I32 table_ref;
  I32 i, argc;
  CHAR arg_name[MAX_NAME];
  TimerNode* node;

  LUA_STACK;
  LUA_WATCH(ls);

  // get the timer interval time
  if(lua_type(ls, 1) != LUA_TNUMBER)
  {
    lua_pushstring(ls, "__timer_create : invalid expected time!\n");
    __error(ls);
  }
  interval = lua_tonumber(ls, 1);
  lua_remove(ls, 1);
  LUA_WATCH(ls);

  // get the timer function
  if(lua_type(ls, 1) != LUA_TFUNCTION)
  {
    lua_pushstring(ls, "__timer_create : invalid expected function!\n");
    __error(ls);
  }

  // create the new table
  lua_newtable(ls);
  LUA_WATCH(ls);

  // add the function to the table
  lua_pushstring(ls, "function");
  LUA_WATCH(ls);
  lua_pushvalue(ls, 1);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_remove(ls, 1);
  LUA_WATCH(ls);

  argc = lua_gettop(ls) - 1;

  // add the param to the table  
  for(i = 1; i <= argc; i++)
  {
    sprintf(arg_name, "arg%d", i);
    lua_pushstring(ls, arg_name);
    LUA_WATCH(ls);
    lua_pushvalue(ls, i);
    LUA_WATCH(ls);
    lua_rawset(ls, -3);
    LUA_WATCH(ls);
  }

  // register the table
  table_ref = luaL_ref(ls, LUA_REGISTRYINDEX);
  LUA_WATCH(ls);

  // return the timer id
  lua_pushnumber(ls, table_ref);
  LUA_WATCH(ls);

  // add the node to the list tail
  node = (TimerNode*)malloc(sizeof(TimerNode));
  node->table_ref = table_ref;
  node->argc = argc;
  node->interval = interval;
  node->time = GSeconds() + interval;
  node->next = NULL;
  __add_time_node(node);

  return 1;
}

/*
====================
__timer_destroy
====================
*/
I32 __timer_destroy(lua_State *ls)
{
  I32 table_ref; 

  LUA_STACK;
  LUA_WATCH(ls);

  // get the timer id
  if(lua_gettop(ls) != 1 || lua_type(ls, 1) != LUA_TNUMBER)
  {
    lua_pushstring(ls, "__timer_destroy : invalid expected id!\n");
    __error(ls);
  }
  table_ref = lua_tonumber(ls, 1);

  // unregister the table
  luaL_unref(ls, LUA_REGISTRYINDEX, table_ref);
  LUA_WATCH(ls);

  // remove it from the list
  __del_time_node(table_ref);

  return 0;
}

/*
====================
__timer_tick
====================
*/
VOID __timer_tick()
{
  lua_State* ls;
  I32 i;
  CHAR arg_name[MAX_NAME];  
  TimerNode* node = NULL;
  F64 sys_time = GSeconds();
  LUA_STACK;

  ls = s_lua_state;
  CHECK(ls);
  LUA_WATCH(ls);

  node = s_timer_list;
  while (node)
  {
    if (node->time <= sys_time)
    {
      // get the timer table
      lua_rawgeti(ls, LUA_REGISTRYINDEX, node->table_ref);
      LUA_WATCH(ls);

      // get the function
      lua_pushstring(ls, "function");
      LUA_WATCH(ls);
      lua_rawget(ls, 1);
      LUA_WATCH(ls);

      // get the params
      for(i = 1; i <= node->argc; i++)
      {
        sprintf(arg_name, "arg%d", i);
        lua_pushstring(ls, arg_name);
        LUA_WATCH(ls);
        lua_rawget(ls, 1);
        LUA_WATCH(ls);
      }

      // call the function
      if(lua_pcall(ls, node->argc, 0, 0) != 0)
      {
        lua_pushstring(ls, "__timer_tick : invalid expected call!\n");
        __error(ls);
      }
      LUA_WATCH(ls);

      // remove the timer from table
      luaL_unref(ls, LUA_REGISTRYINDEX, node->table_ref);
      LUA_WATCH(ls);

      // the next timer call
      node->time = sys_time + node->interval;
    }
    node = node->next;
  }
}

//////////////////////////////////////////////////////////////////////////

/*
====================
__rpc_create
====================
*/
I32 __rpc_create(lua_State *ls)
{
  I32 rpc_id = 0;
  LuaRpc* lua_rpc;

  LUA_STACK;
  LUA_WATCH(ls);

  // check the rpc id
  if(lua_gettop(ls) != 1 || lua_type(ls, 1) != LUA_TNUMBER)
  {
    lua_pushstring(ls, "__rpc_create : Invalid create, expected rpc id!\n");
    __error(ls);
  }
  rpc_id = lua_tonumber(ls, 1);

  // alloc a memory for the rpc object
  lua_rpc = (LuaRpc*)(lua_newuserdata(ls, sizeof(LuaRpc)));
  CHECK(lua_rpc);
  LUA_WATCH(ls);
  lua_rpc->type = LT_RPC;
  lua_rpc->id = rpc_id;

  // create the metatable for the lua object
  lua_newtable(ls);
  LUA_WATCH(ls);
  lua_pushstring(ls, "__index");
  LUA_WATCH(ls);
  lua_pushcclosure(ls, __rpc_index, 0);
  LUA_WATCH(ls);
  lua_rawset(ls, -3);
  LUA_WATCH(ls);
  lua_setmetatable(ls, -2);
  LUA_WATCH(ls);

  return 1;
}

/*
====================
__rpc_index
====================
*/
I32 __rpc_index(lua_State *ls)
{
  LuaRpc* lua_rpc;
  const CHAR* key;

  LUA_STACK;
  LUA_WATCH(ls);

  // if key is nil, return nil
  if(lua_isnil(ls, 2))
  {
    lua_pushnil(ls);
    LUA_WATCH(ls);
    return 1;
  }

  // get the object
  lua_rpc = (LuaRpc*)(lua_touserdata(ls, 1)); 
  CHECK(lua_rpc && lua_rpc->type==LT_RPC);

  // we have to ignore the first argument since this may point to
  // a method that is not present in this class (but in a subclass)
  key = lua_tostring(ls, 2);
  if(strlen(key) != lua_strlen(ls, 2)) 
  { 
    lua_pushstring(ls, VA("__object_index : don`t support keys with embedded nulls : '%s'.\n",lua_tostring(ls, 2)) );
    __error(ls);
  }

  // also add rpc call to the closure as return value
  lua_pushcclosure(ls, &__rpc_call, 2);
  LUA_WATCH(ls);

  return 1;
}

/*
====================
__rpc_call
====================
*/
VOID (*rpc_call)(const RPC_CALL* rc) = NULL;
I32 __rpc_call(lua_State *ls)
{
  LuaRpc* lua_rpc;
  const CHAR* func_name;

  I32 i;
  RPC_CALL rc;
  U16 size = 0;
  U8* data = &rc.data[0];
  U16* count;
  U16 len = 0;

  LUA_STACK;
  LUA_WATCH(ls);
  
  // get the rpc object
  lua_rpc = (LuaRpc*)(lua_touserdata(ls, lua_upvalueindex(1))); 
  CHECK(lua_rpc && lua_rpc->type==LT_RPC);

  // get the function name
  func_name = (const CHAR*)lua_tostring(ls, lua_upvalueindex(2));
  CHECK(func_name && func_name != "");

  // set call protocol
  // rc.type = CPT_CALL;

  // get the server id
  rc.sid = lua_rpc->id;

  // add the function name
  len = strlen(func_name);
  CHECK(len < MAX_NAME);
  (*((U16*)(data+size))) = len;
  size += sizeof(U16);
  memcpy(data+size, func_name, len*sizeof(CHAR));
  size += len*sizeof(CHAR);

  // set the count
  count = (U16*)(data+size);
  size += sizeof(U16);
  *count = 0;

  // pop the arg of the function from the lua
  for(i = 1; i <= lua_gettop(ls); i++)
  {
    __pop_value(ls, i, data, &size);
    (*count)++;
  }
  rc.size = size;

  // call rpc function
  if (rpc_call != NULL)
  {
    rpc_call(&rc);
  }

  // test the rpc event
  // rpc_event(&rc);

  return 0;
}

/*
====================
rpc_event
====================
*/
BOOL rpc_event(const RPC_CALL* rc)
{
  lua_State* ls;
  I32 argc = 0;
  I32 i;
  const U8* data = &rc->data[0];
  U16 pos = 0;
  CHAR func_name[MAX_NAME];
  U16 len = 0;

  LUA_STACK;
  ls = s_lua_state;
  CHECK(ls);
  LUA_WATCH(ls);

  // get the rpc functon name
  len = *((U16*)(data+pos));
  CHECK(len < MAX_NAME);
  pos += sizeof(U16); 
  memcpy(func_name, data+pos, len*sizeof(CHAR));
  pos += len*sizeof(CHAR);
  func_name[len] = 0;

  // check the function
  lua_pushstring(ls, func_name);
  LUA_WATCH(ls);
  lua_gettable(ls, LUA_GLOBALSINDEX);
  LUA_WATCH(ls);
  if(!lua_isfunction(ls, -1))
  {
    lua_pushstring(ls, VA("rpc_event : can`t find the rpc function : %s.\n", func_name));
    __error(ls);
    return FALSE;
  }

  // get the param count
  argc = *((U16*)(data+pos));
  pos += sizeof(U16);

  // push the param
  for(i = 0; i < argc; i++)
  {
    __push_value(ls, data, &pos);
  }
  CHECK(pos == rc->size);

  // call the function
  if(lua_pcall(ls, argc, 0, 0) != 0)
  {
    LUA_WATCH(ls);
    return FALSE;
  }
  LUA_WATCH(ls);

  return TRUE;
}

//////////////////////////////////////////////////////////////////////////