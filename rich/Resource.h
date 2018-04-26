/*
 *  Resource.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Resource__
#define __Resource__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glow.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented Resource interface.  
	class Resource : public GReference
	{
  protected:
		// constructor
		Resource(const CHAR* name);
		
		// destructor
		virtual ~Resource();
  
		// load the data
		virtual VOID Load(VOID* data, U32 size) = 0;

  private:
    Str mName;

  public:
    template<typename T> 
    static T* Find(const CHAR* name)
    {
      CHECK(name);
      std::map<Str, Resource*>::iterator it = s_res_map.find(name);
      if(it != s_res_map.end()) return dynamic_cast<T*>(it->second);
      T*res = GNEW(T(name));
      GDataPtr data = (GData*)GLoad(name);      
      res->Load(data->Ptr(), data->Size());
      return res;
    }
  private:		
    static VOID Add(const CHAR* name, Resource* res);
    static VOID Remove(const CHAR* name);
  private:
    static std::map<Str, Resource*>s_res_map;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Resource__

//////////////////////////////////////////////////////////////////////////