#ifndef skei_thread_included
#define skei_thread_included
//----------------------------------------------------------------------

////#include "lib/h_Defines.h"
//#include "src/h_Defines.h"
//
////----------------------------------------------------------------------
//
//class h_ThreadBase
//{
//  protected:
//    bool  mThreadRunning;
//    int   mThreadSleep;
//  public:
//    h_ThreadBase()                      { mThreadRunning=false; mThreadSleep=-1; }
//    virtual ~h_ThreadBase()             {}
//    virtual void startThread(int ms=-1) {} // -1 = no timer
//    virtual void stopThread(void)       {}
//    //
//    // override this..
//    // called at thread creation, or every timer tick if ms > 0
//    virtual void doThreadFunc(void)     {}
//    //
//};
//
////----------------------------------------------------------------------
//#ifdef H_LINUX
////----------------------------------------------------------------------
//
//#include <pthread.h>
//#include <unistd.h> // sleep
//
//class h_Thread : public h_ThreadBase
//{
//  private:
//    pthread_t mThreadHandle;
//    //bool      mThreadRunning;
//    //int       mThreadSleep;
//
//  private:
//
//    static void* threadProc(void* data)
//      {
//        h_Thread* thr = (h_Thread*)data;
//        if (thr)
//        {
//          if (thr->mThreadSleep>=0)
//          {
//            while (thr->mThreadRunning)
//            {
//              thr->doThreadFunc();
//              usleep(thr->mThreadSleep*1000); //ms*1000;
//            }
//          } else thr->doThreadFunc();
//        }
//        return NULL;
//      }
//
//  public:
//
//    h_Thread()
//    : h_ThreadBase()
//      {
//        mThreadHandle = 0;
//        //mThreadRunning = false;
//        //mThreadSleep = -1;
//      }
//
//    virtual ~h_Thread()
//      {
//        if (mThreadRunning) stopThread();
//      }
//
//    //virtual void doThreadFunc(void) {}
//
//    virtual void startThread(int ms=-1)
//      {
//        mThreadSleep = ms;
//        mThreadRunning = true;
//        pthread_create(&mThreadHandle,NULL,&threadProc,this);
//      }
//
//    virtual void stopThread(void)
//      {
//        mThreadRunning = false;
//        void* ret;
//        pthread_join(mThreadHandle,&ret);
//      }
//
//};
//
//#endif
//
////----------------------------------------------------------------------
//#ifdef H_WIN32
////----------------------------------------------------------------------
//
//#include <windows.h>
////#include <unistd.h> // sleep
//
//class h_Thread : public h_ThreadBase
//{
//  private:
//    HANDLE  mThreadHandle;
//    DWORD   mThreadID;
//      //bool  mThreadRunning;
//      //int   mThreadSleep;
//
//  private:
//
//    static DWORD WINAPI threadProc(LPVOID data)
//      {
//        h_Thread* thr = (h_Thread*)data;
//        if (thr)
//        {
//          if (thr->mThreadSleep>=0)
//          {
//            while (thr->mThreadRunning)
//            {
//              thr->doThreadFunc();
//              Sleep(thr->mThreadSleep);
//            }
//          } else thr->doThreadFunc();
//        }
//        return H_NULL;
//      }
//
//  public:
//
//    h_Thread() //: h_ThreadBase()
//      {
//        mThreadHandle = 0;
//        mThreadID = 0;
//        //mThreadRunning = false;
//        //mThreadSleep = -1;
//      }
//
//    virtual ~h_Thread()
//      {
//        if (mThreadRunning) stopThread();
//      }
//
//    virtual void startThread(int ms=-1)
//      {
//        mThreadSleep   = ms;
//        mThreadRunning = true;
//        mThreadHandle = CreateThread(H_NULL,0,&threadProc,(LPVOID)this,H_NULL,&mThreadID);
//      }
//
//    virtual void stopThread(void)
//      {
//        //void* ret;
//        mThreadRunning = false;
//        //DWORD waiter = WaitForSingleObject(mThreadHandle,INFINITE);
//        CloseHandle(mThreadHandle);
//      }
//
//};
//
//#endif

//----------------------------------------------------------------------
#endif

