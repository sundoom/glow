/*
 *  Shader.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Shader__
#define __Shader__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented shader interface.
	class Shader : public GReference
	{		
	public:		
		// constructor
		Shader();
		
		// destructor
		virtual ~Shader();

		// load the data
		VOID Load(const GData* data);

	private:
		// get the uniform handle
		HANDLE GetUniform(const CHAR *name) const;

		// set the uniform value
		VOID Set1f(HANDLE handle, F32 v);
		VOID Set2f(HANDLE handle, F32 v0, F32 v1);
		VOID Set3f(HANDLE handle, F32 v0, F32 v1, F32 v2);
		VOID Set4f(HANDLE handle, F32 v0, F32 v1, F32 v2, F32 v3);
		VOID Set1i(HANDLE handle, I32 v);
		VOID Set2i(HANDLE handle, I32 v0, I32 v1);
		VOID Set3i(HANDLE handle, I32 v0, I32 v1, I32 v2);
		VOID Set4i(HANDLE handle, I32 v0, I32 v1, I32 v2, I32 v3);
		VOID Set1fv(HANDLE handle, U32 count, const F32 * v);
		VOID Set2fv(HANDLE handle, U32 count, const F32 * v);
		VOID Set3fv(HANDLE handle, U32 count, const F32 * v);
		VOID Set4fv(HANDLE handle, U32 count, const F32 * v);
		VOID Set1iv(HANDLE handle, U32 count, const I32 * v);
		VOID Set2iv(HANDLE handle, U32 count, const I32 * v);
		VOID Set3iv(HANDLE handle, U32 count, const I32 * v);
		VOID Set4iv(HANDLE handle, U32 count, const I32 * v);
		VOID SetMatrix2fv(HANDLE handle, U32 count, const F32 * v);
		VOID SetMatrix3fv(HANDLE handle, U32 count, const F32 * v);
		VOID SetMatrix4fv(HANDLE handle, U32 count, const F32 * v);
		VOID SetSampler(HANDLE handle, const BaseTexture* texture);

	private:
		// bind the shader
		VOID Bind(Context* rc);
		BOOL BindPass(const CHAR* name);

		// update the shader
		VOID Update(const GData* data);

	private:
		#pragma pack(1)
		struct SAMPLER
		{
			BaseTexturePtr texture;
			U32 wrap_s, wrap_t, wrap_r;
			U32 min_filter, mag_filter;
		};
		struct UNIFORM
		{			
			U32	type;
			U32	count;
			std::vector<U8>data;
			SAMPLER sampler;
		};
		struct PROGRAM
		{
			Str name;
			U32	object;
			std::list< std::pair<I32, UNIFORM*> >uniforms;
		};
		struct PASS
		{
			PROGRAM* program;
			U32 blend_src, blend_dst;
			U32	depth_func;
			BOOL depth_mask;
			U32	cull_mode;
		};
		#pragma pack()

	private:
		GPtr<class Context>mRCPtr;
		GDataPtr mDataPtr;
		std::list<PROGRAM>mPrograms;
		std::map<Str, UNIFORM>mUniforms;
		std::map<Str, PASS>mPasses;

	private: friend class Primitive;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Shader>ShaderPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Shader__

//////////////////////////////////////////////////////////////////////////