/*
 *  Main.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////
// windows下shader里uniform的数组名都加了[0],如gBones[0]
//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include <GMath.h>
NAMESPACE_USE(Math)
#include <Render.h>
NAMESPACE_USE(Render)

#include <EGL/egl.h>
#include "Window.h"

#include "../Game/Game.h"

//////////////////////////////////////////////////////////////////////////

static WindowPtr gWndPtr;
static U32 gWndWidth = 1024;
static U32 gWndHeight = 768;

//////////////////////////////////////////////////////////////////////////

static VOID appInit();
static VOID appRun();
static VOID appExit();

//////////////////////////////////////////////////////////////////////////

/*
====================
main
====================
*/ 
I32 main(I32 argc, CHAR** argv)
{
	// register the window class
	WNDCLASSEX wcex;	
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)Window::wndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= ::GetModuleHandle(NULL);
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)WHITE_BRUSH;
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= "GWindow";
	wcex.hIconSm		= 0;
	CHECK(::RegisterClassEx(&wcex));

	appInit();

	// the message loop
	MSG msg; ::memset(&msg,0,sizeof(msg));
	while(msg.message != WM_QUIT) 
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		} 
		else 
		{
			appRun();

			// sleep it
			::Sleep(1);
		}
	}

	appExit();	

	// unregister the window class
	::UnregisterClass("GWindow", ::GetModuleHandle(NULL));

	return 0;
}

//////////////////////////////////////////////////////////////////////////

static EGLDisplay gDisplay;
static EGLContext gContext;
static EGLSurface gSurface;
/*
====================
appInit
====================
*/
static VOID appInit()
{
	// create the window
	gWndPtr = GNEW(Window); CHECK(gWndPtr);
	gWndPtr->SetText("Glow");
	gWndPtr->move(gWndWidth, gWndHeight);

	// Get Display
	gDisplay = eglGetDisplay(::GetDC(gWndPtr->handle()));
	CHECK(gDisplay!=EGL_NO_DISPLAY);

	// Initialize EGL
	EGLint major, minor;
	CHECK(eglInitialize(gDisplay, &major, &minor));

	// Get configs
	EGLint num_configs;
	CHECK(eglGetConfigs(gDisplay, NULL, 0, &num_configs));

	// Choose config
	EGLint attribs[] =
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_NONE
	};
	EGLConfig config;
	CHECK(eglChooseConfig(gDisplay, attribs, &config, 1, &num_configs));

	// Create a surface
	gSurface = eglCreateWindowSurface(gDisplay, config, (EGLNativeWindowType)gWndPtr->handle(), NULL);
	CHECK(gSurface!=EGL_NO_SURFACE);

	// Create a GL context
	EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	gContext = eglCreateContext(gDisplay, config, EGL_NO_CONTEXT, context_attribs);
	CHECK(gContext!=EGL_NO_CONTEXT)

	// Make the gContext current
	CHECK(eglMakeCurrent(gDisplay, gSurface, gSurface, gContext));

	// init the game
	Game::instance().init();
	Game::instance().bind(0);
	Game::instance().window(gWndWidth,gWndHeight);
}

/*
====================
appRun
====================
*/
static VOID appRun()
{
	// get the frame time
	static F64 system_time = GSeconds();
	F64 current_time = GSeconds();
	F64 frame_time = current_time - system_time;
	if(frame_time == 0) return;
	system_time = current_time;

	// run the game
	Game::instance().run((F32)frame_time);

	// swap buffers
	eglSwapBuffers(gDisplay, gSurface);
}

/*
====================
appExit
====================
*/
static VOID appExit()
{
	// exit the game
	Game::instance().exit();

	// release the egl
	if(gDisplay!=EGL_NO_DISPLAY) 
	{
		eglMakeCurrent(gDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if(gContext!=EGL_NO_CONTEXT)eglDestroyContext(gDisplay, gContext);		
		if(gContext!=EGL_NO_SURFACE)eglDestroySurface(gDisplay, gSurface);
		eglTerminate(gDisplay);
	}

	// release the window
	if(gWndPtr) gWndPtr.Release();
}

//////////////////////////////////////////////////////////////////////////

/*
====================
GLoad
====================
*/
const GData* GLoad(const CHAR* path)
{
	GFile file;
	file.Open(path);
	U32 pos = file.Seek(0,FS_CUR);
	U32 size = file.Seek(0,FS_END);	
	file.Seek(pos,FS_SET);
	GData* data = GNEW(GData(size+1,0)); CHECK(data);
	CHECK(file.Read(data->Ptr(),data->Size())==size);
	file.Close();
	return data;
}

//////////////////////////////////////////////////////////////////////////