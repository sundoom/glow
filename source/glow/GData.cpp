/*
 *  GData.cpp
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

GData::GData()
{
	GUARD(GData::GData);

	UNGUARD;
}

GData::GData(U32 size)
{
	GUARD(GData::GData);

	mData.resize(size);

	UNGUARD;
}

GData::GData(U32 size, U8 value)
{
	GUARD(GData::GData);

	mData.resize(size, value);

	UNGUARD;
}

GData::~GData()
{
	GUARD(GData::~GData);

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Size
====================
*/
VOID GData::Size(U32 size)
{
	GUARD(GData::Size);

	mData.resize(size);

	UNGUARD;
}

VOID GData::Size(U32 size, U8 value)
{
	GUARD(GData::Size);

	mData.resize(size, value);

	UNGUARD;
}

U32 GData::Size() const
{
	GUARD(GData::Size);

	return mData.size();

	UNGUARD;
}

/*
====================
Ptr
====================
*/
VOID* GData::Ptr() const
{
	GUARD(GData::Ptr);

	if(mData.size()) return (VOID*)&mData[0];
	return NULL;

	UNGUARD;
}
	
//////////////////////////////////////////////////////////////////////////
