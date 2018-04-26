/*
 *  Game.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "Game.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(XTom)

//////////////////////////////////////////////////////////////////////////

	Game::Game()
	{
		mFBO = 0;

		mWndWidth = 768;
		mWndHeight = 1024;	

		mFov = 90.0;
		mAspect = (F32)mWndWidth/(F32)mWndHeight;
		mZNear = 1.0;
		mZFar = 2000.0;

		mEye = Vector3(-3,0,3);
		mCenter = Vector3(0,0,0);
		mUp = Vector3(0,0,1);

		mAngle = 0.0f;
	}

	Game::~Game()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	instance
	====================
	*/ 
	Game& Game::instance()
	{
		GUARD(Game::instance);

		static Game s_game;
		return s_game;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	init
	====================
	*/
	VOID Game::init()
	{
		// create the graph context
		mGraphPtr = GNEW(Graph); CHECK(mGraphPtr);

		// test the pawn
		mPawnPtr = GNEW(Pawn); CHECK(mPawnPtr);
		{
			// set the animation set
			AnimSet* set = GNEW(AnimSet); CHECK(set);			
			{
				set->Load(GLoad("model/NPC_F006/NPC_F006.G3DA"));
			}
			mPawnPtr->SetAnimSet(set);

			// add the skeletal mesh
			SkeletalMesh* mesh = GNEW(SkeletalMesh); CHECK(mesh);
			{
				mesh->Load(GLoad("model/NPC_F006/NPC_F006.G3DS"));
			}
			mPawnPtr->AddSkeletalMesh("body", mesh);
		}
/*
		// create the shape
		mBoxPtr = GNEW(Box); CHECK(mBoxPtr);
		mCapsulePtr = GNEW(Capsule); CHECK(mCapsulePtr);
		mConePtr = GNEW(Cone); CHECK(mConePtr);
		mCylinderPtr = GNEW(Cylinder); CHECK(mCylinderPtr);
		mSpherePtr = GNEW(Sphere); CHECK(mSpherePtr);
		mConePtr->type(Primitive::PT_LINE_LOOP);
*/
		build();

		mAngle = 0.0f;
	}

	/*
	====================
	run
	====================
	*/
	VOID Game::run(F32 delta_time)
	{
		// reset the render context
		mGraphPtr->Reset();

		// draw the box
		// mBoxPtr->draw(mGraphPtr.Ptr());
		// mCapsulePtr->draw(mGraphPtr.Ptr());
		// mConePtr->draw(mGraphPtr.Ptr());
		// mCylinderPtr->draw(mGraphPtr.Ptr());
		// mSpherePtr->draw(mGraphPtr.Ptr());

		mAngle += 0.1f;

		mPawnPtr->Play("idle");
		mPawnPtr->Tick(delta_time);

		// draw the color rt
		{
			mGraphPtr->draw(static_cast<Operation*>(mColorRTPtr.Ptr()));

			// get the window rect
			Vector4 rect = Vector4(0.0f,0.0f,(F32)mColorTexPtr->Width(), (F32)mColorTexPtr->Height());

			// set the viewport
			mGraphPtr->viewport(rect);
			ViewportPtr viewport = GNEW(Viewport((I32)rect[0],(I32)rect[1],(I32)rect[2],(I32)rect[3])); CHECK(viewport);
			mGraphPtr->draw(static_cast<Operation*>(viewport.Ptr()));

			// set the scissor
			ScissorPtr scissor = GNEW(Scissor((I32)rect[0],(I32)rect[1],(I32)rect[2],(I32)rect[3]));
			mGraphPtr->draw(static_cast<Operation*>(scissor.Ptr()));

			// clear the target and the zbuffer
			ClearPtr clear = GNEW(Clear(Clear::CT_COLOR|Clear::CT_DEPTH,Vector4(0.0f,0.0f,0.0f,0.0f),1.0f)); CHECK(clear);
			mGraphPtr->draw(static_cast<Operation*>(clear.Ptr()));

			// make the projection and the modelview
			Matrix proj_mat = Matrix::makePerspective(mFov,mAspect,mZNear,mZFar);
			mGraphPtr->projection(proj_mat);
			Matrix view_mat = Matrix::makeLookAt(mEye,mCenter,mUp);
			mGraphPtr->view(view_mat);

			// draw the pawn
			{
				AUTO_LOCK_WORLD(mGraphPtr.Ptr());
				Quaternion quat; quat.setRotate(GMath::degToRad(mAngle),Vector3(0,0,1));
				Matrix world = Matrix::makeRotate(quat);
				mGraphPtr->world(world*mGraphPtr->world());
				mPawnPtr->Draw(mGraphPtr.Ptr());
			}
		}

		// draw the null rt
		{
			mGraphPtr->draw(static_cast<Operation*>(mNullRTPtr.Ptr()));

			// get the window rect
			Vector4 rect = Vector4(0,0,(F32)mWndWidth, (F32)mWndHeight);

			// set the viewport
			mGraphPtr->viewport(rect);
			ViewportPtr viewport = GNEW(Viewport((I32)rect[0],(I32)rect[1],(I32)rect[2],(I32)rect[3])); CHECK(viewport);
			mGraphPtr->draw(static_cast<Operation*>(viewport.Ptr()));

			// set the scissor
			ScissorPtr scissor = GNEW(Scissor((I32)rect[0],(I32)rect[1],(I32)rect[2],(I32)rect[3]));
			mGraphPtr->draw(static_cast<Operation*>(scissor.Ptr()));

			// clear the target and the zbuffer
			ClearPtr clear = GNEW(Clear(Clear::CT_COLOR|Clear::CT_DEPTH,Vector4(0.5f,0.4f,0.5f,1.0f),1.0f)); CHECK(clear);
			mGraphPtr->draw(static_cast<Operation*>(clear.Ptr()));

			// draw the pawn
			{
				AUTO_LOCK_WORLD(mGraphPtr.Ptr());

				// make the projection and the modelview
				Matrix proj_mat = Matrix::makePerspective(mFov,mAspect,mZNear,mZFar);
				mGraphPtr->projection(proj_mat);
				Matrix view_mat = Matrix::makeLookAt(mEye,mCenter,mUp);
				mGraphPtr->view(view_mat);
				
				Quaternion quat; quat.setRotate(GMath::degToRad(mAngle),Vector3(0,0,1));
				Matrix world = Matrix::makeRotate(quat);
				mGraphPtr->world(world*mGraphPtr->world());
				mPawnPtr->Draw(mGraphPtr.Ptr());
			}

			// draw the quad
			{
				AUTO_LOCK_WORLD(mGraphPtr.Ptr());

				// compute the ortho matrix
				F32 left, right, bottom, top, nearval, farval;
				left = -(F32)mWndWidth / 2.0f;
				right = (F32)mWndWidth / 2.0f;
				bottom = -(F32)mWndHeight / 2.0f;
				top = (F32)mWndHeight / 2.0f;
				nearval = 0.0f;
				farval = 1.0f;

				// make the projection and the modelview
				Matrix proj_mat = Matrix::makeOrtho(left, right, bottom, top, nearval, farval);
				mGraphPtr->projection(proj_mat);
				Matrix view_mat = Matrix::makeTranslate(Vector3(left,bottom,0.0));
				mGraphPtr->view(view_mat);
				mQuadPtr->GetConstant("gWVP")->SetMatrix(mGraphPtr->wvp());
				mGraphPtr->draw(static_cast<Operation*>(mQuadPtr.Ptr()));
			}
		}

		// swap the graph buffer
		mGraphPtr->swap();
	}

	/*
	====================
	exit
	====================
	*/
	VOID Game::exit()
	{
		if(mPawnPtr)mPawnPtr.Release();
		if(mColorTexPtr)mColorTexPtr.Release();
		if(mColorRTPtr)mColorRTPtr.Release();
		if(mNullRTPtr)mNullRTPtr.Release();
		if(mQuadPtr)mQuadPtr.Release();
/*		if(mBoxPtr)mBoxPtr.Release();
		if(mCapsulePtr)mCapsulePtr.Release();
		if(mConePtr)mConePtr.Release();
		if(mCylinderPtr)mCylinderPtr.Release();
		if(mSpherePtr)mSpherePtr.Release();		
		if(mGraphPtr) mGraphPtr.Release();
		*/
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	bind
	====================
	*/
	VOID Game::bind(U32 fbo)
	{
		GUARD(Game::bind);
	
		// build the fbo
		mNullRTPtr = GNEW(Target(fbo)); CHECK(mNullRTPtr);

		UNGUARD;
	}

	/*
	====================
	window
	====================
	*/
	VOID Game::window(U32 width, U32 height)
	{
		GUARD(Game::window);

		// set the window
		mWndWidth = width;
		mWndHeight = height;
		mAspect = (F32)mWndWidth/(F32)mWndHeight;

		UNGUARD;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	build
	====================
	*/
	VOID Game::build()
	{
		GUARD(Game::build);

		// build the color texture
		Image* image = GNEW(Image); CHECK(image);
		image->Width(256);
		image->Height(256);
		image->Depth(1);
		image->PixelFormat(PF_RGBA);
		image->DataType(DT_UNSIGNED_BYTE);
		mColorTexPtr = GNEW(Texture); CHECK(mColorTexPtr);
		mColorTexPtr->Load(image);

		// build the color rt
		mColorRTPtr = GNEW(Target(mColorTexPtr.Ptr())); CHECK(mColorRTPtr);

		// build the primitive
		mQuadPtr = GNEW(Primitive); CHECK(mQuadPtr);
		mQuadPtr->SetType(Primitive::PT_TRIANGLES);

		// set the wvp
		Constant* wvp_constant_ptr = GNEW(Constant); CHECK(wvp_constant_ptr);
		wvp_constant_ptr->SetMatrix(Matrix());
		mQuadPtr->SetConstant("gWVP",wvp_constant_ptr);

		// set the color texture
		Constant* texture_constant_ptr = GNEW(Constant); CHECK(texture_constant_ptr);
		texture_constant_ptr->SetTexture(mColorTexPtr.Ptr());
		mQuadPtr->SetConstant("gBaseTex",texture_constant_ptr);

		// set the shader
		Str shader_key_name = "shader/default.xml";
		KeyPtr shader_key_ptr = Key::Find(shader_key_name.c_str());
		if(shader_key_ptr == NULL)
		{
			Shader*shader = GNEW(Shader); CHECK(shader);
			shader->Load(GLoad(shader_key_name.c_str()));
			shader_key_ptr = GNEW(Key(shader_key_name.c_str(), shader)); CHECK(shader_key_ptr);
		}
		mKeys.push_back(shader_key_ptr);
		mQuadPtr->SetShader(dynamic_cast<Shader*>(shader_key_ptr->Ptr()),"p0");

		// build the vertex buffer
		F32 x = 0.0f, y = 0.0f, w = 256, h = 256;
		DVT vertexes[] = 
		{
			{x,		y,		0,		0,		0},
			{x+w,	y,		0,		1,		0},
			{x+w,	y+h,	0,		1,		1},
			{x,		y+h,	0,		0,		1},
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

		UNGUARD;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
