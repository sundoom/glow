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
#include "TextureData.h"
#include "ShaderData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

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

		// mAngle = 0.0f;
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
	  static Game s_game;
		return s_game;
	}

	//////////////////////////////////////////////////////////////////////////
  

	/*
	====================
	init
	====================
	*/
	VOID Game::init()
	{
    TextureData * tex = Resource::Find<TextureData>("texture/lena.tga");
    ShaderData * shader = Resource::Find<ShaderData>("shader/default.xml");
    
	}

	/*
	====================
	run
	====================
	*/
	VOID Game::run(F32 delta_time)
	{
    
	}

	/*
	====================
	exit
	====================
	*/
	VOID Game::exit()
	{
  
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	bind
	====================
	*/
	VOID Game::bind(U32 fbo)
	{
	
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
	
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
