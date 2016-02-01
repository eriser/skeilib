#ifndef skei_lock_included
#define skei_lock_included
//----------------------------------------------------------------------

#include "skei_defines.h"

//class h_MutexBase
//{
//  public:
//    h_MutexBase()               {}
//    virtual ~h_MutexBase()      {}
//    virtual void lock(void)     {}
//    virtual bool tryLock(void)  {return false;}
//    virtual void unlock(void)   {}
//};

//----------------------------------------------------------------------

#ifdef SKEI_WIN32

  #include <windows.h>

  class SLock {
    private:
      CRITICAL_SECTION  MCriticalSection;

    public:

      SLock() {
        InitializeCriticalSection(&MCriticalSection);
      }

      ~SLock() {
        DeleteCriticalSection(&MCriticalSection);
      }

      void lock(void) {
        EnterCriticalSection(&MCriticalSection);
      }

      bool tryLock(void) {
        return TryEnterCriticalSection( &MCriticalSection );
      }

      void unlock(void) {
        LeaveCriticalSection(&MCriticalSection);
      }

  };

#endif // SKEI_LINUX

//----------------------------------------------------------------------

#ifdef SKEI_LINUX

  // -lpthread
  #include <pthread.h>

  class SLock {
    private:
      pthread_mutex_t MMutex; // = PTHREAD_MUTEX_INITIALIZER;

    public:

      SLock() {
        pthread_mutex_init(&MMutex,SKEI_NULL);
      }

      ~SLock() {
        pthread_mutex_destroy(&MMutex);
      }

      void lock(void) {
        pthread_mutex_lock(&MMutex);
      }

      bool tryLock(void) {
        return (pthread_mutex_trylock(&MMutex)==0);
      }

      void unlock(void) {
        pthread_mutex_unlock(&MMutex);
      }

  };

#endif

//----------------------------------------------------------------------
#endif

