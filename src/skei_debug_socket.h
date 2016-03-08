#ifndef skei_debug_socket_included
#define skei_debug_socket_included
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG_SOCKET

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>  // snprintf, dprintf

//----------------------------------------------------------------------

class SDebug_Socket {

  private:

    int skei_socket_out;

  public:

    SDebug_Socket() {
      skei_socket_out = socket(PF_UNIX,SOCK_STREAM,0);
      sockaddr_un address;
      memset(&address, 0, sizeof(sockaddr_un));
      address.sun_family = AF_UNIX;
      snprintf(address.sun_path,108,"/tmp/skei.sock");
      connect(skei_socket_out,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
    }

    //----------

    ~SDebug_Socket() {
      close(skei_socket_out);
    }

    //----------

    void print(const char* ABuffer) {
      dprintf(skei_socket_out,"%s",ABuffer);
    }

};

//----------------------------------------------------------------------

static SDebug_Socket skei_debug_socket;

//----------------------------------------------------------------------

#define SDebug_Socket_Print  skei_debug_socket.print

#endif // SKEI_DEBUG_SOCKET

//----------------------------------------------------------------------
#endif
