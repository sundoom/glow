/*
 *  GPtr.h
 *  glow
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __GPtr__
#define __GPtr__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glow.h>

//////////////////////////////////////////////////////////////////////////
// smart pointer for handling referenced counted objects.
template<class T>
class GPtr
{
private: 
	typedef T* GPtr::*bool_type;
	T* mPtr;

public: 
	typedef T element_type;

	GPtr() : mPtr(0)
	{
		GUARD(GPtr());

		UNGUARD;
	}

	GPtr(T* ptr) : mPtr(ptr) 
	{
		GUARD(GPtr(T* ptr));

		if(mPtr) mPtr->IncRef();

		UNGUARD;
	}

	GPtr(const GPtr& rp) : mPtr(rp.mPtr) 
	{
		GUARD(GPtr(const GPtr& rp));

		if(mPtr) mPtr->IncRef(); 

		UNGUARD;
	}

	template<class O> 
	GPtr(const GPtr<O>& rp) : mPtr(rp.mPtr) 
	{ 
		GUARD(GPtr(const GPtr<O>& rp));

		if(mPtr) mPtr->IncRef(); 

		UNGUARD;
	}

	~GPtr() 
	{
		GUARD(~GPtr);

		if(mPtr) mPtr->DecRef();  
		mPtr = 0; 

		UNGUARD;
	}

	GPtr& operator = (const GPtr& rp)
	{
		GUARD(operator = (const GPtr& rp));

		if(mPtr == rp.mPtr) return *this;
		T* tmp_ptr = mPtr;
		mPtr = rp.mPtr;
		if(mPtr) mPtr->IncRef();
		if(tmp_ptr) tmp_ptr->DecRef();
		return *this;

		UNGUARD;
	}

	template<class O> 
	GPtr& operator = (const GPtr<O>& rp)
	{
		GUARD(operator = (const GPtr<O>& rp));

		if(mPtr == rp.mPtr) return *this;
		T* tmp_ptr = mPtr;
		mPtr = rp.mPtr;
		if(mPtr) mPtr->IncRef();
		if(tmp_ptr) tmp_ptr->DecRef();
		return *this;

		UNGUARD;
	}

	GPtr& operator = (T* ptr)
	{
		GUARD(operator = (T* ptr));

		if(mPtr == ptr) return *this;
		T* tmp_ptr = mPtr;
		mPtr = ptr;
		if(mPtr) mPtr->IncRef();
		if(tmp_ptr) tmp_ptr->DecRef();
		return *this;

		UNGUARD;
	}

	BOOL operator == (const GPtr& rp) const 
	{
		GUARD(operator == (const GPtr& rp));

		return (mPtr == rp.mPtr) ? TRUE : FALSE;

		UNGUARD;
	}

	BOOL operator == (const T* ptr) const 
	{
		GUARD(operator == (const T* ptr));

		return (mPtr == ptr) ? TRUE : FALSE;

		UNGUARD;
	}

	friend BOOL operator == (const T* ptr, const GPtr& rp) 
	{
		GUARD(operator == (const T* ptr, const GPtr& rp));

		return (ptr == rp.mPtr) ? TRUE : FALSE; 

		UNGUARD;
	}

	BOOL operator != (const GPtr& rp) const 
	{
		GUARD(operator != (const GPtr& rp));

		return (mPtr != rp.mPtr) ? TRUE : FALSE; 

		UNGUARD;
	}

	BOOL operator != (const T* ptr) const 
	{
		GUARD(operator != (const T* ptr));

		return (mPtr != ptr) ? TRUE : FALSE; 

		UNGUARD;
	}

	friend BOOL operator != (const T* ptr, const GPtr& rp) 
	{
		GUARD(operator != (const T* ptr, const GPtr& rp));

		return ( ptr != rp.mPtr ) ? TRUE : FALSE; 

		UNGUARD;
	}

	T& operator*() const 
	{
		GUARD(operator*());

		return *mPtr; 

		UNGUARD;
	}

	T* operator->() const 
	{
		GUARD(operator->());

		return mPtr; 

		UNGUARD;
	}

	T* Ptr() const 
	{
		GUARD(Ptr());

		return mPtr; 

		UNGUARD;
	}

	BOOL operator!() const
	{
		GUARD(operator!());

		return mPtr == 0 ? TRUE : FALSE;

		UNGUARD;
	} 

	BOOL Valid() const       
	{
		GUARD(valid());

		return mPtr != 0 ? TRUE : FALSE;

		UNGUARD;
	}

	VOID Release() 
	{
		GUARD(Release());

		if(mPtr) mPtr->DecRef(); 
		mPtr = 0;

		UNGUARD;
	}

	operator bool_type() const 
	{ 
		GUARD(operator bool_type());

		return mPtr != 0 ? &GPtr::mPtr : 0; 

		UNGUARD;
	}
};

//////////////////////////////////////////////////////////////////////////

template<class T, class Y> GPtr<T> static_ptr_cast(const GPtr<Y>& rp) 
{ 
	GUARD(static_ptr_cast);

	return static_cast<T*>(rp.Ptr()); 

	UNGUARD;
}

template<class T, class Y> GPtr<T> dynamic_ptr_cast(const GPtr<Y>& rp) 
{ 
	GUARD(dynamic_ptr_cast);

	return dynamic_cast<T*>(rp.Ptr()); 

	UNGUARD;
}

template<class T, class Y> GPtr<T> const_ptr_cast(const GPtr<Y>& rp) 
{ 
	GUARD(const_ptr_cast);

	return const_cast<T*>(rp.Ptr()); 

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GPtr__

//////////////////////////////////////////////////////////////////////////