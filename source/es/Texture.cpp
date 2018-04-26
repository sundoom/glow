//////////////////////////////////////////////////////////////////////////

#include "Texture.h"
#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(RHI)

//////////////////////////////////////////////////////////////////////////

#if !defined(GL_IMG_texture_compression_pvrtc)
  #define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG		0x8C00
  #define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG		0x8C01
  #define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG		0x8C02
  #define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG		0x8C03
#endif

//////////////////////////////////////////////////////////////////////////

Texture::Texture()
{
	mTarget = GL_TEXTURE_2D;		
}

Texture::~Texture() 
{

}

//////////////////////////////////////////////////////////////////////////

/*
====================
Load
====================
*/
VOID Texture::Load(U32 level, U32 width, U32 height, U32 format, U32 type, const U8* pixels)
{
	// store the info
	mWidth = width;
	mHeight = height;
	mFormat = format;

	// generate the texture object
	CHECK(mObject == 0);
	glGenTextures(1, &mObject); CHECK(mObject);

	// bind the texture
	this->Bind();

	// commit the pixel data to the gpu memory
	for(U32 i = 0; i < level; i++)
	{
		U32 pixel_size = 0;
		switch(format)
		{
		case PF_ALPHA:
			pixel_size = width * height * sizeof(U8);
			glTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, format, type,  pixels);
			break;
		case PF_RGB:
			pixel_size = width * height * 3 * sizeof(U8);
			glTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, format, type,  pixels);
			break;
		case PF_RGBA:
			pixel_size = width * height * 4 * sizeof(U8);
			glTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, format, type,  pixels);
			break;
		case PF_L:
			pixel_size = width * height * sizeof(U8);
			glTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, format, type,  pixels);
			break;
		case PF_LA:
			pixel_size = width * height * 2 * sizeof(U8);
			glTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, format, type,  pixels);
			break;
		case PF_RGB_DXT1:
      pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
			glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, width, height, 0, pixel_size, pixels);
			break;
		case PF_RGBA_DXT1:
      pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
			glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, width, height, 0, pixel_size, pixels);
			break;
		case PF_RGBA_DXT3:      
      pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
			glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, width, height, 0, pixel_size, pixels);
			break;
		case PF_RGBA_DXT5:
      pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
			glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, width, height, 0, pixel_size, pixels);
			break;
		default:
			GAssert(VA("The texture`s format(%d) is unknown.\n", format));
			break;
		}
		pixels += pixel_size;
		width >>= 1; height >>= 1;
	}
}

/*
====================
Update
====================
*/
VOID Texture::Update(U32 level, I32 x, I32 y, U32 width, U32 height, U32 format, U32 type, const U8* pixels)
{
	CHECK(mObject);

	// bind the texture
	this->Bind();

	// commit the pixel data to the gpu memory
	for(U32 i = 0; i < level; i++)
	{
		U32 pixel_size = 0;
		switch(format)
		{
		case PF_ALPHA:
			pixel_size = width * height * sizeof(U8);
			glTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, format, type, pixels);
			break;
		case PF_RGB:
			pixel_size = width * height * 3 * sizeof(U8);
			glTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, format, type, pixels);
			break;
		case PF_RGBA:
			pixel_size = width * height * 4 * sizeof(U8);
			glTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, format, type, pixels);
			break;
		case PF_L:
			pixel_size = width * height * sizeof(U8);
			glTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, format, type, pixels);
			break;
		case PF_LA:
			pixel_size = width * height * 2 * sizeof(U8);
			glTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, format, type, pixels);
			break;
		case PF_RGB_DXT1:
      pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
			glCompressedTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, pixel_size, pixels);
			break;
		case PF_RGBA_DXT1:
      pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
			glCompressedTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, pixel_size, pixels);
			break;
		case PF_RGBA_DXT3:
      pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
			glCompressedTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, pixel_size, pixels);
			break;
		case PF_RGBA_DXT5:
      pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
			glCompressedTexSubImage2D(GL_TEXTURE_2D, i, x, y, width, height, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, pixel_size, pixels);
			break;
		default:
			GAssert(VA("The texture`s format(%d) is unknown.\n", format));
			break;
		}
		pixels += pixel_size;
		width >>= 1; height >>= 1;
	}
}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
