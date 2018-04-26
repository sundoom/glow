//////////////////////////////////////////////////////////////////////////

#include "Image.h"
#include <RHI.h>
NAMESPACE_USE(RHI)

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

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
		mMipmapCount = n;
	}

	U32 Image::MipmapCount() const
	{
		return mMipmapCount;
	}

	/*
	====================
	Mipmap
	====================
	*/
	VOID Image::Mipmap(const U8*data, U32 size)
	{
    mMipmaps.resize(size);
    if(data)::memcpy(&mMipmaps[0], data, size);	
	}

	const U8* Image::Mipmap(U32& size) const
	{
		size = mMipmaps.size();
		if(size > 0) return &mMipmaps[0];
		return NULL;
	}

	const U8* Image::Mipmap() const
	{
		if(mMipmaps.size() > 0) return &mMipmaps[0];
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
				image->Mipmap(NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader); 
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*)( colorMap + ( tga_header->ColorMapEntrySize + 4 ) / 8 * tga_header->ColorMapLength );
				U32* pixelData = (U32*)image->Mipmap();

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
				image->Mipmap(NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader); 
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap();
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
				image->Mipmap(NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;				
				U16*imageData = (U16*)( colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength );
				U32*pixelData = (U32*)image->Mipmap();
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
				image->Mipmap(NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8* imageData = (U8*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap();
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
				image->Mipmap(NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap();
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
				image->Mipmap(NULL,usize*vsize*sizeof(U32));

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGAHeader);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U16* imageData = (U16*) (colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap();
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

  //////////////////////////////////////////////////////////////////////////
  // DDS
  //////////////////////////////////////////////////////////////////////////

  #pragma pack(push,1)

    // DDPixelFormat flags
    enum 
    {
      DDPF_ALPHAPIXELS        = 0x00000001l,
      DDPF_ALPHA              = 0x00000002l,
      DDPF_FOURCC             = 0x00000004l,
      DDPF_RGB                = 0x00000040l,		
      DDPF_LUMINANCE          = 0x00020000l
    };

    struct DDPixelFormat
    {
      U32	Size;
      U32	Flags;
      U32	FourCC;
      U32	RGBBitCount;
      U32	RBitMask;
      U32	GBitMask;
      U32	BBitMask;
      U32	RGBAlphaBitMask;
    };	

    // DDCaps2 flags
    enum 
    {
      DDSCAPS_TEXTURE	= 0x00001000l,
      DDSCAPS_COMPLEX	= 0x00000008l,
      DDSCAPS_MIPMAP	= 0x00400000l,
      DDSCAPS2_VOLUME	= 0x00200000l
    };

    struct DDCaps2
    {
      U32	Caps1;
      U32	Caps2;
      U32	Reserved[2];
    };	

    // DDSurfaceDesc2 flags
    enum 
    {
      DDSD_CAPS			= 0x00000001l,
      DDSD_HEIGHT			= 0x00000002l,
      DDSD_WIDTH			= 0x00000004l,
      DDSD_PITCH			= 0x00000008l,
      DDSD_PIXELFORMAT	= 0x00001000l,		
      DDSD_MIPMAPCOUNT	= 0x00020000l,
      DDSD_LINEARSIZE		= 0x00080000l,
      DDSD_DEPTH			= 0x00800000l
    };

    struct DDSurfaceDesc2
    {
      U32	Size;
      U32	Flags;
      U32	Height;
      U32	Width;
      U32	PitchOrLinearSize;
      U32	Depth;
      U32	MipMapCount;
      U32	Reserved1[11];
      DDPixelFormat PixelFormat;
      DDCaps2	Caps;
      U32	Reserved2;
    };

  #define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))	
  #define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
  #define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

  #pragma pack(pop)

	/*
	====================
	loadDDS

	load dds image file
	====================
	*/
	Image* Image::LoadDDS(const GData* data)
	{
    // { GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, GL_RGBA, GL_UNSIGNED_BYTE, TRUE }, //	PF_DXT1
    // { GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, GL_RGBA, GL_UNSIGNED_BYTE, TRUE }, //	PF_DXT3
    // { GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, GL_RGBA, GL_UNSIGNED_BYTE, TRUE }, //	PF_DXT5

    CHECK(data);

    // get the data
    U32 data_pos = 0;
    const U8* data_ptr = (U8*)data->Ptr();

    // check the dds magic
    U32 magic = *(U32*)(data_ptr+data_pos);
    data_pos += sizeof(U32);
    CHECK(magic == MAKEFOURCC('D','D','S',' '));

    // get the DDSurfaceDesc2
    DDSurfaceDesc2& ddsd = *(DDSurfaceDesc2*)(data_ptr+data_pos);
    data_pos += sizeof(DDSurfaceDesc2);

    // check valid structure sizes
    CHECK(ddsd.Size == 124 && ddsd.PixelFormat.Size == 32);

    // get the image properties.
    U32 width = 0, height = 0, depth = 0;
    width = ddsd.Width;
    height = ddsd.Height;
    if( ddsd.Flags & DDSD_DEPTH ) depth = ddsd.Depth;

    // the pixel info
    U32 data_type = 0, pixel_format	= 0, pixel_size = 0;

    // handle some esoteric formats
    if(ddsd.PixelFormat.Flags & DDPF_ALPHA)
    {
      pixel_format  = PF_ALPHA;
      data_type		  = DT_UNSIGNED_BYTE;
      pixel_size		= 1;
    }
    else if(ddsd.PixelFormat.Flags & DDPF_FOURCC)
    {
      switch(ddsd.PixelFormat.FourCC)
      {
      case FOURCC_DXT1:
        if(ddsd.PixelFormat.Flags&DDPF_ALPHAPIXELS)
          pixel_format  = PF_RGBA_DXT1;
        else 
          pixel_format  = PF_RGB_DXT1;
        data_type		    = DT_UNSIGNED_BYTE;
        pixel_size		  = 1;
        break;
      case FOURCC_DXT3:
        pixel_format  = PF_RGBA_DXT3;
        data_type		  = DT_UNSIGNED_BYTE;
        pixel_size		= 2;
        break;
      case FOURCC_DXT5:
        pixel_format  = PF_RGBA_DXT5;
        data_type		  = DT_UNSIGNED_BYTE;
        pixel_size		= 2;
        break;
      default:
        GAssert( VA("The FourCC pixel format(%c%c%c%c) is not supported!",  
          (CHAR)((ddsd.PixelFormat.FourCC & 0x000000ff)),
          (CHAR)((ddsd.PixelFormat.FourCC & 0x0000ff00)>>8), 
          (CHAR)((ddsd.PixelFormat.FourCC & 0x00ff0000)>>16), 
          (CHAR)((ddsd.PixelFormat.FourCC & 0xff000000)>>24)) );
        return NULL; 
      }
    }
    else if(ddsd.PixelFormat.Flags & DDPF_RGB)
    {
      struct RGBFormat
      {
        const CHAR* name;
        U32 bit_count;
        U32 r_bit_mask;
        U32 g_bit_mask;
        U32 b_bit_mask;
        U32	a_bit_mask;        
        U32 pixel_format;
        U32 data_type;
      };
      static const RGBFormat rgb_formats[] =            
      {        
        { "RGB",			24,		0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000,	PF_RGB,		DT_UNSIGNED_BYTE  },        
        { "RGBA",		  32,		0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000,	PF_RGBA,	DT_UNSIGNED_BYTE  },         
      };
      BOOL found = FALSE;
      for(U32 i = 0; i < sizeof(rgb_formats)/sizeof(RGBFormat); i++)
      {
        const RGBFormat& f = rgb_formats[i];
        if(ddsd.PixelFormat.RGBBitCount     == f.bit_count &&
          ddsd.PixelFormat.RBitMask         == f.r_bit_mask &&
          ddsd.PixelFormat.GBitMask         == f.g_bit_mask &&
          ddsd.PixelFormat.BBitMask         == f.b_bit_mask &&
          ddsd.PixelFormat.RGBAlphaBitMask  == f.a_bit_mask )
        {
          if(f.pixel_format != 0 && f.data_type != 0)
          {
            pixel_format  = f.pixel_format;
            data_type     = f.data_type;
            pixel_size		= f.bit_count/8;
            found = TRUE;
            break;
          }
          else
          {
            GAssert( VA("The RGB pixel format(%s) is not supported!",f.name) );
            return NULL;                   
          }
        }
      }
      if(!found)
      {
        GAssert( VA("The RGB pixel format is not supported! ddsd.PixelFormat.RGBBitCount = %d ddsd.PixelFormat.RBitMask = 0x%x ddsd.PixelFormat.GBitMask = 0x%x ddsd.PixelFormat.BBitMask = 0x%x ddsd.PixelFormat.RGBAlphaBitMask = 0x%x",
          ddsd.PixelFormat.RGBBitCount, ddsd.PixelFormat.RBitMask, ddsd.PixelFormat.GBitMask, ddsd.PixelFormat.BBitMask, ddsd.PixelFormat.RGBAlphaBitMask) );
        return NULL;
      }
    }
    else if(ddsd.PixelFormat.Flags & DDPF_LUMINANCE)
    {
      pixel_format  = (ddsd.PixelFormat.Flags&DDPF_ALPHAPIXELS) ? PF_LA : PF_L;
      data_type		  = DT_UNSIGNED_BYTE;
      pixel_size		= (ddsd.PixelFormat.Flags&DDPF_ALPHAPIXELS) ? 2 : 1;
    }
    else
    {
      GAssert( VA("DDS` pixel format is error: 0x%x format is not supported!",ddsd.PixelFormat.Flags) );
      return NULL;
    }

    // the total of the pixel size
    U32 total_size = 0;
    U32 mipmap_count = 0;

    // take care of mipmaps if any.    
    if(ddsd.MipMapCount > 1)
    {
      // handle s3tc compressed mipmaps.
      if(ddsd.PixelFormat.Flags&DDPF_FOURCC)
      {
        CHECK(ddsd.PixelFormat.FourCC==FOURCC_DXT1 || ddsd.PixelFormat.FourCC==FOURCC_DXT3 || ddsd.PixelFormat.FourCC==FOURCC_DXT5);
        U32 w = ddsd.Width;
        U32 h = ddsd.Height;
        U32 block_size = (ddsd.PixelFormat.FourCC == FOURCC_DXT1) ? 8 : 16;
        for(U32 k = 1; k < ddsd.MipMapCount && (w || h); ++k)
        {
          if(w == 0) w = 1;
          if(h == 0) h = 1;
          total_size += (((w+3)/4) * ((h+3)/4) * block_size);
          w >>= 1;
          h >>= 1;
          mipmap_count++;
        }
      }
      // handle uncompressed mipmaps
      else
      {
        U32 w = ddsd.Width;
        U32 h = ddsd.Height;
        for(U32 k = 1; k < ddsd.MipMapCount && (w || h); ++k)
        {
          if(w == 0) w = 1;
          if(h == 0) h = 1;
          total_size += w * h * pixel_size;
          w >>= 1;
          h >>= 1;
          mipmap_count++;
        }
      }
    }
    else
    {
      mipmap_count = 1;
      if(ddsd.PixelFormat.Flags&DDPF_FOURCC)
      {
        CHECK(ddsd.PixelFormat.FourCC==FOURCC_DXT1 || ddsd.PixelFormat.FourCC==FOURCC_DXT3 || ddsd.PixelFormat.FourCC==FOURCC_DXT5);
        U32 block_size = (ddsd.PixelFormat.FourCC == FOURCC_DXT1) ? 8 : 16;
        total_size += (((width+3)/4) * ((height+3)/4) * block_size);
      }
      else
      {
        total_size = width * height * pixel_size;
      }
    }

    // build the image
    Image* image = GNEW(Image); CHECK(image);
    image->Width(width);
    image->Height(height);
    image->PixelFormat(pixel_format);
    image->DataType(data_type);
    image->MipmapCount(mipmap_count);
    image->Mipmap(data_ptr+data_pos,total_size);
		return image;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
