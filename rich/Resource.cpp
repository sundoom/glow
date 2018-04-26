//////////////////////////////////////////////////////////////////////////

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

//////////////////////////////////////////////////////////////////////////  
  
	Resource::Resource(const CHAR* name)
	{
    CHECK(name);
    mName = name;
//    GDataPtr data_ptr = (GData*)GLoad(name);
//    CHECK(data_ptr);
//    this->Load(data_ptr->Ptr(), data_ptr->Size());
//    this->Load(NULL, 0);
    Add(name, this);
	}

  Resource::~Resource()
	{
    Remove(mName.c_str());
	}



  //////////////////////////////////////////////////////////////////////////

  std::map<Str, Resource*> Resource::s_res_map;

  //////////////////////////////////////////////////////////////////////////
  /*
	====================
	find
	====================
	*/
/*  template<typename T> 
  T* Resource::Find(const CHAR* name)
  {
    CHECK(name);
    std::map<Str, Resource*>::iterator it = s_res_map.find(name);
    if(it != s_res_map.end()) return dynamic_cast<T*>(it->second);
    T*res = GNEW(T(name));
    res->Load(NULL, 0);
    return res;
  }
  */
/*  
  template<typename T>
	T* Resource::Find(const CHAR* name)
	{
		CHECK(name);
		std::map<Str, T*>::iterator it = s_res_map.find(name);
		if(it != s_res_map.end()) return dynamic_cast<T*>(it->second);
    return GNEW(T(name));
	}
  */
 
	/*
	====================
	add
	====================
	*/  
	VOID Resource::Add(const CHAR* name, Resource* res)
	{
    CHECK(name);
    if(s_res_map.find(name) != s_res_map.end()) GAssert(VA("The resource(%s) is existent already!", name));
    s_res_map.insert(std::make_pair(name, res));
	}

	/*
	====================
	remove
	====================
	*/
	VOID Resource::Remove(const CHAR* name)
	{
		CHECK(name);
    std::map<Str, Resource*>::iterator it = s_res_map.find(name);
    if(it == s_res_map.end()) GAssert(VA("Can`t find the resource : %s.", name));
    s_res_map.erase(it);
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
