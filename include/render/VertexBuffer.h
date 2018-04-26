/*
 *  VertexBuffer.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __VertexBuffer__
#define __VertexBuffer__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented vertex object interface.
	class VertexBuffer : public GReference
	{
	public:
		// the vertex type
		enum TYPE
		{
			VT_1I,
			VT_2I,
			VT_3I,
			VT_4I,
			VT_1F,
			VT_2F,
			VT_3F,
			VT_4F
		};

	public:		
		// constructor
		VertexBuffer();
		
		// destructor
		virtual ~VertexBuffer();

		// load the data
		VOID Load(const GData* data);

	private:
		// bind the vertex buffer
		VOID Bind(class Context* rc);

		// update the buffer
		VOID Update(const GData* data, BOOL created);

	private:
		GPtr<class Context>mRCPtr;
		GDataPtr mDataPtr;
		U32 mObject;
		U32 mCount;
		U32 mStride;
		std::vector<U8>mAttribs;

	private: friend class Primitive;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<VertexBuffer>VertexBufferPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __VertexBuffer__

//////////////////////////////////////////////////////////////////////////