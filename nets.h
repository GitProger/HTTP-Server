#ifndef __NETS_INTEG_H_INC
#define __NETS_INTEG_H_INC

static int startup();
static int cleanup();
static int lasterr();
/* Implement this:
 *   WSADATA wsaData;
 *   int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
 *   ... WSAGetLastError()
 *   ... WSACleanup()
 */
#if defined(__MINGW32__) || defined(__WIN32__) || defined(WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    static int startup() {
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    static int cleanup() 
        { return WSACleanup(); }
    static int lasterr()
        { return WSAGetLastError(); }
#else
    #if defined(__linux__) || defined(__linux)
        typedef unsigned char BYTE;
        #define MAKEWORD(a,b) \
            (((long)a & 0xFF) << 8) | (long)(b & 0xFF);
        #define SOCKET int
        #define INVALID_SOCKET -1
        #define SOCKADDR struct sockaddr
        #define SOCKET_ERROR -1
        #define closesocket close
        #include <errno.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <strings.h>
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <unistd.h>
        static int cleanup() {
            return 0;
        }
        static int lasterr() {
            return errno;
        }
        static int cleanup() {
            return 0;
        }
    #else
        #error "Sorry, no version for your OS."
    #endif
#endif

#endif /* nets.h */
