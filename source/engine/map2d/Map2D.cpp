//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	Map2D::Map2D()
	{
		mWidth = mHeight = 0;
	}
	
	Map2D::~Map2D()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID Map2D::Load(const GData* data)
	{
		GUARD(Map2D::Load);

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
				Chunk2DPtr chunk = GNEW(Chunk2D); CHECK(chunk);
				chunk->Load(data_ptr);
				mChunks.push_back(chunk);
			}
		}

		UNGUARD;
	}

	/*
	====================
	update
	====================
	*/
	VOID Map2D::Update(const Vector4& viewport)
	{
		GUARD(Map2D::Update);

		Rect src, dst, clip;

		// compute the view
		src.left	= U2P(viewport[0])/U2P(CHUNK_STRIDE);
		src.top		= U2P(viewport[1])/U2P(CHUNK_STRIDE);
		src.right	= (U2P(viewport[2])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[2])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[2])/U2P(CHUNK_STRIDE);
		src.bottom	= (U2P(viewport[3])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[3])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[3])/U2P(CHUNK_STRIDE);
		
		dst.left	= 0;
		dst.top		= 0;
		dst.right	= mWidth;
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
	VOID Map2D::Draw(Graph *gc)
	{
		GUARD(Map2D::Draw);

		for(U32 j = mViewport[1]; j < mViewport[3]; j++)
		{
			for(U32 i = mViewport[0]; i < mViewport[2]; i++)
			{
				Chunk2DPtr& chunk = mChunks[i+j*mWidth];
				AUTO_LOCK_WORLD(gc);
				I32 px, py; T2P(px, py, i, j);
				gc->world(Matrix::makeTranslate(P2U(px),P2U(py),0)*gc->world());
				chunk->Draw(gc);
			}
		}

		UNGUARD;
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
