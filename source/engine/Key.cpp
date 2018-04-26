//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	Key::Key(const CHAR*name, GReference* ptr)
	{
		CHECK(name&&ptr);
		mName = name;
		mPtr = ptr;				
		Key::Add(name, this);
	}

	Key::~Key()
	{
		if(mPtr) mPtr.Release();
		Key::Remove(mName.c_str());
	}

	/*
	====================
	Name
	====================
	*/
	const CHAR* Key::Name()
	{
		GUARD(Key::Name);

		return mName.c_str();

		UNGUARD;
	}

	/*
	====================
	ptr
	====================
	*/
	GReference* Key::Ptr()
	{
		GUARD(Key::ptr);

		return mPtr.Ptr();

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	static std::map<Str, Key*>s_key_map;

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	find
	====================
	*/
	Key* Key::Find(const CHAR* name)
	{
		GUARD(Key::Find);

		CHECK(name);
		std::map<Str, Key*>::iterator it = s_key_map.find(name);
		if(it == s_key_map.end()) return NULL;
		return it->second;

		UNGUARD;
	}

	/*
	====================
	add
	====================
	*/
	VOID Key::Add(const CHAR* name, const Key* key)
	{
		GUARD(Key::Add);

		CHECK(name&&key);
		if(s_key_map.find(name) != s_key_map.end()) GAssert(VA("The key(%s) is existent already!", name));
		s_key_map.insert(std::make_pair(name, (Key*)key));

		UNGUARD;
	}

	/*
	====================
	remove
	====================
	*/	
	VOID Key::Remove(const CHAR* name)
	{
		GUARD(Key::Remove);

		CHECK(name);
		std::map<Str, Key*>::iterator it = s_key_map.find(name);
		if(it == s_key_map.end()) GAssert(VA("Can`t find the key : %s.", name));
		s_key_map.erase(it);

		UNGUARD;
	}
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

