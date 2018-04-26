//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	Texture::Texture()
	{
		GUARD(Texture::Texture);

		mTarget = GL_TEXTURE_2D;
		
		UNGUARD;
	}

	Texture::~Texture() 
	{
		GUARD(Texture::~Texture);		

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Update
	====================
	*/
	VOID Texture::Update(const Image* image, BOOL created)
	{
		GUARD(Texture::Update);

		// get the format
		U32 width = image->Width();
		U32 height = image->Height();
		U32 pixel_format = image->PixelFormat();
		U32 data_type = image->DataType();

		// create the texture
		if(created) 
		{
			// if we require mip mapping.
			if(image->MipmapCount() > 0)
			{
				for(U32 i = 0; i < image->MipmapCount(); i++)
				{
					// get the mipmap
					U32 pixel_size = 0;
					const U8* pixels = image->Mipmap(i, pixel_size);
#if 0
					// upload the texture
					if( pixel_format == PF_RGB_DXT1 || 
						pixel_format == PF_RGBA_DXT1 || 
						pixel_format == PF_RGB_4BPPV1 || 
						pixel_format == PF_RGB_2BPPV1 ||
						pixel_format == PF_RGBA_4BPPV1 || 
						pixel_format == PF_RGBA_2BPPV1 )
					{
						glCompressedTexImage2D( 
							GL_TEXTURE_2D, 
							i, 
							pixel_format,
							width, 
							height, 
							0,
							pixel_size, 
							pixels );
					}
					else
					{
						glTexImage2D(
							GL_TEXTURE_2D, 
							i, 
							pixel_format,
							width, 
							height, 
							0, 
							pixel_format,
							data_type, 
							pixels );
					}
#else
					glTexImage2D(
						GL_TEXTURE_2D, 
						i, 
						pixel_format,
						width, 
						height, 
						0, 
						pixel_format,
						data_type, 
						pixels );
#endif
					width >>= 1; height >>= 1;
				}
			}
			else
			{
#if 0
				// upload the texture
				if( pixel_format == PF_RGB_DXT1 || 
					pixel_format == PF_RGBA_DXT1 || 
					pixel_format == PF_RGB_4BPPV1 || 
					pixel_format == PF_RGB_2BPPV1 ||
					pixel_format == PF_RGBA_4BPPV1 || 
					pixel_format == PF_RGBA_2BPPV1 )
				{
					glCompressedTexImage2D( 
						GL_TEXTURE_2D, 
						0, 
						pixel_format, 
						width, 
						height, 
						0, 
						0, 
						NULL );
				}
				else
				{
					glTexImage2D( 
						GL_TEXTURE_2D, 
						0, 
						pixel_format,
						width, 
						height, 
						0, 
						pixel_format, 
						data_type, 
						NULL );
				}
#else
				glTexImage2D( 
					GL_TEXTURE_2D, 
					0, 
					pixel_format,
					width, 
					height, 
					0, 
					pixel_format, 
					data_type, 
					NULL );
#endif
			}
		}
		// update the texture
		else
		{
			for(U32 i = 0; i < image->MipmapCount(); i++)
			{
				// get the mipmap
				U32 pixel_size = 0;
				const U8* pixels = image->Mipmap(i, pixel_size);
#if 0
				// upload the texture
				if( pixel_format == PF_RGB_DXT1 || 
					pixel_format == PF_RGBA_DXT1 || 
					pixel_format == PF_RGB_4BPPV1 || 
					pixel_format == PF_RGB_2BPPV1 ||
					pixel_format == PF_RGBA_4BPPV1 || 
					pixel_format == PF_RGBA_2BPPV1 )
				{
					glCompressedTexSubImage2D( 
						GL_TEXTURE_2D, 
						i, 
						0,
						0,
						width, 
						height, 
						pixel_format,
						pixel_size, 
						pixels );
				}
				else
				{
					glTexSubImage2D( 
						GL_TEXTURE_2D, 
						i, 
						0, 
						0, 
						width, 
						height, 
						pixel_format, 
						data_type,
						pixels );
				}
#else
				glTexSubImage2D( 
					GL_TEXTURE_2D, 
					i, 
					0, 
					0, 
					width, 
					height, 
					pixel_format, 
					data_type,
					pixels );
#endif
				width >>= 1; height >>= 1;
			}
		}
		
		UNGUARD;
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
