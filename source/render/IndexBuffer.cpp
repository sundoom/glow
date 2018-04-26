//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////		

	IndexBuffer::IndexBuffer()
	{
		GUARD(IndexBuffer::IndexBuffer);

		mObject = 0;
		mCount = 0;
		mStride = 0;

		UNGUARD;
	}

	IndexBuffer::~IndexBuffer() 
	{
		GUARD(IndexBuffer::~IndexBuffer);

		if(mRCPtr) {if(mObject){mRCPtr->Release(ROT_INDEX_BUFFER, mObject); mObject = 0;}}

		UNGUARD;
	}	

	//////////////////////////////////////////////////////////////////////////
	
	/*
	====================
	Load
	====================
	*/
	VOID IndexBuffer::Load(const GData* data)
	{
		GUARD(IndexBuffer::Load);

		CHECK(data);
		mDataPtr = (GData*)data;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Bind
	====================
	*/
	VOID IndexBuffer::Bind(Context * rc)
	{
		GUARD(IndexBuffer::bind);

		// check the context
		if(mRCPtr == NULL) mRCPtr = (Context*)rc;
		CHECK(mRCPtr == rc);

		if(mObject == 0)
		{
			// generate the buffer object
			glGenBuffers(1, &mObject); CHECK(mObject);

			// bind the buffer object
			rc->BindIndexBuffer(mObject);

			// commit the data to the gpu memory
			if(mDataPtr){Update(mDataPtr.Ptr(), TRUE); mDataPtr.Release();}
		}
		else
		{
			// bind the buffer object
			rc->BindIndexBuffer(mObject);

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
	VOID IndexBuffer::Update(const GData* data, BOOL created)
	{
		GUARD(IndexBuffer::Update);

		// check the data header
		U8 * data_ptr = (U8*)data->Ptr();
		CHECK(*(U32*)data_ptr == (MAKEFOURCC('G','I','B','O')));
		data_ptr += sizeof(U32);

		// get the count
		mCount = *(U32*)data_ptr;
		data_ptr += sizeof(U32);

		// get the stride
		mStride = *(U32*)data_ptr;
		data_ptr += sizeof(U32);

		// commit the data to the gpu memory
		if(created)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount*mStride, data_ptr, GL_STATIC_DRAW);
		}
		else
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mCount*mStride, data_ptr);
		}

		UNGUARD;
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
