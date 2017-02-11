// Simple threads (wrapper around pthreads)
// George F. Riley, Georgia Tech, Spring 2008
#include <iostream>
// Define the type for the thread function
typedef void* (*PThread)(void*);

// Typedef for the mutex
typedef pthread_mutex_t* gthread_mutex_t;

// To be called by each thread on entry
void StartThread();

// To be called by each thread just before exiting
void EndThread();

// To be called by the main before exiting
void WaitAllThreads();

// Mutex functions

// Lock the mutex
void LockMutex(gthread_mutex_t&);

// Unlock the mutex
void UnlockMutex(gthread_mutex_t&);

void InitializeGThreads();

// Define the arguments for the thread wrappers
class Arg0
{
public:
  Arg0(void(*f)()) :
       m_f(f) {}
public:       
  void (*m_f)();
};

template <typename T1>
class Arg1
{
public:
  Arg1(void(*f)(T1), T1 t1) :
       m_f(f), m_t1(t1) {}
public:       
  void (*m_f)(T1);
  T1 m_t1;
};

template <typename T1, typename T2>
class Arg2
{
public:
  Arg2(void(*f)(T1, T2), T1 t1, T2 t2) :
       m_f(f), m_t1(t1), m_t2(t2){}
public:       
  void (*m_f)(T1, T2);
  T1 m_t1;
  T2 m_t2;
};

template <typename T1, typename T2, typename T3>
class Arg3
{
public:
 Arg3(void(*f)(T1, T2, T3), T1 t1, T2 t2, T3 t3) :
  m_f(f), m_t1(t1), m_t2(t2), m_t3(t3){}
public:       
  void (*m_f)(T1, T2, T3);
  T1 m_t1;
  T2 m_t2;
  T3 m_t3;
};

template <typename T1, typename T2, typename T3, typename T4>
class Arg4
{
public:
 Arg4(void(*f)(T1, T2, T3, T4), T1 t1, T2 t2, T3 t3, T4 t4) :
  m_f(f), m_t1(t1), m_t2(t2), m_t3(t3), m_t4(t4) {}
public:       
  void (*m_f)(T1, T2, T3, T4);
  T1 m_t1;
  T2 m_t2;
  T3 m_t3;
  T4 m_t4;
};

// Declaration of thread wrappers
template <typename>
  void* TW0(void*);

template <typename T1>
  void* TW1(void*);

template <typename T1, typename T2>
  void* TW2(void*);
  
template <typename T1, typename T2, typename T3>
  void* TW3(void*);
  
            
// Implement the thread wrappers
template <typename>
void* TW0(void* v0)
{
  Arg0* a0 = (Arg0*)v0;
  a0->m_f();
  delete a0;
  return 0;
}

template <typename T1>
void* TW1(void* v1)
{
  Arg1<T1>* a1 = (Arg1<T1>*)v1;
  a1->m_f(a1->m_t1);
  delete a1;
  return 0;
}

template <typename T1, typename T2>
  void* TW2(void* v2)
{
  Arg2<T1, T2>* a2 = (Arg2<T1, T2>*)v2;
  a2->m_f(a2->m_t1, a2->m_t2);
  delete a2;
  return 0;
}

template <typename T1, typename T2, typename T3>
  void* TW3(void* v3)
{
  Arg3<T1, T2, T3>* a3 = (Arg3<T1, T2, T3>*)v3;
  a3->m_f(a3->m_t1, a3->m_t2, a3->m_t3);
  delete a3;
  return 0;
}

template <typename T1, typename T2, typename T3, typename T4>
  void* TW4(void* v4)
{
  Arg4<T1, T2, T3, T4>* a4 = (Arg4<T1, T2, T3, T4>*)v4;
  a4->m_f(a4->m_t1, a4->m_t2, a4->m_t3, a4->m_t4);
  delete a4;
  return 0;
}

extern pthread_attr_t attr;

// Implement the Various CreateThread functions
template <typename>
void CreateThread(void(*tf)()) // Zero arguments version
{
  InitializeGThreads();
  pthread_t pt = 0;
  Arg0* a0 = new Arg0(tf);
  StartThread();
  pthread_create(&pt, &attr, (PThread)TW0, (void*)a0);
  pthread_detach(pt);
}

template <typename T1>
void CreateThread(void(*tf)(T1), T1 t1)
{
  InitializeGThreads();
  pthread_t pt;
  Arg1<T1>* a1 = new Arg1<T1>(tf, t1);
  //typedef void(*TW1T)(Arg1<T1>*);
  //void(*tw1)(Arg1<T1>*) = (TW1T)TW1<T1>;
  StartThread();
  pthread_create(&pt, &attr, TW1<T1>, (void*)a1);
  pthread_detach(pt);
  pt = 0;
}

template <typename T1, typename T2>
  void CreateThread(void(*tf)(T1,T2), T1 t1, T2 t2)
{
  InitializeGThreads();
  pthread_t pt;
  Arg2<T1,T2>* a2 = new Arg2<T1,T2>(tf, t1, t2);
  StartThread();
  pthread_create(&pt, &attr, (PThread)TW2<T1,T2>, (void*)a2);
  pthread_detach(pt);
  pt = 0;
}

template <typename T1, typename T2, typename T3>
  void CreateThread(void(*tf)(T1,T2,T3), T1 t1, T2 t2, T3 t3)
{
  InitializeGThreads();
  pthread_t pt;
  Arg3<T1,T2,T3>* a3 = new Arg3<T1,T2,T3>(tf, t1, t2, t3);
  StartThread();
  int k = pthread_create(&pt, &attr, (PThread)TW3<T1,T2,T3>, (void*)a3);
  pthread_detach(pt);
}

template <typename T1, typename T2, typename T3, typename T4>
  void CreateThread(void(*tf)(T1,T2,T3,T4), T1 t1, T2 t2, T3 t3, T4 t4)
{
  InitializeGThreads();
  pthread_t pt;
  Arg4<T1,T2,T3,T4>* a = new Arg4<T1,T2,T3,T4>(tf, t1, t2, t3, t4);
  StartThread();
  pthread_create(&pt, &attr, (PThread)TW4<T1,T2,T3,T4>, (void*)a);
  pthread_detach(pt);
  pt = 0;
}

