#ifndef __SAMPLES_THREAD_H__
#define __SAMPLES_THREAD_H__
//////////////////////////////////////////////////////////////////////////////
//
//    THREAD.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      Multithreading common classes.

    TODO: 
      + Code Review 2006-08-20
        ----------------------
        (Portability):
        Find & replace with a cross platform threading library since this is currently windows only

        (Performance + Correctness):
        Possibly replace locking mechanism with the volatile keyword (see http://msdn2.microsoft.com/en-us/library/12a04hfd.aspx).
        Simply make the isLocked member volatile.
        Also: The keyword is perhaps used a little incorrectly at the moment with isRunning, finishFlag and isSuspended.
        (Note: this will need to be profiled for possible performance gain \ loss)
*/

/*                                 INCLUDES                                 */
#include <windows.h>
//#include "types.h"

/*                                  CLASSES                                  */
class Mutex
{
protected:
  bool isLocked;
  HANDLE hMutex;
public:
  Mutex();
  ~Mutex();

  /*  Locks the mutex. This also waits until the lock has taken place, so if another 
      thread has already locked the mutex, this waits until that thread has unlocked the 
      mutex
  */
  void lock();
  
  /*  unlocks the mutex */
  void unlock();
  
  /*  Checks whether the mutex is unlocked, but does not wait if it is not*/
  bool peekUnlock() const;
  
  /*  Waits until the mutex is unlocked */
  void waitUnlock(); 
};


class Thread
{
  friend void runThread(Thread& thread);

protected:
  Mutex mutex;
  HANDLE hThread;
  uint32 ID; //win 95, 98 & ME requires this for the CreateThread function
  volatile bool isRunning;
  volatile bool finishFlag;
  volatile bool isSuspended;

  /* Overide this.  to implement a standard initialization routine
     Don't call manually - it is called internally by the Thread class */
  virtual void init();
  
  /* main is called repeatedly until finish is called. To make main run only once,
     finish should be called at some point in the the main loop
     Don't call manually - it is called internally by the Thread class.*/
  virtual void main() = 0;
  
  /* Overide this to implement a standard de-initialization routine
     Don't call manually - it is called internally by the Thread class. */
  virtual void end();
  
public:
  Thread();
  ~Thread();
  void start();

  /* Sends a finish command to the thread. This does not immediately terminate the
     thread. The thread terminates after the currently executing main() function
     exits, and then calls end(), after which it ends.  
     Call waitTerminate afterwards to wait until the thread has finished executing. */
  void finish();

  /* Terminates the thread's execution immediately, and executes end()
     Currently not supported until the effects of using this (if possible) can be 
     determined.
  *
  void terminate();*/

  void suspend(); 
  void resume();
  
  /* waits until the thread has started and the init function has been called. */
  void waitStart(); 
  
  /* Waits for the tread to end.         
     finish() must be called before this function is called if the thread
     does not call finish() by itself (in its overided main loop).
     May not be called by the thread itself (i.e. in main, init or end). */
  void waitTerminate(); 

  inline bool getIsRunning()   { lock(); bool tmp = isRunning; unlock(); return tmp; }
  inline bool getIsSuspended() { lock(); bool tmp = isSuspended; unlock(); return tmp; }
  inline bool getFinishFlag()  { lock(); bool tmp = finishFlag; unlock(); return tmp; }

  // locking functions
  inline void lock() { mutex.lock(); }
  inline void unlock() { mutex.unlock(); }
  inline bool peekUnlock() const { return mutex.peekUnlock(); }
  inline void waitUnlock() { mutex.waitUnlock(); }
};

#endif
