//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	DC::DC()
	{
		mDirty = FALSE;
		mWidth = mHeight = 0;
	}
	
	DC::~DC()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Width
	====================
	*/
	VOID DC::Width(U32 w)
	{
		if(mWidth != w)
		{
			mWidth = w;
			mDirty = TRUE;
		}
	}

	U32 DC::Width() const
	{
		return mWidth;
	}

	/*
	====================
	Height
	====================
	*/
	VOID DC::Height(U32 h)
	{
		if(mHeight != h)
		{
			mHeight = h;
			mDirty = TRUE;
		}
	}

	U32 DC::Height() const
	{
		return mHeight;
	}

	

	/*
	====================
	Draw
	====================
	*/
	VOID DC::Draw(const Bitmap* bitmap)
	{
		GUARD(DC::Draw);

		CHECK(bitmap);
		mBitmaps.push_back((Bitmap*)bitmap);

		UNGUARD;
	}

	/*
	====================
	Flush
	====================
	*/
	VOID DC::Flush(const Rect& rc)
	{
		GUARD(DC::Flush);

		// clip the dc screen
		Rect dc_rc;
		dc_rc.left = 0;
		dc_rc.right = mWidth;
		dc_rc.top = 0;
		dc_rc.bottom = mHeight;
		Rect new_rc;
		if(!IntersectRect(&new_rc, &rc, &dc_rc)) return;

		// clip the ranges
		std::list<Rect>ranges;
		for(std::list<Rect>::iterator it = mRanges.begin(); it != mRanges.end(); ++it)
		{
			Rect& old_rc = (*it);
			Rect clip_rc;
			if(IntersectRect(&clip_rc, &new_rc, &old_rc))
			{
				// rect : 1
				Rect rc1;
				rc1.left	= old_rc.left;
				rc1.right	= clip_rc.right;
				rc1.top		= old_rc.top;
				rc1.bottom	= clip_rc.top;
				if(!IsRectEmpty(&rc1)) ranges.push_back(rc1);

				// rect : 2
				Rect rc2;
				rc2.left	= clip_rc.right;
				rc2.right	= old_rc.right;
				rc2.top		= old_rc.top;
				rc2.bottom	= clip_rc.bottom;
				if(!IsRectEmpty(&rc2)) ranges.push_back(rc2);

				// rect : 3
				Rect rc3;
				rc3.left	= clip_rc.left;
				rc3.right	= old_rc.right;
				rc3.top		= clip_rc.bottom;
				rc3.bottom	= old_rc.bottom;
				if(!IsRectEmpty(&rc3)) ranges.push_back(rc3);

				// rect : 4
				Rect rc4;
				rc4.left	= old_rc.left;
				rc4.right	= clip_rc.left;
				rc4.top		= clip_rc.top;
				rc4.bottom	= old_rc.bottom;
				if(!IsRectEmpty(&rc4)) ranges.push_back(rc4);
			}
			else
			{
				ranges.push_back(old_rc);
			}
		}
		ranges.push_back(rc);
		mRanges = ranges;

		UNGUARD;
	}

	/*
	====================
	Swap
	====================
	*/
	VOID DC::Swap(Graph *gc)
	{
		GUARD(DC::Swap);

		// if it is dirty then build it
		if(mDirty) { Build(); mDirty = FALSE; }

		// calculate redraw rect
		Update();

		// paint all of the bitmap to the dc
		for(std::list<BitmapPtr>::iterator it = mBitmaps.begin(); it != mBitmaps.end(); ++it)
		{
			BitmapPtr& bitmap = (*it);
			Paint(bitmap.Ptr());
		}

		// update the texture
		mTexPtr->Load(mImagePtr.Ptr());

		// draw the scene primitive
		gc->draw(mQuadPtr.Ptr());

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Build
	====================
	*/
	VOID DC::Build()
	{
		GUARD(DC::Build);

		// get the texture stride
		U32 tex_width = 1, tex_height = 1;
		while((tex_width <<= 1) < mWidth);
		while((tex_height <<= 1) < mHeight);

		// build the texture
		if(mTexPtr==NULL){mTexPtr = GNEW(Texture);CHECK(mTexPtr);}
		if(mImagePtr==NULL){mImagePtr = GNEW(Image);CHECK(mImagePtr);}
		mImagePtr->Width(tex_width);
		mImagePtr->Height(tex_height);
		mImagePtr->Depth(1);
		mImagePtr->PixelFormat(PF_RGB);
		mImagePtr->DataType(DT_UNSIGNED_BYTE);
		mImagePtr->MipmapCount(1);
		mImagePtr->Mipmap(0,NULL,3*tex_width*tex_height*sizeof(U8));
		mTexPtr->Load(mImagePtr.Ptr());

		// build the scene primitive
		if(mQuadPtr==NULL)
		{
			mQuadPtr = GNEW(Primitive); CHECK(mQuadPtr);
			mQuadPtr->SetType(Primitive::PT_TRIANGLES);

			// set the color texture
			Constant* texture_constant_ptr = GNEW(Constant); CHECK(texture_constant_ptr);
			texture_constant_ptr->SetTexture(mTexPtr.Ptr());
			mQuadPtr->SetConstant("gBaseTex",texture_constant_ptr);

			// set the shader
			Str shader_name = "shader/dc.xml";
			KeyPtr shader_key_ptr = Key::Find(shader_name.c_str());
			if(shader_key_ptr == NULL)
			{
				Shader*shader = GNEW(Shader); CHECK(shader);
				shader->Load(GLoad(shader_name.c_str()));
				shader_key_ptr = GNEW(Key(shader_name.c_str(), shader)); CHECK(shader_key_ptr);
			}
			mKeys.push_back(shader_key_ptr);
			mQuadPtr->SetShader(dynamic_cast<Shader*>(shader_key_ptr->Ptr()), "p0");

			// build the vertex buffer
			F32 s = (F32)mWidth/(F32)tex_width, t = (F32)mHeight/(F32)tex_height;
			DVT vertexes[] = 
			{
				{-1,	-1,		0,		0,		1},
				{1,		-1,		0,		s,		1},
				{1,		1,		0,		s,		1-t},
				{-1,	1,		0,		0,		1-t},
			};	
			VertexBufferPtr vb_ptr = GNEW(VertexBuffer); CHECK(vb_ptr);
			{
				GDataPtr vd_ptr = GNEW(GData); CHECK(vd_ptr);
				vd_ptr->Size(3*sizeof(U32) + 3*sizeof(U8) + sizeof(vertexes));
				U8*data_ptr = (U8*)vd_ptr->Ptr();
				*(U32*)data_ptr = MAKEFOURCC('G','V','B','O');
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = sizeof(vertexes)/sizeof(DVT); 
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = sizeof(DVT);
				data_ptr += sizeof(U32);
				*(U8*)data_ptr = 2;
				data_ptr += sizeof(U8);
				*(U8*)data_ptr = VertexBuffer::VT_3F;
				data_ptr += sizeof(U8);
				*(U8*)data_ptr = VertexBuffer::VT_2F;
				data_ptr += sizeof(U8);
				::memcpy(data_ptr, vertexes, sizeof(vertexes));
				data_ptr += sizeof(vertexes);
				vb_ptr->Load(vd_ptr.Ptr());
			}
			mQuadPtr->SetVertexBuffer(vb_ptr.Ptr());

			// build the index
			const U16 indexes[] = { 3, 0, 2, 2, 0, 1 };
			IndexBufferPtr ib_ptr = GNEW(IndexBuffer); CHECK(ib_ptr);
			{
				GDataPtr id_ptr = GNEW(GData); CHECK(id_ptr);
				id_ptr->Size(3*sizeof(U32) + sizeof(indexes));
				U8*data_ptr = (U8*)id_ptr->Ptr();
				*(U32*)data_ptr = MAKEFOURCC('G','I','B','O');
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = sizeof(indexes)/sizeof(U16); 
				data_ptr += sizeof(U32);
				*(U32*)data_ptr = sizeof(U16); 
				data_ptr += sizeof(U32);
				::memcpy(data_ptr, &indexes[0], sizeof(indexes));
				data_ptr += sizeof(indexes);
				ib_ptr->Load(id_ptr.Ptr());
			}		
			mQuadPtr->SetIndexBuffer(ib_ptr.Ptr());	

			// build the bounding box
			BoundingBox box;
			box.set(MAX_F32,MAX_F32,MAX_F32,MIN_F32,MIN_F32,MIN_F32);
			for(U32 i = 0; i < sizeof(vertexes)/sizeof(DVTN); i++)box.expand(vertexes[i].point);
			mQuadPtr->SetBox(box);
		}
		else
		{
			// update the vertex buffer
			VertexBufferPtr vb_ptr = mQuadPtr->GetVertexBuffer();
			CHECK(vb_ptr);
			F32 s = (F32)mWidth/(F32)tex_width, t = (F32)mHeight/(F32)tex_height;
			DVT vertexes[] = 
			{
				{-1,	-1,		0,		0,		1},
				{1,		-1,		0,		s,		1},
				{1,		1,		0,		s,		1-t},
				{-1,	1,		0,		0,		1-t},
			};
			GDataPtr vd_ptr = GNEW(GData); CHECK(vd_ptr);
			vd_ptr->Size(3*sizeof(U32) + 3*sizeof(U8) + sizeof(vertexes));
			U8*data_ptr = (U8*)vd_ptr->Ptr();
			*(U32*)data_ptr = MAKEFOURCC('G','V','B','O');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(vertexes)/sizeof(DVT); 
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = sizeof(DVT);
			data_ptr += sizeof(U32);
			*(U8*)data_ptr = 2;
			data_ptr += sizeof(U8);
			*(U8*)data_ptr = VertexBuffer::VT_3F;
			data_ptr += sizeof(U8);
			*(U8*)data_ptr = VertexBuffer::VT_2F;
			data_ptr += sizeof(U8);
			::memcpy(data_ptr, vertexes, sizeof(vertexes));
			data_ptr += sizeof(vertexes);
			vb_ptr->Load(vd_ptr.Ptr());

			// build the bounding box
			BoundingBox box;
			box.set(MAX_F32,MAX_F32,MAX_F32,MIN_F32,MIN_F32,MIN_F32);
			for(U32 i = 0; i < sizeof(vertexes)/sizeof(DVTN); i++)box.expand(vertexes[i].point);
			mQuadPtr->SetBox(box);
		}

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	====================
	Update
	====================
	*/
	VOID DC::Update()
	{
		GUARD(DC::Update);

		// update all of the bitmap area
		for(std::list<BitmapPtr>::iterator it = mBitmaps.begin(); it != mBitmaps.end(); ++it)
		{
			BitmapPtr& bitmap = (*it);
			if(bitmap->mDirty)
			{
				// flush old rect
				Rect old_rc;
				old_rc.left = bitmap->mX2;
				old_rc.right = bitmap->mX2 + bitmap->mW2;
				old_rc.top = bitmap->mY2;
				old_rc.bottom = bitmap->mY2 + bitmap->mH2;
				if(!IsRectEmpty(&old_rc)) Flush(old_rc);

				// flush new rect
				Rect new_rc;
				new_rc.left = bitmap->mX1;
				new_rc.right = bitmap->mX1 + bitmap->mW1;
				new_rc.top = bitmap->mY1;
				new_rc.bottom = bitmap->mY1 + bitmap->mH1;
				if(!IsRectEmpty(&new_rc)) Flush(new_rc);

				// reset the state
				bitmap->mDirty = FALSE;
			}
		}

		UNGUARD;
	}

	/*
	====================
	Paint
	====================
	*/
	VOID DC::Paint(const Bitmap*bitmap)
	{
		GUARD(DC::Paint);

		for(std::list<Rect>::iterator it = mRanges.begin(); it != mRanges.end(); ++it)
		{
			Rect& dst_rc = (*it);
			Rect src_rc;
			src_rc.left = bitmap->mX1;
			src_rc.right = bitmap->mX1 + bitmap->mW1;
			src_rc.top = bitmap->mY1;
			src_rc.bottom = bitmap->mY1 + bitmap->mH1;
			Rect clip_rc;
			if(!IntersectRect(&clip_rc, &dst_rc, &src_rc))continue;

			I32 src_x = clip_rc.left - src_rc.left;
			I32 src_y = clip_rc.top - src_rc.top;
			U32 src_w = bitmap->mW1;
			U32 src_h = bitmap->mH1;
			U8*src_data = bitmap->mData;

			I32 dst_x = clip_rc.left;
			I32 dst_y = clip_rc.top;
			U32 dst_w = mImagePtr->Width();
			U32 dst_h = mImagePtr->Height();
			U8*dst_data = (U8*)mImagePtr->Mipmap(0);

			I32 width = clip_rc.right - clip_rc.left;
			I32 height = clip_rc.bottom - clip_rc.top;
			if(width <= 0 || height <= 0) continue;

			switch(bitmap->mFormat)
			{
			case PF_RGB:
				{
					for(I32 j = 0; j < height; j++)
					{
						for(I32 i = 0; i < width; i++)
						{
							U8&sb = src_data[(src_x+i)*3 + (src_y+j)*src_w*3];
							U8&sg = *(&sb+1);
							U8&sr = *(&sb+2);
							U8&dr = dst_data[(dst_x+i)*3 + (dst_y+j)*dst_w*3];
							U8&dg = *(&dr+1);
							U8&db = *(&dr+2);
							dr = sr;
							dg = sg;
							db = sb;
						}
					}
				}
				break;
			case PF_RGBA:
				{
					for(I32 j = 0; j < height; j++)
					{
						for(I32 i = 0; i < width; i++)
						{
							U8&sb = src_data[(src_x+i)*4 + (src_y+j)*src_w*4];
							U8&sg = *(&sb+1);
							U8&sr = *(&sb+2);
							U8&sa = *(&sb+3);
							U8&dr = dst_data[(dst_x+i)*3 + (dst_y+j)*dst_w*3];
							U8&dg = *(&dr+1);
							U8&db = *(&dr+2);
							dr = (sr - dr)*sa/255 + dr;
							dg = (sg - dg)*sa/255 + dg;
							db = (sb - db)*sa/255 + db;
						}
					}
				}
				break;
			}
		}

		UNGUARD;
	}

	

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
