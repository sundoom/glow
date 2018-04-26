/*
 *  Primitive.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Primitive__
#define __Primitive__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented primitive interface.
	class Primitive: public Operation
	{
	public:
		enum TYPE
		{
			PT_POINTS			= 0x0000,
			PT_LINES			= 0x0001,
			PT_LINE_LOOP		= 0x0002,
			PT_LINE_STRIP       = 0x0003,
			PT_TRIANGLES        = 0x0004,
			PT_TRIANGLE_STRIP   = 0x0005,
			PT_TRIANGLE_FAN     = 0x0006,
			PT_QUADS            = 0x0007,
			PT_QUAD_STRIP       = 0x0008,
			PT_POLYGON          = 0x0009,
		};

	public:		
		// constructor
		Primitive();
		
		// destructor
		virtual ~Primitive();

		// set/get the element type
		VOID SetType(U32 t);
		U32 GetType() const;		

		// set/get the shader
		VOID SetShader(const Shader * s, const CHAR* pass);
		const Shader* GetShader() const;
		Shader* GetShader();

		// set/get the constant
		VOID SetConstant(const CHAR* name, const Constant* c);
		const Constant* GetConstant(const CHAR *name) const;
		Constant* GetConstant(const CHAR *name);

		// set/get the vertex buffer
		VOID SetVertexBuffer(const VertexBuffer* vb);
		const VertexBuffer* GetVertexBuffer() const;
		VertexBuffer* GetVertexBuffer();

		// set/get the index buffer
		VOID SetIndexBuffer(const IndexBuffer * ib);
		const IndexBuffer* GetIndexBuffer() const;
		IndexBuffer* GetIndexBuffer();

		// set/get the bounding box
		VOID SetBox(const BoundingBox& box);
		const BoundingBox& GetBox() const;
		BoundingBox& GetBox();

	private:
		// implement the render operation
		VOID operator() (class Context*rc);

	private:
		U32 mType;
		ShaderPtr mShaderPtr;
		Str mPassName;
		std::map< Str, std::pair<ConstantPtr, HANDLE> > mConstants;
		VertexBufferPtr mVBPtr;
		IndexBufferPtr mIBPtr;
		BoundingBox mBox;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Primitive>PrimitivePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Primitive__

//////////////////////////////////////////////////////////////////////////