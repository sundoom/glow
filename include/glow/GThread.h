
/*
 *  GThread.h
 *  glow
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __GThread__
#define __GThread__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glow.h>

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented thread interface.
class GThread
{
public:
  // enumerated type for thread priority
  enum PriorityType 
  {
    THREAD_PRIORITY_MAX,      // The maximum possible priority
    THREAD_PRIORITY_HIGH,     // A high (but not max) setting
    THREAD_PRIORITY_NOMINAL,  // An average priority
    THREAD_PRIORITY_LOW,      // A low (but not min) setting
    THREAD_PRIORITY_MIN,      // The miniumum possible priority
    THREAD_PRIORITY_DEFAULT   // Priority scheduling default
  };

public:		
  // constructor
  GThread();

  // destructor
  virtual ~GThread();

  // start the thread
  VOID start(U32 priority = THREAD_PRIORITY_NOMINAL, U32 size = 0);

  // end the thread
  VOID end();

protected:
  // thread's run method.  must be implemented by derived classes.
  // this is where the action happens.
  virtual VOID run() = 0;

private:
  // the thread hook
  static U32 __stdcall hook(VOID* arg);

private:
  HANDLE	mHandle;		
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GThread__

//////////////////////////////////////////////////////////////////////////