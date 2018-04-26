//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer()
	{
		GUARD(VertexBuffer::VertexBuffer);

		mObject = 0;
		mCount = 0;
		mStride = 0;
		mAttribs.push_back(0);

		UNGUARD;
	}

	VertexBuffer::~VertexBuffer() 
	{
		GUARD(VertexBuffer::~VertexBuffer);

		if(mRCPtr) {if(mObject){mRCPtr->Release(ROT_VERTEX_BUFFER, mObject); mObject = 0;}}

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////	

	/*
	====================
	Load
	====================
	*/
	VOID VertexBuffer::Load(const GData* data)
	{
		GUARD(VertexBuffer::Load);

		CHECK(data);
		mDataPtr = (GData*)data;

		// check the data header
		U8* data_ptr = (U8*)data->Ptr();
		CHECK(*(U32*)data_ptr == (MAKEFOURCC('G','V','B','O')));
		data_ptr += sizeof(U32);

		// get the count
		mCount = *(U32*)data_ptr;
		data_ptr += sizeof(U32);

		// get the stride
		mStride = *(U32*)data_ptr;
		data_ptr += sizeof(U32);

		mAttribs.clear();

		// get the count of the attribute		
		U8 attrib_count = *(U8*)data_ptr;
		data_ptr += sizeof(U8);
		mAttribs.push_back(attrib_count);

		// get the attribute
		for(U32 i = 0; i < attrib_count; i++) mAttribs.push_back(data_ptr[i]);				
		data_ptr += attrib_count * sizeof(U8);

		UNGUARD;
	}
	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Bind
	====================
	*/
	VOID VertexBuffer::Bind(Context* rc)
	{
		GUARD(VertexBuffer::Bind);

		// check the context
		if(mRCPtr == NULL) mRCPtr = (Context*)rc;
		CHECK(mRCPtr == rc);

		if(mObject == 0)
		{
			// generate the buffer object
			glGenBuffers(1, &mObject); CHECK(mObject);

			// bind the buffer object
			mRCPtr->BindVertexBuffer(mObject, mStride, &mAttribs[0]);

			// commit the data to the gpu memory
			if(mDataPtr){Update(mDataPtr.Ptr(), TRUE); mDataPtr.Release();}
		}
		else
		{
			// bind the buffer object
			mRCPtr->BindVertexBuffer(mObject, mStride, &mAttribs[0]);

			// commit the data to the gpu memory
			if(mDataPtr){Update(mDataPtr.Ptr(), FALSE); mDataPtr.Release();}
		}

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Update
	====================
	*/
	VOID VertexBuffer::Update(const GData* data, BOOL created)
	{
		GUARD(VertexBuffer::Update);

		// check the data header
		U8* data_ptr = (U8*)data->Ptr();
		CHECK( *(U32*)data_ptr == (MAKEFOURCC('G','V','B','O')) );
		data_ptr += sizeof(U32);

		// get the count
		mCount = *(U32*)data_ptr;
		data_ptr += sizeof(U32);

		// get the stride
		mStride = *(U32*)data_ptr;
		data_ptr += sizeof(U32);

		mAttribs.clear();

		// get the count of the attribute		
		U8 attrib_count = *(U8*)data_ptr;
		data_ptr += sizeof(U8);
		mAttribs.push_back(attrib_count);

		// get the attribute
		for(U32 i = 0; i < attrib_count; i++) mAttribs.push_back(data_ptr[i]);				
		data_ptr += attrib_count * sizeof(U8);

		if(created)
		{
			// commit the data to the gpu memory
			glBufferData(GL_ARRAY_BUFFER, mCount*mStride, data_ptr, GL_STATIC_DRAW);
		}
		else
		{
			// commit the data to the gpu memory
			glBufferSubData(GL_ARRAY_BUFFER, 0, mCount*mStride, data_ptr);
		}

		UNGUARD;
	}	
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
