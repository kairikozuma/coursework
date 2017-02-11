// Simple threads (wrapper around pthreads)
// George F. Riley, Georgia Tech, Spring 2008

#include <iostream>
#include <pthread.h>
#include "gthread.h"

using namespace std;

// Private to the wrappers
static bool initialized = false;
static pthread_mutex_t joinMutex;
static pthread_mutex_t exitMutex;
static pthread_cond_t  exitCond;
       int             joinCount;
       pthread_attr_t  attr;
       pthread_mutex_t outMutex;

typedef void* (*PThread)(void*);

void InitializeGThreads()
{
  if (!initialized)
    {
      joinCount = 0;
      pthread_mutex_init(&exitMutex,0);
      pthread_mutex_init(&joinMutex,0);
      pthread_mutex_init(&outMutex,0);
      pthread_cond_init(&exitCond, 0);
      // Main holds the exit mutex until waiting for exitCond condition
      pthread_mutex_lock(&exitMutex);
      // The main also locks the join mutex, to prevent premature
      // thread exits.
      pthread_mutex_lock(&joinMutex);
      initialized = true;
      pthread_attr_init(&attr);
      //size_t ss;
      //pthread_attr_getstacksize(&attr, &ss);
      //std::cout << "Default stack size is " << ss << std::endl;
      //pthread_attr_setstacksize(&attr, 1000000);

    }
}


void StartThread()
{ // This is called by the CreateThread templates, to make sure we know
  // how many completions are expected, even if some threads have not
  // yet started.
  //  InitializeGThreads();
  //pthread_mutex_lock(&joinMutex);
  pthread_mutex_lock(&outMutex);
  pthread_mutex_unlock(&outMutex);
  // We don't need to lock the joinMutex here, as the main retains
  // the lock 
  joinCount++;
  //cout << "Start: Join count is " << joinCount << endl;
  //pthread_mutex_unlock(&joinMutex);
}

void EndThread()
{
  pthread_mutex_lock(&joinMutex);
  joinCount--;
  if (joinCount == 0)
    { // Last to exit, notify main
      pthread_mutex_unlock(&joinMutex);
      pthread_mutex_lock(&exitMutex);
      pthread_cond_signal(&exitCond);
      pthread_mutex_unlock(&exitMutex);
    }
  else
    {
      pthread_mutex_unlock(&joinMutex);
    }
  pthread_exit(0);
}

void WaitAllThreads()
{
  // We can now unlock the joinMutex. We waited until now to be sure
  // that joinCount has in fact counted up to the actual thread count
  // before we wait for the exitCondition.
  pthread_mutex_unlock(&joinMutex);
  pthread_cond_wait(&exitCond, &exitMutex);
  pthread_mutex_lock(&joinMutex);
}

void LockMutex(gthread_mutex_t& m)
{
  if (!m)
    { // First access, create and initilize
      m = new pthread_mutex_t;
      pthread_mutex_init(m, NULL);
    }
  pthread_mutex_lock(m);
}

void UnlockMutex(gthread_mutex_t& m)
{
  pthread_mutex_unlock(m);
}
