//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	#pragma pack(push,1)
	struct TGAHeader
	{
		U8	IdFieldLength;
		U8	ColorMapType;
		U8	ImageTypeCode;		// 2 for uncompressed RGB format
		U16	ColorMapOrigin;
		U16	ColorMapLength;
		U8	ColorMapEntrySize;
		U16	XOrigin;
		U16	YOrigin;
		U16	Width;
		U16	Height;
		U8	BitsPerPixel;
		U8	ImageDescriptor;
	};
	#pragma pack(pop)

	//////////////////////////////////////////////////////////////////////////

	Image::Image()
	{
		mWidth = mHeight = mDepth = 0;
		mPixelFormat = PF_RGBA;
		mDataType = DT_UNSIGNED_BYTE;
	}

	Image::~Image()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Width
	====================
	*/
	VOID Image::Width(U32 w)
	{
		mWidth = w;
	}

	U32 Image::Width() const
	{
		return mWidth;
	}

	/*
	====================
	Height
	====================
	*/
	VOID Image::Height(U32 h)
	{
		mHeight = h;
	}

	U32 Image::Height() const
	{
		return mHeight;
	}

	/*
	====================
	Depth
	====================
	*/
	VOID Image::Depth(U32 d)
	{
		mDepth = d;
	}

	U32 Image::Depth() const
	{
		return mDepth;
	}

	/*
	====================
	PixelFormat
	====================
	*/
	VOID Image::PixelFormat(U32 f)
	{
		mPixelFormat = f;
	}

	U32 Image::PixelFormat() const
	{
		return mPixelFormat;
	}

	/*
	====================
	DataType
	====================
	*/
	VOID Image::DataType(U32 t)
	{
		mDataType = t;
	}

	U32 Image::DataType() const
	{
		return mDataType;
	}

	/*
	====================
	MipmapCount
	====================
	*/
	VOID Image::MipmapCount(U32 n)
	{
		mMipmaps.resize(n);
	}

	U32 Image::MipmapCount() const
	{
		return mMipmaps.size();
	}

	/*
	====================
	Mipmap
	====================
	*/
	VOID Image::Mipmap(U32 level, const U8*data, U32 size)
	{
		if (level<mMipmaps.size())
		{
			std::vector<U8>&pixels = mMipmaps[level];
			pixels.resize(size);
			if(data)::memcpy(&pixels[0], data, size);
		}
	}

	const U8* Image::Mipmap(U32 level, U32& size) const
	{
		if(level < mMipmaps.size())
		{
			size = mMipmaps[level].size();
			if(size > 0) return &mMipmaps[level][0];
		}
		return NULL;
	}

	const U8* Image::Mipmap(U32 level) const
	{
		if(level < mMipmaps.size())
		{
			if(mMipmaps[level].size() > 0) return &mMipmaps[level][0];
		}
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load

	load tga and dds file
	====================
	*/
	Image* Image::Load(const GData* data)
	{
		CHECK(data);
		GDataPtr data_ptr = (GData*)data;

		// check the magic
		U32 magic = *(U32*)data_ptr->Ptr();
		if(magic == MAKEFOURCC('D','D','S',' '))
		{
			return LoadDDS(data);
		}
		else
		{
			return LoadTGA(data);
		}

		return NULL;
	}

	/*
	====================
	loadTGA

	load tga image file
	====================
	*/
	Image* Image::LoadTGA(const GData* data)
	{
		CHECK(data);

		// get the header of the tga
		const TGAHeader* tga_header = (TGAHeader*)data->Ptr();
		if( tga_header->ImageTypeCode == 10 ) // 10 = RLE compressed 
		{
			if( tga_header->BitsPerPixel == 32 )
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for( I32 i = 1; i < usize; i <<= 1 ) ubits++;	
				for( I32 j = 1; j < vsize; j <<= 1 ) vbits++;
				if( usize != (1 << ubits) || vsize != (1 << vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Width(usize);
				image->Height(vsize);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(1);
				image->Mipmap(0,NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader); 
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*)( colorMap + ( tga_header->ColorMapEntrySize + 4 ) / 8 * tga_header->ColorMapLength );
				U32* pixelData = (U32*)image->Mipmap(0);

				U8	pixel[4];
				I32 rleRun = 0;
				I32 rawRun = 0;

				for( I32 Y = tga_header->Height - 1; Y >= 0; Y-- ) // Y-flipped.
				{					
					for( I32 X = 0; X < tga_header->Width; X++ )
					{						
						if( rleRun > 0 )
						{
							rleRun--;  // reuse current Pixel data.
						}
						else if( rawRun == 0 ) // new raw pixel or RLE-run.
						{
							U8 rleChunk = *(imageData++);							
							if( rleChunk & 0x80 )
							{
								rleRun = ( rleChunk & 0x7F ) + 1;
								rawRun = 1;
							}
							else
							{
								rawRun = ( rleChunk & 0x7F ) + 1;
							}
						}							
						// Retrieve new pixel data - raw run or single pixel for RLE stretch.
						if( rawRun > 0 )
						{
							pixel[2] = imageData[0];
							pixel[1] = imageData[1];
							pixel[0] = imageData[2];
							pixel[3] = imageData[3];
							imageData += 4;
							rawRun--;
							rleRun--;
						}
						// Store.
						*( (pixelData + Y*tga_header->Width)+X ) = *(U32*)&pixel;
					}
				}

				return image;
			}
			else if( tga_header->BitsPerPixel == 24 )
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for( I32 i = 1; i < usize; i <<= 1 ) ubits++;	
				for( I32 j = 1; j < vsize; j <<= 1 ) vbits++;
				if( usize != (1 << ubits) || vsize != (1 << vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL;}

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Width(usize);
				image->Height(vsize);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(1);
				image->Mipmap(0,NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader); 
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap(0);
				U8 pixel[4];
				I32 rleRun = 0;
				I32 rawRun = 0;

				for( I32 Y = tga_header->Height - 1; Y >= 0; Y-- ) // Y-flipped.
				{					
					for( I32 X = 0;X < tga_header->Width; X++ )
					{						
						if( rleRun > 0 )
						{
							rleRun--;  // reuse current Pixel data.
						}
						else if( rawRun == 0 ) // new raw pixel or RLE-run.
						{
							U8 rleChunk = *(imageData++);
							if( rleChunk & 0x80 )
							{
								rleRun = ( rleChunk & 0x7F ) + 1;
								rawRun = 1;
							}
							else
							{
								rawRun = ( rleChunk & 0x7F ) + 1;
							}
						}							
						// Retrieve new pixel data - raw run or single pixel for RLE stretch.
						if( rawRun > 0 )
						{
							pixel[2] = *(imageData++);
							pixel[1] = *(imageData++);
							pixel[0] = *(imageData++);
							pixel[3] = 255;
							rawRun--;
							rleRun--;
						}
						// Store.
						*( (pixelData + Y*tga_header->Width)+X ) = *(U32*)&pixel;
					}
				}

				return image;
			}
			else if( tga_header->BitsPerPixel == 16 )
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for( I32 i = 1; i < usize; i <<= 1 ) ubits++;	
				for( I32 j = 1; j < vsize; j <<= 1 ) vbits++;
				if( usize != (1 << ubits) || vsize != (1 << vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Width(usize);
				image->Height(vsize);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(1);
				image->Mipmap(0,NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;				
				U16*imageData = (U16*)( colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength );
				U32*pixelData = (U32*)image->Mipmap(0);
				U8 pixel[4];
				I32 rleRun = 0;
				I32 rawRun = 0;

				for( I32 Y = tga_header->Height - 1; Y >= 0; Y-- ) // Y-flipped.
				{					
					for( I32 X = 0; X < tga_header->Width; X++ )
					{						
						if( rleRun > 0 )
						{
							rleRun--;  // reuse current Pixel data.
						}
						else if( rawRun == 0 ) // new raw pixel or RLE-run.
						{
							U8 rleChunk =  *( (U8*)imageData );
							imageData = (U16*)( ( (U8*)imageData ) + 1 );
							if( rleChunk & 0x80 )
							{
								rleRun = ( rleChunk & 0x7F ) + 1;
								rawRun = 1;
							}
							else
							{
								rawRun = ( rleChunk & 0x7F ) + 1;
							}
						}							
						// Retrieve new pixel data - raw run or single pixel for RLE stretch.
						if( rawRun > 0 )
						{ 
							U16 color = *(imageData++);
							pixel[0] = (( color >> 10 ) & 0x1f) << 3;
							pixel[1] = (( color >> 5 ) & 0x1f) << 3;
							pixel[2] = (color & 0x1f) << 3;
							pixel[3] = 255;
							rawRun--;
							rleRun--;
						}
						// Store.
						*( (pixelData + Y*tga_header->Width)+X ) = *(U32*)&pixel;
					}
				}

				return image;
			}
			else
			{
				GAssert( VA( "TGA header uses an unsupported rle-compressed bit-depth: %u", tga_header->BitsPerPixel ) );
				return NULL;
			}
		}
		else if( tga_header->ImageTypeCode == 2 ) // 2 = Uncompressed 
		{
			if( tga_header->BitsPerPixel == 32 )
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for( I32 i = 1; i < usize; i <<= 1 ) ubits++;	
				for( I32 j = 1; j < vsize; j <<= 1 ) vbits++;
				if( usize != (1 << ubits) || vsize != (1 << vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Width(usize);
				image->Height(vsize);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(1);
				image->Mipmap(0,NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8* imageData = (U8*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap(0);
				U8 pixel[4];

				for( I32 Y = 0; Y < tga_header->Height; Y++ )
				{
					for( I32 X = 0; X < tga_header->Width; X++ )
					{
						pixel[2] = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width * 4 ) + X * 4 + 0 );
						pixel[1] = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width * 4 ) + X * 4 + 1 );
						pixel[0] = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width * 4 ) + X * 4 + 2 );
						pixel[3] = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width * 4 ) + X * 4 + 3 );
						*( ( pixelData + Y * tga_header->Width ) + X ) = *(U32*)&pixel;
					}
				}

				return image;
			}
			else if(tga_header->BitsPerPixel == 24)
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for( I32 i = 1; i < usize; i <<= 1 ) ubits++;	
				for( I32 j = 1; j < vsize; j <<= 1 ) vbits++;
				if( usize != (1 << ubits) || vsize != (1 << vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Width(usize);
				image->Height(vsize);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(1);
				image->Mipmap(0,NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap(0);
				U8 pixel[4];

				for( I32 Y = 0; Y < tga_header->Height; Y++ )
				{
					for( I32 X = 0; X < tga_header->Width; X++ )
					{
						pixel[2] = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width * 3 ) + X * 3 + 0 );
						pixel[1] = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width * 3 ) + X * 3 + 1 );
						pixel[0] = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width * 3 ) + X * 3 + 2 );
						pixel[3] = 255;
						*( ( pixelData + Y * tga_header->Width ) + X ) = *(U32*)&pixel;
					}
				}

				return image;
			}
			else if( tga_header->BitsPerPixel == 16 )
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for( I32 i = 1; i < usize; i <<= 1 ) ubits++;	
				for( I32 j = 1; j < vsize; j <<= 1 ) vbits++;
				if( usize != (1 << ubits) || vsize != (1 << vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Width(usize);
				image->Height(vsize);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(1);
				image->Mipmap(0,NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U16* imageData = (U16*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap(0);
				U8 pixel[4];

				for( I32 Y = 0; Y < tga_header->Height; Y++ )
				{
					for( I32 X = 0; X < tga_header->Width; X++ )
					{
						U16 color = *(( imageData + ( tga_header->Height - Y - 1 ) * tga_header->Width ) + X );
						pixel[0] = (( color >> 10 ) & 0x1f) << 3;
						pixel[1] = (( color >> 5 ) & 0x1f) << 3;
						pixel[2] = (color & 0x1f) << 3;
						pixel[3] = 255;
						*( ( pixelData + Y * tga_header->Width ) + X ) = *(U32*)&pixel;
					}
				}

				return image;
			}
			else
			{
				GAssert( VA("TGA uses an unsupported bit-depth: %u" ,tga_header->BitsPerPixel) );
				return NULL;
			}
		}
		else
		{
			GAssert( VA("TGA is an unsupported type: %u", tga_header->ImageTypeCode) );
			return NULL;
		}

		return NULL;
	}

	/*
	====================
	loadDDS

	load dds image file
	====================
	*/
	Image* Image::LoadDDS(const GData* data)
	{
		return NULL;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
