#ifndef __SERVER_H_INC
#define __SERVER_H_INC

/*#define _WIN32_WINNT 0x501*/
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <sys/stat.h>
#include <signal.h>
void server_kill(void);
int server_init(void);
#include "log.h"
#include "serversys.h"
#include "responses.h"

void server_kill() {
    fclose(logFileG);
    fclose(errFileG);
}

void sigint(int sig) {
    LogEvent("Interrupted (SIGINT).", logFileG);
    server_kill();
    exit(0);
}
void sigsegv(int sig) {
    LogEvent("Null pointer (SIGSEGV).", logFileG);
    server_kill();
    exit(4);
}

int server_init() {
    signal(SIGSEGV, sigsegv);
    signal(SIGINT, sigint);
    setlocale(LC_ALL, "Russian");
    InitLogsFiles();
    return 0;
}

int showLog(const char *all, int ip) {
    char *buffer = (char*)malloc(MAX_USER_INFO_LEN);
    if (!buffer) {
        LogError("Null pointer", logFileG);
        return 1;
    }
    strncpy(buffer, all, MAX_USER_INFO_LEN);
    const char *reqt = strtok(buffer, ":\r\n");
    if (!reqt) reqt = "Unknown";
    char *host = makeIP(ip);
    char *tm = timeStr();
    fprintf(logFileG, "[%s] request from: %15s | %s\n", tm, host, reqt);
    free(buffer);
    free(host);
    free(tm);
    return 0;
}


int server() {
    char *time = timeStr();
    if (!time)
        time = " Error in timeStr()";
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    fprintf(logFileG, "[%s] Entry to the main server proc (IP=%s, Port=%s)\n", time, ServerConfig.ip, ServerConfig.port);
    if (result) {
        fprintf(errFileG, "[%s] WSAStartup(...) failed: %d\n", time, result);
        return result;
    }
    struct addrinfo *addr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    result = getaddrinfo(ServerConfig.ip, ServerConfig.port, &hints, &addr);

    if (result != 0) {
        fprintf(errFileG, "[%s] getaddrinfo(...) failed: %d\n", time, result);
        WSACleanup();
        return 1;
    }

    int listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        fprintf(errFileG, "[%s] Error at socket: %d\n", time, WSAGetLastError());
        freeaddrinfo(addr);
        WSACleanup();
        return 1;
    }

    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    if (result == SOCKET_ERROR) {
        fprintf(errFileG, "[%s] bind(...) failed with error: %d\n", time, WSAGetLastError());
        freeaddrinfo(addr);
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(errFileG, "[%s] listen(...) failed with error: %d\n",
            time, WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }


    int client_socket = INVALID_SOCKET;
    char *user_info = (char*)malloc(MAX_USER_INFO_LEN); /* request of client */
    char *response = (char*)malloc(1 << 25); /* 1MB */
    response[0] = 0;
    if (!response) {
        LogError("No free memory.", errFileG);
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }
    
    fprintf(logFileG, "[%s] Run (IP=%s, Port=%s)\n", time, ServerConfig.ip, ServerConfig.port);
    /* MAIN */
    for (;;) {
        client_socket = accept(listen_socket, NULL, NULL);
        
        if (client_socket == INVALID_SOCKET) {
            fprintf(stderr, "accept(...) failed: %d\n", WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            return 1;
        }

        result = recv(client_socket, user_info, MAX_USER_INFO_LEN, 0);

        if (result == SOCKET_ERROR) {
            fprintf(logFileG, "[%s] recv(...) failed: %d\n", time, result);
            closesocket(client_socket);
        } else if (result == 0) {
            LogError("Connection closed.", errFileG);
        } else if (result > 0) {
            user_info[result] = '\0';
            
            struct sockaddr_in testdata = {};
            socklen_t socklen = sizeof(testdata);
            getpeername(client_socket, (struct sockaddr *)&testdata, &socklen);
            unsigned int Iip = testdata.sin_addr.s_addr;
            unsigned int resplen = 0, i;

            showLog(user_info, Iip);

            if (firstOfApril()) {
                LogEvent("One teapot more!", logFileG);
                Imateapot(response);
                goto SendAnswer;    
            }

            ZeroMemory(response, 1 << 25);
            
           /*HTML*/ 

           char req[128], uri[128];
           if (parseMetReq(req, uri, user_info) == 400) {
               badRequest(response);
               goto SendAnswer;
           }
           resplen = CreateResponse(req, uri, response);
            
          SendAnswer:
            result = send(client_socket, response, resplen, 0);
            if (result == SOCKET_ERROR) {
                fprintf(errFileG, "[%s] send(...) failed: %d\n", time, WSAGetLastError());
            }
            closesocket(client_socket);
        }
    }
    free(response);
    free(user_info);
    free(time);
    closesocket(listen_socket);
    freeaddrinfo(addr);
    WSACleanup();
    return 0;
}

#endif
