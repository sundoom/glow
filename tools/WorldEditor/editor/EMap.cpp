//////////////////////////////////////////////////////////////////////////

#include "Editor.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

EMap::EMap()
{

}

EMap::~EMap()
{

}

//////////////////////////////////////////////////////////////////////////	

/*
====================
Load
====================
*/
VOID EMap::Load(const GData* data)
{
	GUARD(Map::Load);

	CHECK(data);
	U8*data_ptr = (U8*)data->Ptr();

	// check the map header
	CHECK(*(U32*)data_ptr == (MAKEFOURCC('G','M','A','P')));
	data_ptr += sizeof(U32);

	// get the stride of the map
	mWidth = *(U32*)data_ptr;
	data_ptr += sizeof(U32);
	mHeight = *(U32*)data_ptr;
	data_ptr += sizeof(U32);

	// load all of the chunks
	for(U32 j = 0; j < mHeight; j++)
	{
		for(U32 i = 0; i < mWidth; i++)
		{
			ChunkPtr chunk = GNEW(EChunk); CHECK(chunk);
			chunk->Load(data_ptr);
			mChunks.push_back(chunk);
		}
	}

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////