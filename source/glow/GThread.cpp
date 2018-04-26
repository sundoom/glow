/*
 *  GThread.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glow.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

//////////////////////////////////////////////////////////////////////////

#include <process.h>

//////////////////////////////////////////////////////////////////////////

GThread::GThread() 
{
	mHandle = NULL;
}

GThread::~GThread() 
{
	
}

/*
====================
start
====================
*/
VOID GThread::start(U32 priority, U32 size)
{
	// create the thread
	U32 id;
	mHandle = reinterpret_cast<HANDLE>(::_beginthreadex(0, size, hook, this, CREATE_SUSPENDED, &id));
	if(mHandle == 0)
	{
		GAssert(VA("Fail to create a new thread and the error code is %d!", ::GetLastError()));
	}

	// set the thread priority
	I32 prio = THREAD_PRIORITY_NORMAL;
	switch(priority) 
	{
	case THREAD_PRIORITY_MAX:
		prio = THREAD_PRIORITY_HIGHEST;
		break;
	case THREAD_PRIORITY_HIGH:
		prio = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	case THREAD_PRIORITY_NOMINAL:
		prio = THREAD_PRIORITY_NORMAL;
		break;
	case THREAD_PRIORITY_LOW:
		prio = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case THREAD_PRIORITY_MIN:
		prio = THREAD_PRIORITY_IDLE;
		break;
	}
	if(::SetThreadPriority(mHandle, prio) == 0)
	{
		GAssert(VA("Fail to set the thread priority and the error code is %d!", ::GetLastError()));
	}

	// resume the thread
	if(::ResumeThread(mHandle) == -1)
	{
		GAssert(VA("Fail to resume the thread and the error code is %d!", ::GetLastError()));
	}
}

/*
====================
end
====================
*/
VOID GThread::end()
{
  // terminate the thread and close the thread handle
	if(mHandle)
	{
		// wait until the controlled thread terminates.
		if(::WaitForSingleObject(mHandle, INFINITE) != WAIT_OBJECT_0)
		{
			GAssert( VA("Fail to terminate the thread and the error code is %d!", ::GetLastError()) );
		}

		// close the thread handle
		if(::CloseHandle(mHandle) == 0)
		{
			GAssert( VA("Fail to close the thread handle and the error code is %d!", ::GetLastError()) );
		}

		mHandle = NULL;
	}
}

/*
====================
hook
====================
*/
U32 __stdcall GThread::hook(VOID * arg) 
{
	try
	{
		// the thread run
		static_cast<GThread*>(arg)->run();
	}
	catch (...)
	{
		GAssert("The thread run is error!");
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////