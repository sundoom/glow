//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	Map::Map()
	{
		mWidth = mHeight = 0;
	}
	
	Map::~Map()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID Map::Load(const GData* data)
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
		for(I32 j = 0; j < mHeight; j++)
		{
			for(I32 i = -mWidth/2; i < mWidth/2; i++)
			{
				std::pair<I32,I32> coord(i,j);
				ChunkPtr chunk = GNEW(Chunk); CHECK(chunk);
				chunk->Load(data_ptr);
				// mChunks.push_back(chunk);
				mChunks.insert(std::make_pair(coord,chunk));
			}
		}

		UNGUARD;
	}

	/*
	====================
	update
	====================
	*/
	VOID Map::Update(const Vector4& viewport)
	{
		GUARD(Map::Update);

		Rect src, dst, clip;

		// compute the view
	/*	src.left	= U2P(viewport[0])/U2P(CHUNK_STRIDE);
		src.top		= U2P(viewport[1])/U2P(CHUNK_STRIDE);
		src.right	= (U2P(viewport[2])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[2])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[2])/U2P(CHUNK_STRIDE);
		src.bottom	= (U2P(viewport[3])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[3])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[3])/U2P(CHUNK_STRIDE);
	*/	
		src.left	= (U2P(viewport[0])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[0])/U2P(CHUNK_STRIDE)-1) : U2P(viewport[0])/U2P(CHUNK_STRIDE);
		src.top		= (U2P(viewport[1])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[1])/U2P(CHUNK_STRIDE)-1) : U2P(viewport[1])/U2P(CHUNK_STRIDE);
		src.right	= (U2P(viewport[2])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[2])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[2])/U2P(CHUNK_STRIDE);
		src.bottom	= (U2P(viewport[3])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[3])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[3])/U2P(CHUNK_STRIDE);

		dst.left	= -mWidth/2;
		dst.top		= 0;
		dst.right	= mWidth/2;
		dst.bottom	= mHeight;	
		
		IntersectRect(&clip, &dst, &src);

		// update all of the chunk		
		if(mViewport[0]!=clip.left || mViewport[1]!=clip.top || mViewport[2]!=clip.right || mViewport[3]!=clip.bottom)
		{
			// store the view rect
			mViewport[0] = clip.left;
			mViewport[1] = clip.top;
			mViewport[2] = clip.right;
			mViewport[3] = clip.bottom;
		}

		UNGUARD;
	}

	/*
	====================
	Draw
	====================
	*/
	VOID Map::Draw(Graph *gc)
	{
		GUARD(Map::Draw);

		for(I32 j = mViewport[1]; j < mViewport[3]; j++)
		{
			for(I32 i = mViewport[0]; i < mViewport[2]; i++)
			{
				std::pair<I32,I32> coord(i,j);
				mChunks.find(coord);
				std::map< std::pair<I32,I32>, ChunkPtr >::iterator it = mChunks.find(coord);
				if(it == mChunks.end()) continue;
				ChunkPtr& chunk = it->second;
				AUTO_LOCK_WORLD(gc);
				gc->world(Matrix::makeTranslate(i*CHUNK_STRIDE,j*CHUNK_STRIDE,0)*gc->world());
				chunk->Draw(gc);
			}
		}

		UNGUARD;
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
