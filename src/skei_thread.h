#ifndef skei_thread_included
#define skei_thread_included
//----------------------------------------------------------------------

#ifdef SKEI_LINUX
//#include <pthread.h>
//#include <unistd.h> // sleep
#endif

#ifdef SKEI_WIN32
//#include <windows.h>
#endif

//----------------------------------------------------------------------

class SThread {

  private:

    bool        MThreadRunning;
    int         MThreadSleep;
    #ifdef SKEI_LINUX
    pthread_t   MThreadHandle;
    #endif
    #ifdef SKEI_WIN32
    HANDLE  MThreadHandle;
    DWORD   MThreadID;
    #endif

  //----------------------------------------
  //
  //----------------------------------------

  private:

    #ifdef SKEI_LINUX
    static void* threadProc(void* data) {
      SThread* thr = (SThread*)data;
      if (thr) {
        if (thr->SThreadSleep>=0) {
          while (thr->SThreadRunning) {
            thr->doThreadFunc();
            usleep(thr->mThreadSleep*1000); //ms*1000;
          }
        } else thr->doThreadFunc();
      }
      return SKEI_NULL;
    }
    #endif

    //----------

    #ifdef SKEI_WIN32
    static DWORD WINAPI threadProc(LPVOID data) {
      MThread* thr = (MThread*)data;
      if (thr) {
        if (thr->MThreadSleep>=0) {
          while (thr->MThreadRunning) {
            thr->on_threadFunc();
            Sleep(thr->MThreadSleep);
          }
        } else thr->on_threadFunc();
      }
      return SKEI_NULL;
    }
    #endif

  public:

    SThread() {
      MThreadRunning = false;
      MThreadSleep = -1;
      #ifdef SKEI_LINUX
      mThreadHandle = 0;
      #endif
      #ifdef SKEI_WIN32
      MThreadHandle = 0;
      MThreadID = 0;
      #endif
    }

    //----------

    virtual ~SThread() {
      if (MThreadRunning) stopThread();
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // -1 = no timer



    void startThread(int ms=-1) {
      MThreadSleep = ms;
      MThreadRunning = true;
      #ifdef SKEI_LINUX
      pthread_create(&MThreadHandle,SKEI_NULL,&threadProc,this);
      #endif
      #ifdef SKEI_WIN32
      MThreadHandle = CreateThread(SKEI_NULL,0,&threadProc,(LPVOID)this,SKEI_NULL,&MThreadID);
      #endif
    }



    virtual void stopThread(void) {
      MThreadRunning = false;
      void* ret;
      #ifdef SKEI_LINUX
      pthread_join(MThreadHandle,&ret);
      #endif
      #ifdef SKEI_WIN32
      //void* ret;
      //DWORD waiter = WaitForSingleObject(MThreadHandle,INFINITE);
      CloseHandle(MThreadHandle);
      #endif
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // override this..
    // called at thread creation, or every timer tick if ms > 0

    virtual
    void on_threadFunc(void) {
    }

};

//----------------------------------------------------------------------
#endif

