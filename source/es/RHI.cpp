/*
 *  RHI.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <RHI.h>
#include "Command.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "State.h"
#include "BaseTexture.h"
#include "Texture.h"
#include "CubeTexture.h"
#include "RenderTarget.h"
#include "Shader.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(RHI)

//////////////////////////////////////////////////////////////////////////

#ifndef USE_RENDER_THREAD

//////////////////////////////////////////////////////////////////////////
  
	/*
	====================
	RICreateVertexBuffer
	====================
	*/
	HANDLE RICreateVertexBuffer(U32 count, U32 stride, U8 attrib[16], VOID* data)
	{
		VertexBuffer* vb_ptr = GNEW(VertexBuffer);
		vb_ptr->Load(count, stride, attrib, data);
		return vb_ptr;
	}

	/*
	====================
	RIUpdateVertexBuffer
	====================
	*/
	VOID RIUpdateVertexBuffer(HANDLE handle, U32 count, U32 stride, U8 attrib[16], VOID* data)
	{
		CHECK(handle);
		VertexBuffer* vb_ptr = (VertexBuffer*)handle;
		vb_ptr->Load(count, stride, attrib, data);
	}

	/*
	====================
	RIDestroyVertexBuffer
	====================
	*/
	VOID RIDestroyVertexBuffer(HANDLE handle)
	{
		CHECK(handle);
		VertexBuffer* vb_ptr = (VertexBuffer*)handle;
		GDELETE(vb_ptr);
	}

	/*
	====================
	RIBindVertexBuffer
	====================
	*/
	VOID RIBindVertexBuffer(HANDLE handle)
	{
		CHECK(handle);
		VertexBuffer* vb_ptr = (VertexBuffer*)handle;
		vb_ptr->Bind();
	}  

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateIndexBuffer
	====================
	*/
	HANDLE RICreateIndexBuffer(U32 count, U32 stride, VOID* data)
	{
		IndexBuffer* ib_ptr = GNEW(IndexBuffer);
		ib_ptr->Load(count, stride, data);
		return ib_ptr;
	}

	/*
	====================
	RIUpdateIndexBuffer
	====================
	*/
	VOID RIUpdateIndexBuffer(HANDLE handle, U32 count, U32 stride, VOID* data)
	{
		CHECK(handle);
		IndexBuffer* ib_ptr = (IndexBuffer*)handle;
		ib_ptr->Load(count, stride, data);
	}

	/*
	====================
	RIDestroyIndexBuffer
	====================
	*/
	VOID RIDestroyIndexBuffer(HANDLE handle)
	{
		CHECK(handle);
		IndexBuffer* ib_ptr = (IndexBuffer*)handle;
		GDELETE(ib_ptr);
	}

	/*
	====================
	RIBindIndexBuffer
	====================
	*/
	VOID RIBindIndexBuffer(HANDLE handle)
	{
		CHECK(handle);
		IndexBuffer* ib_ptr = (IndexBuffer*)handle;
		ib_ptr->Bind();
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateTexture
	====================
	*/
	HANDLE RICreateTexture(U32 level, U32 width, U32 height, U32 format, U32 type, const VOID* pixels)
	{
		Texture*tex_ptr = GNEW(Texture);
		if(level > 0 )
		{
			tex_ptr->Load(level, width, height, format, type, (U8*)pixels);
		}
		else
		{
			tex_ptr->Load(1, width, height, format, type, NULL);
		}
		return tex_ptr;
	}

	/*
	====================
	RIUpdateTexture
	====================
	*/
	VOID RIUpdateTexture(HANDLE handle, U32 level, I32 x, I32 y, U32 width, U32 height, U32 format, U32 type, const VOID* pixels)
	{
		CHECK(handle);
		Texture*tex_ptr = (Texture*)handle;
		tex_ptr->Update(level, x, y, width, height, format, type, (U8*)pixels);
	}

	/*
	====================
	RIDestroyTexture
	====================
	*/
	VOID RIDestroyTexture(HANDLE handle)
	{
		CHECK(handle);
		Texture*tex_ptr = (Texture*)handle;
		GDELETE(tex_ptr);
	}

	/*
	====================
	RIBindTexture
	====================
	*/
	VOID RIBindTexture(HANDLE handle, U32 unit)
	{
		CHECK(handle);
		Texture*tex_ptr = (Texture*)handle;
		tex_ptr->Bind(unit);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateRenderTarget
	====================
	*/
	HANDLE RICreateRenderTarget(HANDLE color, HANDLE depth)
	{
		CHECK(color || depth);
		RenderTarget* rt_ptr = GNEW(RenderTarget);
		rt_ptr->Build((BaseTexture*)color, (BaseTexture*)depth);
		return rt_ptr;
	}

	/*
	====================
	RIDestroyRenderTarget
	====================
	*/
	VOID RIDestroyRenderTarget(HANDLE handle)
	{
		CHECK(handle);
		RenderTarget* rt_ptr = (RenderTarget*)handle;
		GDELETE(rt_ptr);
	}

	/*
	====================
	RIBindRenderTarget
	====================
	*/
	VOID RIBindRenderTarget(HANDLE handle)
	{
		if(handle)
		{
			RenderTarget* rt_ptr = (RenderTarget*)handle;
			rt_ptr->Bind();
		}
		else
		{
			BindFramebuffer(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateShader
	====================
	*/
	HANDLE RICreateShader(VOID* data, U32 size)
	{
		Shader* shader_ptr = GNEW(Shader);
		shader_ptr->Load(data, size);
		return shader_ptr;
	}

	/*
	====================
	RIDestroyShader
	====================
	*/
	VOID RIDestroyShader(HANDLE handle)
	{
		CHECK(handle);
		Shader* shader_ptr = (Shader*)handle;
		GDELETE(shader_ptr);
	}

	/*
	====================
	RIBindShader
	====================
	*/
	VOID RIBindShader(HANDLE handle, const CHAR* pass)
	{
		CHECK(handle && pass);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->Bind(pass);
	}

	/*
	====================
	RISetShaderParameter1f
	====================
	*/
	VOID RISetShaderParameter1f(HANDLE handle, const CHAR* pname, F32 v)
	{
		CHECK(handle && pname);   
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter1f(pname, v);
	}

	/*
	====================
	RISetShaderParameter2f
	====================
	*/
	VOID RISetShaderParameter2f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1)
	{
		CHECK(handle && pname);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter2f(pname, v0, v1);
	}

	/*
	====================
	RISetShaderParameter3f
	====================
	*/
	VOID RISetShaderParameter3f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1, F32 v2)
	{
		CHECK(handle && pname);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter3f(pname, v0, v1, v2);
	}

	/*
	====================
	RISetShaderParameter4f
	====================
	*/
	VOID RISetShaderParameter4f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1, F32 v2, F32 v3)
	{
		CHECK(handle && pname);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter4f(pname, v0, v1, v2, v3);
	}

	/*
	====================
	RISetShaderParameter1i
	====================
	*/
	VOID RISetShaderParameter1i(HANDLE handle, const CHAR* pname, I32 v)
	{
		CHECK(handle && pname);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter1i(pname, v);   
	}

	/*
	====================
	RISetShaderParameter2i
	====================
	*/
	VOID RISetShaderParameter2i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1)
	{
		CHECK(handle && pname);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter2i(pname, v0, v1);
	}

	/*
	====================
	RISetShaderParameter3i
	====================
	*/
	VOID RISetShaderParameter3i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1, I32 v2)
	{
		CHECK(handle && pname);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter3i(pname, v0, v1, v2);

	}

	/*
	====================
	RISetShaderParameter4i
	====================
	*/
	VOID RISetShaderParameter4i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1, I32 v2, I32 v3)
	{
		CHECK(handle && pname);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter4i(pname, v0, v1, v2, v3);
	}

	/*
	====================
	RISetShaderParameter1fv
	====================
	*/
	VOID RISetShaderParameter1fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter1fv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameter2fv
	====================
	*/
	VOID RISetShaderParameter2fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter2fv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameter3fv
	====================
	*/
	VOID RISetShaderParameter3fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter3fv(pname, count, v);  
	}

	/*
	====================
	RISetShaderParameter4fv
	====================
	*/
	VOID RISetShaderParameter4fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter4fv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameter1iv
	====================
	*/
	VOID RISetShaderParameter1iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter1iv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameter2iv
	====================
	*/
	VOID RISetShaderParameter2iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter2iv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameter3iv
	====================
	*/
	VOID RISetShaderParameter3iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter3iv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameter4iv
	====================
	*/
	VOID RISetShaderParameter4iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameter4iv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameterMatrix2fv
	====================
	*/
	VOID RISetShaderParameterMatrix2fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameterMatrix2fv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameterMatrix3fv
	====================
	*/
	VOID RISetShaderParameterMatrix3fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameterMatrix3fv(pname, count, v);    
	}

	/*
	====================
	RISetShaderParameterMatrix4fv
	====================
	*/
	VOID RISetShaderParameterMatrix4fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameterMatrix4fv(pname, count, v);
	}

	/*
	====================
	RISetShaderParameterSampler
	====================
	*/
	VOID RISetShaderParameterSampler(HANDLE handle, const CHAR* pname, HANDLE texture)
	{
		CHECK(handle && pname && texture);
		Shader* shader_ptr = (Shader*)handle;
		shader_ptr->SetParameterSampler(pname, (BaseTexture*)texture);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RIReset
	====================
	*/
	VOID RIReset()
	{
		Reset();
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RIClear
	====================
	*/
	VOID RIClear(U32 flags, F32 red, F32 green, F32 blue, F32 alpha, F32 depth)
	{
		if(flags & CT_COLOR) glClearColor(red, green, blue, alpha);
		if(flags & CT_DEPTH) glClearDepthf(depth);
		glClear(flags);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RISetViewport
	====================
	*/
	VOID RISetViewport(I32 x, I32 y, I32 width, I32 height)
	{
		glViewport(x, y, width, height);
	}

	/*
	====================
	RISetScissor
	====================
	*/
	VOID RISetScissor(BOOL enable, I32 x, I32 y, I32 width, I32 height)
	{
    BindScissorTest(enable);
    if(enable) glScissor(x, y, width, height);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RIDrawPrimitive
	====================
	*/
	VOID RIDrawPrimitive(U32 mode, I32 count, U32 type)
	{
		glDrawElements(mode, count, type, 0);
	}

//////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////

	static I32 s_cmd_ptr = 0;
	static std::list<CMD_BASE*>s_cmd_list[2];

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateVertexBuffer
	====================
	*/
	HANDLE RICreateVertexBuffer(U32 count, U32 stride, U8 attrib[16], VOID* data)
	{
		HANDLE handle = GNEW(VertexBuffer);
		CMD_CREATE_VERTEX_BUFFER* cmd = (CMD_CREATE_VERTEX_BUFFER*)GNEW(U8[sizeof(CMD_CREATE_VERTEX_BUFFER)+count*stride]);
		cmd->base.type = CT_CREATE_VERTEX_BUFFER;
		cmd->handle = handle;
		cmd->count = count;
		cmd->stride = stride;
		::memcpy(cmd->attrib, attrib, sizeof(cmd->attrib));
		::memcpy((cmd+1), data, count*stride*sizeof(U8));
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
		return handle;
	}

	/*
	====================
	RIUpdateVertexBuffer
	====================
	*/
	VOID RIUpdateVertexBuffer(HANDLE handle, U32 count, U32 stride, U8 attrib[16], VOID* data)
	{
		CHECK(handle);
		CMD_UPDATE_VERTEX_BUFFER*cmd = (CMD_UPDATE_VERTEX_BUFFER*)GNEW(U8[sizeof(CMD_UPDATE_VERTEX_BUFFER)+count*stride*sizeof(U8)]);
		cmd->base.type = CT_UPDATE_VERTEX_BUFFER;
		cmd->handle = handle;
		cmd->count = count;
		cmd->stride = stride;
		::memcpy(cmd->attrib, attrib, sizeof(cmd->attrib));
		::memcpy((cmd+1), data, count*stride*sizeof(U8));
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RIDestroyVertexBuffer
	====================
	*/
	VOID RIDestroyVertexBuffer(HANDLE handle)
	{
		CHECK(handle);
		CMD_DESTROY_VERTEX_BUFFER*cmd = GNEW(CMD_DESTROY_VERTEX_BUFFER);
		cmd->base.type = CT_DESTROY_VERTEX_BUFFER;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);    
	}

	/*
	====================
	RIBindVertexBuffer
	====================
	*/
	VOID RIBindVertexBuffer(HANDLE handle)
	{
		CHECK(handle);
		CMD_BIND_VERTEX_BUFFER* cmd = GNEW(CMD_BIND_VERTEX_BUFFER);
		cmd->base.type = CT_BIND_VERTEX_BUFFER;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}  

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateIndexBuffer
	====================
	*/
	HANDLE RICreateIndexBuffer(U32 count, U32 stride, VOID* data)
	{
		HANDLE handle = GNEW(IndexBuffer);
		CMD_CREATE_INDEX_BUFFER*cmd = (CMD_CREATE_INDEX_BUFFER*)GNEW(U8[sizeof(CMD_CREATE_INDEX_BUFFER)+count*stride]);
		cmd->base.type = CT_CREATE_INDEX_BUFFER;
		cmd->handle = handle;
		cmd->count = count;
		cmd->stride = stride;
		::memcpy((cmd+1), data, count*stride*sizeof(U8));
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
		return handle;
	}

	/*
	====================
	RIUpdateIndexBuffer
	====================
	*/
	VOID RIUpdateIndexBuffer(HANDLE handle, U32 count, U32 stride, VOID* data)
	{
		CHECK(handle);
		CMD_UPDATE_INDEX_BUFFER*cmd = (CMD_UPDATE_INDEX_BUFFER*)GNEW(U8[sizeof(CMD_UPDATE_INDEX_BUFFER)+count*stride]);
		cmd->base.type = CT_UPDATE_INDEX_BUFFER;
		cmd->handle = handle;
		cmd->count = count;
		cmd->stride = stride;
		::memcpy((cmd+1), data, count*stride*sizeof(U8));
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RIDestroyIndexBuffer
	====================
	*/
	VOID RIDestroyIndexBuffer(HANDLE handle)
	{
		CHECK(handle);
		CMD_DESTROY_INDEX_BUFFER*cmd = GNEW(CMD_DESTROY_INDEX_BUFFER);
		cmd->base.type = CT_DESTROY_INDEX_BUFFER;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RIBindIndexBuffer
	====================
	*/
	VOID RIBindIndexBuffer(HANDLE handle)
	{
		CHECK(handle);
		CMD_BIND_INDEX_BUFFER* cmd = GNEW(CMD_BIND_INDEX_BUFFER);
		cmd->base.type = CT_BIND_INDEX_BUFFER;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateTexture
	====================
	*/
	HANDLE RICreateTexture(U32 level, U32 width, U32 height, U32 format, U32 type, const VOID* pixels)
	{
		U32 size = 0;
		U32 w = width;
		U32 h = height;
		for(U32 i = 0; i < level; i++)
		{
			switch(format)
			{
      case PF_ALPHA:
        size += w * h * sizeof(U8);
        break;
      case PF_RGB:
        size += w * h * 3 * sizeof(U8);
        break;
      case PF_RGBA:
        size += w * h * 4 * sizeof(U8);
        break;
      case PF_L:
        size += w * h * sizeof(U8);
        break;
      case PF_LA:
        size += w * h * 2 * sizeof(U8);
        break;
      case PF_RGB_DXT1:
        size += (((w+3)/4) * ((h+3)/4) * 8);
        break;
      case PF_RGBA_DXT1:
        size += (((w+3)/4) * ((h+3)/4) * 8);        
        break;
      case PF_RGBA_DXT3:      
        size += (((w+3)/4) * ((h+3)/4) * 16);
        break;
      case PF_RGBA_DXT5:
        size += (((w+3)/4) * ((h+3)/4) * 16);        
        break;
			default:
				GAssert("The format of texture is error!\n");
				break;
			}
			w >>= 1; h >>= 1;
		}
		HANDLE handle = GNEW(Texture);
		CMD_CREATE_TEXTURE*cmd = (CMD_CREATE_TEXTURE*)GNEW(U8[sizeof(CMD_CREATE_TEXTURE)+size]);
		cmd->base.type = CT_CREATE_TEXTURE;
		cmd->handle = handle;
		cmd->level = level;
		cmd->width = width;
		cmd->height = height;
		cmd->format = format;
		cmd->type = type;
		if(size > 0) ::memcpy((cmd+1), pixels, size);		
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
		return handle;
	}

	/*
	====================
	RIUpdateTexture
	====================
	*/
	VOID RIUpdateTexture(HANDLE handle, U32 level, I32 x, I32 y, U32 width, U32 height, U32 format, U32 type, const VOID* pixels)
	{
		CHECK(handle);
		U32 size = 0;
		U32 w = width;
		U32 h = height;
		for(U32 i = 0; i < level; i++)
		{
      switch(format)
      {
      case PF_ALPHA:
        size += w * h * sizeof(U8);
        break;
      case PF_RGB:
        size += w * h * 3 * sizeof(U8);
        break;
      case PF_RGBA:
        size += w * h * 4 * sizeof(U8);
        break;
      case PF_L:
        size += w * h * sizeof(U8);
        break;
      case PF_LA:
        size += w * h * 2 * sizeof(U8);
        break;
      case PF_RGB_DXT1:
        size += (((w+3)/4) * ((h+3)/4) * 8);
        break;
      case PF_RGBA_DXT1:
        size += (((w+3)/4) * ((h+3)/4) * 8);        
        break;
      case PF_RGBA_DXT3:      
        size += (((w+3)/4) * ((h+3)/4) * 16);
        break;
      case PF_RGBA_DXT5:
        size += (((w+3)/4) * ((h+3)/4) * 16);        
        break;
      default:
        GAssert("The format of texture is error!\n");
        break;
      }
			w >>= 1; h >>= 1;
		}
		CMD_UPDATE_TEXTURE*cmd = (CMD_UPDATE_TEXTURE*)GNEW(U8[sizeof(CMD_UPDATE_TEXTURE)+size]);
		cmd->base.type = CT_UPDATE_TEXTURE;
		cmd->handle = handle;
		cmd->level = level;
		cmd->x = x;
		cmd->y = y;
		cmd->width = width;
		cmd->height = height;
		cmd->format = format;
		cmd->type = type;
		::memcpy((cmd+1), pixels, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RIDestroyTexture
	====================
	*/
	VOID RIDestroyTexture(HANDLE handle)
	{
		CHECK(handle);
		CMD_DESTROY_TEXTURE*cmd = GNEW(CMD_DESTROY_TEXTURE);
		cmd->base.type = CT_DESTROY_TEXTURE;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RIBindTexture
	====================
	*/
	VOID RIBindTexture(HANDLE handle, U32 unit)
	{
		CHECK(handle);
		CMD_BIND_TEXTURE* cmd = GNEW(CMD_BIND_TEXTURE);
		cmd->base.type = CT_BIND_TEXTURE;
		cmd->handle = handle;
		cmd->unit = unit;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateRenderTarget
	====================
	*/
	HANDLE RICreateRenderTarget(HANDLE color, HANDLE depth)
	{
		CHECK(color || depth);
		HANDLE handle = GNEW(RenderTarget);
		CMD_CREATE_RENDER_TARGET*cmd = GNEW(CMD_CREATE_RENDER_TARGET);
		cmd->base.type = CT_CREATE_RENDER_TARGET;
		cmd->handle = handle;
		cmd->color = color;
		cmd->depth = depth;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
		return handle;
	}

	/*
	====================
	RIDestroyRenderTarget
	====================
	*/
	VOID RIDestroyRenderTarget(HANDLE handle)
	{
		CHECK(handle);
		CMD_DESTROY_RENDER_TARGET*cmd = GNEW(CMD_DESTROY_RENDER_TARGET);
		cmd->base.type = CT_DESTROY_RENDER_TARGET;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RIBindRenderTarget
	====================
	*/
	VOID RIBindRenderTarget(HANDLE handle)
	{
		CMD_BIND_RENDER_TARGET* cmd = GNEW(CMD_BIND_RENDER_TARGET);
		cmd->base.type = CT_BIND_RENDER_TARGET;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RICreateShader
	====================
	*/
	HANDLE RICreateShader(VOID* data, U32 size)
	{
		HANDLE handle = GNEW(Shader);
		CMD_CREATE_SHADER*cmd = (CMD_CREATE_SHADER*)GNEW(U8[sizeof(CMD_CREATE_SHADER)+size]);
		cmd->base.type = CT_CREATE_SHADER;
		cmd->handle = handle;
		cmd->size = size;
		::memcpy((cmd+1), data, size*sizeof(U8));
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
		return handle;
	}

	/*
	====================
	RIDestroyShader
	====================
	*/
	VOID RIDestroyShader(HANDLE handle)
	{
		CHECK(handle);
		CMD_DESTROY_SHADER*cmd = GNEW(CMD_DESTROY_SHADER);
		cmd->base.type = CT_DESTROY_SHADER;
		cmd->handle = handle;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RIBindShader
	====================
	*/
	VOID RIBindShader(HANDLE handle, const CHAR* pass)
	{
		CHECK(handle);
		CMD_BIND_SHADER*cmd = GNEW(CMD_BIND_SHADER);
		cmd->base.type = CT_BIND_SHADER;
		cmd->handle = handle;
		::strncpy(cmd->pass, pass, sizeof(cmd->pass));
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter1f
	====================
	*/
	VOID RISetShaderParameter1f(HANDLE handle, const CHAR* pname, F32 v)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_1F* cmd = GNEW(CMD_SET_SHADER_PARAMETER_1F);
		cmd->base.type = CT_SET_SHADER_PARAMETER_1F;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v = v;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter2f
	====================
	*/
	VOID RISetShaderParameter2f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_2F* cmd = GNEW(CMD_SET_SHADER_PARAMETER_2F);
		cmd->base.type = CT_SET_SHADER_PARAMETER_2F;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v[0] = v0;
		cmd->v[1] = v1;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter3f
	====================
	*/
	VOID RISetShaderParameter3f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1, F32 v2)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_3F* cmd = GNEW(CMD_SET_SHADER_PARAMETER_3F);
		cmd->base.type = CT_SET_SHADER_PARAMETER_3F;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v[0] = v0;
		cmd->v[1] = v1;
		cmd->v[2] = v2;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter4f
	====================
	*/
	VOID RISetShaderParameter4f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1, F32 v2, F32 v3)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_4F* cmd = GNEW(CMD_SET_SHADER_PARAMETER_4F);
		cmd->base.type = CT_SET_SHADER_PARAMETER_4F;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v[0] = v0;
		cmd->v[1] = v1;
		cmd->v[2] = v2;
		cmd->v[3] = v3;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter1i
	====================
	*/
	VOID RISetShaderParameter1i(HANDLE handle, const CHAR* pname, I32 v)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_1I* cmd = GNEW(CMD_SET_SHADER_PARAMETER_1I);
		cmd->base.type = CT_SET_SHADER_PARAMETER_1I;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v = v;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter2i
	====================
	*/
	VOID RISetShaderParameter2i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_2I* cmd = GNEW(CMD_SET_SHADER_PARAMETER_2I);
		cmd->base.type = CT_SET_SHADER_PARAMETER_2I;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v[0] = v0;
		cmd->v[1] = v1;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter3i
	====================
	*/
	VOID RISetShaderParameter3i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1, I32 v2)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_3I* cmd = GNEW(CMD_SET_SHADER_PARAMETER_3I);
		cmd->base.type = CT_SET_SHADER_PARAMETER_3I;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v[0] = v0;
		cmd->v[1] = v1;
		cmd->v[2] = v2;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter4i
	====================
	*/
	VOID RISetShaderParameter4i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1, I32 v2, I32 v3)
	{
		CHECK(handle && pname);
		CMD_SET_SHADER_PARAMETER_4I* cmd = GNEW(CMD_SET_SHADER_PARAMETER_4I);
		cmd->base.type = CT_SET_SHADER_PARAMETER_4I;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->v[0] = v0;
		cmd->v[1] = v1;
		cmd->v[2] = v2;
		cmd->v[3] = v3;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	/*
	====================
	RISetShaderParameter1fv
	====================
	*/
	VOID RISetShaderParameter1fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * sizeof(F32);
		CMD_SET_SHADER_PARAMETER_1FV* cmd = (CMD_SET_SHADER_PARAMETER_1FV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_1FV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_1FV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameter2fv
	====================
	*/
	VOID RISetShaderParameter2fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 2 * sizeof(F32);
		CMD_SET_SHADER_PARAMETER_2FV* cmd = (CMD_SET_SHADER_PARAMETER_2FV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_2FV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_2FV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameter3fv
	====================
	*/
	VOID RISetShaderParameter3fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 3 * sizeof(F32);
		CMD_SET_SHADER_PARAMETER_3FV* cmd = (CMD_SET_SHADER_PARAMETER_3FV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_3FV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_3FV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameter4fv
	====================
	*/
	VOID RISetShaderParameter4fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 4 * sizeof(F32);
		CMD_SET_SHADER_PARAMETER_4FV* cmd = (CMD_SET_SHADER_PARAMETER_4FV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_4FV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_4FV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameter1iv
	====================
	*/
	VOID RISetShaderParameter1iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * sizeof(I32);
		CMD_SET_SHADER_PARAMETER_1IV* cmd = (CMD_SET_SHADER_PARAMETER_1IV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_1IV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_1IV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameter2iv
	====================
	*/
	VOID RISetShaderParameter2iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 2 * sizeof(I32);
		CMD_SET_SHADER_PARAMETER_2IV* cmd = (CMD_SET_SHADER_PARAMETER_2IV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_2IV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_2IV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameter3iv
	====================
	*/
	VOID RISetShaderParameter3iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 3 * sizeof(I32);
		CMD_SET_SHADER_PARAMETER_3IV* cmd = (CMD_SET_SHADER_PARAMETER_3IV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_3IV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_3IV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameter4iv
	====================
	*/
	VOID RISetShaderParameter4iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 4 * sizeof(I32);
		CMD_SET_SHADER_PARAMETER_4IV* cmd = (CMD_SET_SHADER_PARAMETER_4IV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_4IV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_4IV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameterMatrix2fv
	====================
	*/
	VOID RISetShaderParameterMatrix2fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 4 * sizeof(F32);
		CMD_SET_SHADER_PARAMETER_MATRIX_2FV* cmd = (CMD_SET_SHADER_PARAMETER_MATRIX_2FV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_MATRIX_2FV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_MATRIX_2FV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameterMatrix3fv
	====================
	*/
	VOID RISetShaderParameterMatrix3fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 9 * sizeof(F32);
		CMD_SET_SHADER_PARAMETER_MATRIX_3FV* cmd = (CMD_SET_SHADER_PARAMETER_MATRIX_3FV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_MATRIX_3FV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_MATRIX_3FV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameterMatrix4fv
	====================
	*/
	VOID RISetShaderParameterMatrix4fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v)
	{
		CHECK(handle && pname && v);
		U32 size = count * 16 * sizeof(F32);
		CMD_SET_SHADER_PARAMETER_MATRIX_4FV* cmd = (CMD_SET_SHADER_PARAMETER_MATRIX_4FV*)GNEW(U8[sizeof(CMD_SET_SHADER_PARAMETER_MATRIX_4FV)+size]);
		cmd->base.type = CT_SET_SHADER_PARAMETER_MATRIX_4FV;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->count = count;
		::memcpy((cmd+1), v, size);
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	/*
	====================
	RISetShaderParameterSampler
	====================
	*/
	VOID RISetShaderParameterSampler(HANDLE handle, const CHAR* pname, HANDLE texture)
	{
		CHECK(handle && pname && texture);
		CMD_SET_SHADER_PARAMETER_SAMPLER* cmd = GNEW(CMD_SET_SHADER_PARAMETER_SAMPLER);
		cmd->base.type = CT_SET_SHADER_PARAMETER_SAMPLER;
		cmd->handle = handle;
		::strncpy(cmd->pname, pname, sizeof(cmd->pname));
		cmd->texture = texture;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd); 
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RIReset
	====================
	*/
	VOID RIReset()
	{
		CMD_RESET* cmd = GNEW(CMD_RESET);
		cmd->base.type = CT_RESET;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RIClear
	====================
	*/
	VOID RIClear(U32 flags, F32 red, F32 green, F32 blue, F32 alpha, F32 depth)
	{
		CMD_CLEAR* cmd = GNEW(CMD_CLEAR);
		cmd->base.type = CT_CLEAR;
		cmd->flags = flags;
		cmd->red = red;
		cmd->green = green;
		cmd->blue = blue;
		cmd->alpha = alpha;
		cmd->depth = depth;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	//////////////////////////////////////////////////////////////////////////

	VOID RISetViewport(I32 x, I32 y, I32 width, I32 height)
	{
		CMD_SET_VIEWPORT* cmd = GNEW(CMD_SET_VIEWPORT);
		cmd->base.type = CT_SET_VIEWPORT;
		cmd->x = x;
		cmd->y = y;
		cmd->width = width;
		cmd->height = height;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	VOID RISetScissor(BOOL enable, I32 x, I32 y, I32 width, I32 height)
	{
		CMD_SET_SCISSOR* cmd = GNEW(CMD_SET_SCISSOR);
		cmd->base.type = CT_SET_SCISSOR;
    cmd->enable = enable;
		cmd->x = x;
		cmd->y = y;
		cmd->width = width;
		cmd->height = height;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RIDrawPrimitive
	====================
	*/
	VOID RIDrawPrimitive(U32 mode, I32 count, U32 type)
	{
		CMD_DRAW_PRIMITIVE* cmd = GNEW(CMD_DRAW_PRIMITIVE);
		cmd->base.type = CT_DRAW_PRIMITIVE;
		cmd->mode = mode;
		cmd->count = count;
		cmd->type = type;
		s_cmd_list[s_cmd_ptr].push_back((CMD_BASE*)cmd);
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	RIExec
	====================
	*/
	VOID RIExec()
	{
		std::list<CMD_BASE*>& cmd_list = s_cmd_list[(s_cmd_ptr+1)%2];
		for (std::list<CMD_BASE*>::iterator it = cmd_list.begin(); it != cmd_list.end(); ++it)
		{
			CMD_BASE* cmd_base = (*it);
			switch(cmd_base->type)
			{
			case CT_CREATE_VERTEX_BUFFER:
				{
					CMD_CREATE_VERTEX_BUFFER*cmd = (CMD_CREATE_VERTEX_BUFFER*)cmd_base;
					VertexBuffer* vb_ptr = (VertexBuffer*)cmd->handle;
					vb_ptr->Load(cmd->count, cmd->stride, cmd->attrib, (cmd+1));
				}
				break;
			case CT_UPDATE_VERTEX_BUFFER:
				{
					CMD_UPDATE_VERTEX_BUFFER*cmd = (CMD_UPDATE_VERTEX_BUFFER*)cmd_base;
					VertexBuffer* vb_ptr = (VertexBuffer*)cmd->handle;
					vb_ptr->Load(cmd->count, cmd->stride, cmd->attrib, (cmd+1));
				}
				break;
			case CT_DESTROY_VERTEX_BUFFER:
				{
					CMD_DESTROY_VERTEX_BUFFER* cmd = (CMD_DESTROY_VERTEX_BUFFER*)cmd_base;
					VertexBuffer* vb_ptr = (VertexBuffer*)cmd->handle;
					GDELETE(vb_ptr);
				}
				break;
			case CT_BIND_VERTEX_BUFFER:
				{
					CMD_BIND_VERTEX_BUFFER* cmd = (CMD_BIND_VERTEX_BUFFER*)cmd_base;
					VertexBuffer* vb_ptr = (VertexBuffer*)cmd->handle;
					vb_ptr->Bind();
				}
				break;
			case CT_CREATE_INDEX_BUFFER:
				{
					CMD_CREATE_INDEX_BUFFER*cmd = (CMD_CREATE_INDEX_BUFFER*)cmd_base;
					IndexBuffer* ib_ptr = (IndexBuffer*)cmd->handle;
					ib_ptr->Load(cmd->count, cmd->stride, (cmd+1));
				}
				break;
			case CT_UPDATE_INDEX_BUFFER:
				{
					CMD_UPDATE_INDEX_BUFFER*cmd = (CMD_UPDATE_INDEX_BUFFER*)cmd_base;
					IndexBuffer* ib_ptr = (IndexBuffer*)cmd->handle;
					ib_ptr->Load(cmd->count, cmd->stride, (cmd+1));
				}
				break;
			case CT_DESTROY_INDEX_BUFFER:
				{
					CMD_DESTROY_INDEX_BUFFER* cmd = (CMD_DESTROY_INDEX_BUFFER*)cmd_base;
					IndexBuffer* ib_ptr = (IndexBuffer*)cmd->handle;
					GDELETE(ib_ptr);
				}
				break;
			case CT_BIND_INDEX_BUFFER:
				{
					CMD_BIND_INDEX_BUFFER*cmd = (CMD_BIND_INDEX_BUFFER*)cmd_base;
					IndexBuffer* ib_ptr = (IndexBuffer*)cmd->handle;
					ib_ptr->Bind();
				}
				break;
			case CT_CREATE_TEXTURE:
				{
					CMD_CREATE_TEXTURE*cmd = (CMD_CREATE_TEXTURE*)cmd_base;
					Texture*tex_ptr = (Texture*)cmd->handle;
					if(cmd->level > 0 )
					{
						tex_ptr->Load(cmd->level, cmd->width, cmd->height, cmd->format, cmd->type, (U8*)(cmd+1));
					}
					else
					{
						tex_ptr->Load(1, cmd->width, cmd->height, cmd->format, cmd->type, NULL);
					}
				}
				break;
			case CT_UPDATE_TEXTURE:
				{
					CMD_UPDATE_TEXTURE*cmd = (CMD_UPDATE_TEXTURE*)cmd_base;
					Texture*tex_ptr = (Texture*)cmd->handle;
					tex_ptr->Update(cmd->level, cmd->x, cmd->y, cmd->width, cmd->height, cmd->format, cmd->type, (U8*)(cmd+1));
				}
				break;
			case CT_DESTROY_TEXTURE:
				{
					CMD_UPDATE_TEXTURE* cmd = (CMD_UPDATE_TEXTURE*)cmd_base;
					Texture*tex_ptr = (Texture*)cmd->handle;
					GDELETE(tex_ptr);
				}
				break;
			case CT_BIND_TEXTURE:
				{
					CMD_BIND_TEXTURE* cmd = (CMD_BIND_TEXTURE*)cmd_base;
					Texture*tex_ptr = (Texture*)cmd->handle;
					tex_ptr->Bind();
				}
				break;
			case CT_CREATE_RENDER_TARGET:
				{
					CMD_CREATE_RENDER_TARGET* cmd = (CMD_CREATE_RENDER_TARGET*)cmd_base;
					RenderTarget* rt_ptr = (RenderTarget*)cmd->handle;
					rt_ptr->Build((BaseTexture*)cmd->color, (BaseTexture*)cmd->depth);
				}
				break;
			case CT_DESTROY_RENDER_TARGET:
				{
					CMD_DESTROY_RENDER_TARGET* cmd = (CMD_DESTROY_RENDER_TARGET*)cmd_base;
					RenderTarget* rt_ptr = (RenderTarget*)cmd->handle;
					GDELETE(rt_ptr);
				}
				break;
			case CT_BIND_RENDER_TARGET:
				{
					CMD_BIND_RENDER_TARGET* cmd = (CMD_BIND_RENDER_TARGET*)cmd_base;
					if(cmd->handle)
					{
						RenderTarget* rt_ptr = (RenderTarget*)cmd->handle;
						rt_ptr->Bind();
					}
					else
					{
						BindFramebuffer(0);
					}
				}
				break;
			case CT_CREATE_SHADER:
				{
					CMD_CREATE_SHADER*cmd = (CMD_CREATE_SHADER*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->Load((cmd+1), cmd->size);
				}
				break;
			case CT_DESTROY_SHADER:
				{
					CMD_DESTROY_SHADER* cmd = (CMD_DESTROY_SHADER*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					GDELETE(shader_ptr);
				}
				break;
			case CT_BIND_SHADER:
				{
					CMD_BIND_SHADER*cmd = (CMD_BIND_SHADER*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->Bind(cmd->pass);
				}
				break;
			case CT_SET_SHADER_PARAMETER_1F:
				{
					CMD_SET_SHADER_PARAMETER_1F*cmd = (CMD_SET_SHADER_PARAMETER_1F*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter1f(cmd->pname, cmd->v);
				}
				break;
			case CT_SET_SHADER_PARAMETER_2F:
				{
					CMD_SET_SHADER_PARAMETER_2F*cmd = (CMD_SET_SHADER_PARAMETER_2F*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter2f(cmd->pname, cmd->v[0], cmd->v[1]);
				}
				break;
			case CT_SET_SHADER_PARAMETER_3F:
				{
					CMD_SET_SHADER_PARAMETER_3F*cmd = (CMD_SET_SHADER_PARAMETER_3F*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter3f(cmd->pname, cmd->v[0], cmd->v[1], cmd->v[2]);
				}
				break;
			case CT_SET_SHADER_PARAMETER_4F:
				{
					CMD_SET_SHADER_PARAMETER_4F*cmd = (CMD_SET_SHADER_PARAMETER_4F*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter4f(cmd->pname, cmd->v[0], cmd->v[1], cmd->v[2], cmd->v[3]);
				}
				break;
			case CT_SET_SHADER_PARAMETER_1I:
				{
					CMD_SET_SHADER_PARAMETER_1I*cmd = (CMD_SET_SHADER_PARAMETER_1I*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter1i(cmd->pname, cmd->v);
				}
				break;
			case CT_SET_SHADER_PARAMETER_2I:
				{
					CMD_SET_SHADER_PARAMETER_2I*cmd = (CMD_SET_SHADER_PARAMETER_2I*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter2i(cmd->pname, cmd->v[0], cmd->v[1]);
				}
				break;
			case CT_SET_SHADER_PARAMETER_3I:
				{
					CMD_SET_SHADER_PARAMETER_3I*cmd = (CMD_SET_SHADER_PARAMETER_3I*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter3i(cmd->pname, cmd->v[0], cmd->v[1], cmd->v[2]);
				}
				break;
			case CT_SET_SHADER_PARAMETER_4I:
				{
					CMD_SET_SHADER_PARAMETER_4I*cmd = (CMD_SET_SHADER_PARAMETER_4I*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter4i(cmd->pname, cmd->v[0], cmd->v[1], cmd->v[2], cmd->v[3]);
				}
				break;
			case CT_SET_SHADER_PARAMETER_1FV:
				{
					CMD_SET_SHADER_PARAMETER_1FV*cmd = (CMD_SET_SHADER_PARAMETER_1FV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter1fv(cmd->pname, cmd->count, (F32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_2FV:
				{
					CMD_SET_SHADER_PARAMETER_2FV*cmd = (CMD_SET_SHADER_PARAMETER_2FV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter2fv(cmd->pname, cmd->count, (F32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_3FV:
				{
					CMD_SET_SHADER_PARAMETER_3FV*cmd = (CMD_SET_SHADER_PARAMETER_3FV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter3fv(cmd->pname, cmd->count, (F32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_4FV:
				{
					CMD_SET_SHADER_PARAMETER_4FV*cmd = (CMD_SET_SHADER_PARAMETER_4FV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter4fv(cmd->pname, cmd->count, (F32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_1IV:
				{
					CMD_SET_SHADER_PARAMETER_1IV*cmd = (CMD_SET_SHADER_PARAMETER_1IV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter1iv(cmd->pname, cmd->count, (I32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_2IV:
				{
					CMD_SET_SHADER_PARAMETER_2IV*cmd = (CMD_SET_SHADER_PARAMETER_2IV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter2iv(cmd->pname, cmd->count, (I32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_3IV:
				{
					CMD_SET_SHADER_PARAMETER_3IV*cmd = (CMD_SET_SHADER_PARAMETER_3IV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter3iv(cmd->pname, cmd->count, (I32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_4IV:
				{
					CMD_SET_SHADER_PARAMETER_4IV*cmd = (CMD_SET_SHADER_PARAMETER_4IV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameter4iv(cmd->pname, cmd->count, (I32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_MATRIX_2FV:
				{
					CMD_SET_SHADER_PARAMETER_MATRIX_2FV*cmd = (CMD_SET_SHADER_PARAMETER_MATRIX_2FV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameterMatrix2fv(cmd->pname, cmd->count, (F32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_MATRIX_3FV:
				{
					CMD_SET_SHADER_PARAMETER_MATRIX_3FV*cmd = (CMD_SET_SHADER_PARAMETER_MATRIX_3FV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameterMatrix3fv(cmd->pname, cmd->count, (F32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_MATRIX_4FV:
				{
					CMD_SET_SHADER_PARAMETER_MATRIX_4FV*cmd = (CMD_SET_SHADER_PARAMETER_MATRIX_4FV*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameterMatrix4fv(cmd->pname, cmd->count, (F32*)(cmd+1));
				}
				break;
			case CT_SET_SHADER_PARAMETER_SAMPLER:
				{
					CMD_SET_SHADER_PARAMETER_SAMPLER*cmd = (CMD_SET_SHADER_PARAMETER_SAMPLER*)cmd_base;
					Shader* shader_ptr = (Shader*)cmd->handle;
					shader_ptr->SetParameterSampler(cmd->pname, (BaseTexture*)cmd->texture);
				}
				break;
			case CT_RESET:
				{
					Reset();
				}
				break;
			case CT_CLEAR:
				{
					CMD_CLEAR* cmd = (CMD_CLEAR*)cmd_base;
					if(cmd->flags & CT_COLOR) glClearColor(cmd->red, cmd->green, cmd->blue, cmd->alpha);
					if(cmd->flags & CT_DEPTH) glClearDepthf(cmd->depth);
					glClear(cmd->flags);
				}
				break;
			case CT_SET_VIEWPORT:
				{
					CMD_SET_VIEWPORT* cmd = (CMD_SET_VIEWPORT*)cmd_base;
					glViewport(cmd->x, cmd->y, cmd->width, cmd->height);
				}
				break;
			case CT_SET_SCISSOR:
				{
					CMD_SET_SCISSOR* cmd = (CMD_SET_SCISSOR*)cmd_base;
          BindScissorTest(cmd->enable);
          if(cmd->enable) glScissor(cmd->x, cmd->y, cmd->width, cmd->height);
				}
				break;
			case CT_DRAW_PRIMITIVE:
				{
					CMD_DRAW_PRIMITIVE* cmd = (CMD_DRAW_PRIMITIVE*)cmd_base;
					glDrawElements(cmd->mode, cmd->count, cmd->type, 0);
				}
				break;
			default:
				GAssert("The type of command is error!\n");
				break;
			}
			GDELETE(cmd_base);
		}
		cmd_list.clear();
	}

	/*
	====================
	RISwap
	====================
	*/
	VOID RISwap()
	{
		s_cmd_ptr = (s_cmd_ptr + 1) % 2;
	}

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
