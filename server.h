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
#include "serversys.h"


ErrorPage(badRequest , "400 Bad Request"           );
ErrorPage(authReq    , "401 Authorization Required");
ErrorPage(forbidden  , "403 Forbidden"             );
ErrorPage(notFound   , "404 Not Found"             );
ErrorPage(Imateapot  , "418 I`m a teapot"          );
ErrorPage(intServErr , "500 Internal Server Error" );


void server_kill() {
    fclose(logFileG);
}

void sigint(int sig) {
    LogEvent("Interrupted (SIGINT).");
    server_kill();
    exit(0);
}
void sigsegv(int sig) {
    LogEvent("Null pointer (SIGSEGV).");
    server_kill();
    exit(4);
}

int server_init() {
    signal(SIGSEGV, sigsegv);
    signal(SIGINT, sigint);
    setlocale(LC_ALL, "Russian");
    static char logname[20] = {};
    time_t t = time(NULL);
    struct tm *tmCl = localtime(&t);
    sprintf(logname, "log_%s%d_%s%d_%d.txt",
        isTwoSgn(tmCl->tm_mday), tmCl->tm_mday,
        isTwoSgn(tmCl->tm_mon), tmCl->tm_mon + 1, tmCl->tm_year + 1900);
    logFileG = fopen(logname, "a");
    if (!logFileG) {
        logFileG = fopen(logname, "w");
    }
    LogEvent("Initialized.");    
    return 0;
}

static int readBuffer(const char *, byte *, int);
static int readTextFile(const char *, char *, int);

static const char *parseReq(const char *all) {
    char *buffer = (char*)malloc(MAX_USER_INFO_LEN);
    strcpy(buffer, all);
    char *method = strtok(buffer, " :\r\n");
    char *reqt = strtok(NULL, " :\r\n");
    if (!reqt || !method) {
        free(buffer);
        return NULL;
    }
    char *result = (char*)malloc(strlen(reqt) + 1);
    strcpy(result, reqt);
    free(buffer);
    return result;
}

int showLog(const char *all, int ip) {
    char *buffer = (char*)malloc(MAX_USER_INFO_LEN);
    if (!buffer) {
        LogError("Null pointer");
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


int server(const char *ipAddr, const char *port) {
    char *time = timeStr();
    if (!time)
        time = " Error in timeStr()";
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    fprintf(logFileG, "[%s] Entry to the main server proc (IP=%s, Port=%s)\n", time, ipAddr, port);
    if (result) {
        fprintf(logFileG, "[%s] WSAStartup(...) failed: %d\n", time, result);
        return result;
    }
    struct addrinfo *addr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    result = getaddrinfo(ipAddr, port, &hints, &addr);

    if (result != 0) {
        fprintf(logFileG, "[%s] getaddrinfo(...) failed: %d\n", time, result);
        WSACleanup();
        return 1;
    }

    int listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        fprintf(logFileG, "[%s] Error at socket: %d\n", time, WSAGetLastError());
        freeaddrinfo(addr);
        WSACleanup();
        return 1;
    }

    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    if (result == SOCKET_ERROR) {
        fprintf(logFileG, "[%s] bind(...) failed with error: %d\n", time, WSAGetLastError());
        freeaddrinfo(addr);
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(logFileG, "[%s] listen(...) failed with error: %d\n",
            time, WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }


    int client_socket = INVALID_SOCKET;
    char *user_info = (char*)malloc(MAX_USER_INFO_LEN); /* request of client */
    char *response = (char*)malloc(1 << 16);
    char *response_body = (char*)malloc(1 << 16);
    response[0] = response_body[0] = 0;
    if (!response || !response_body) {
        LogError("No free memory.");
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }
    
    fprintf(logFileG, "[%s] Run (IP=%s, Port=%s)\n", time, ipAddr, port);
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
            LogError("Connection closed.");
        } else if (result > 0) {
            user_info[result] = '\0';
            
            struct sockaddr_in testdata = {};
            socklen_t socklen = sizeof(testdata);
            getpeername(client_socket, (struct sockaddr *)&testdata, &socklen);
            unsigned int Iip = testdata.sin_addr.s_addr;
            unsigned int resplen = 0, i;

            showLog(user_info, Iip);

            if (firstOfApril()) {
                LogEvent("One lammer more!");
                Imateapot(response);
                goto SendAnswer;    
            }

            const char *reqst = parseReq(user_info);
            if (reqst == NULL) {
                goto SendAnswer;
            }
            
            ZeroMemory(response, 1 << 16);
            ZeroMemory(response_body, 1 << 16);
            
           /*HTML*/ 
            if (!stricmp(reqst, "/")) {
                int sz = file_size("index.html") + 1;
                char *htmlText = malloc(sz + 1);
                if (htmlText == NULL) {
                    LogError("Null pointer (htmlText).");
                    intServErr(response);
                    goto SendAnswer;
                };
                ZeroMemory(htmlText, sz + 1);
                readTextFile("index.html", htmlText, sz);
                sprintf(response_body, "%s", htmlText);
                free(htmlText);
                sprintf(response, "%s%s%s%s%d%s%s", "HTTP/1.0 200 OK\r\n", /*HTTP/1.1*/
                    "Version: HTTP/1.0\r\n",
                    "Content-Type: text/html; charset=utf-4\r\n",/* charset=utf-8 */
                    "Content-Length: ", strlen(response_body),
                    "\r\n\r\n",
                    response_body);
                resplen = strlen(response);
            } else if (!stricmp(reqst, "/orig.gif")) {
            
                /*int sz = file_size("orig.gif");
                byte *fBuffer = (byte*)malloc(sz + 1);
                if (fBuffer == NULL) {
                    LogError("Null pointer (fBuffer).");
                    intServErr(response);
                } else {
                    sprintf(response, "%s%s%s%s%d%s%s", "HTTP/1.0 200 OK\r\n",
                        "Version: HTTP/1.0\r\n",
                        "Content-Type: image/gif\r\n",
                        "Content-Length: ", sz, "\r\nAccept-Ranges: bytes",
                        "\r\n\r\n");
                     resplen = strlen(response);
                     for (i = 0; i < sz; i++)
                         response[resplen + i] = fBuffer[i];
                     resplen += sz;
                     response[resplen - 1] = 0;
                     free(fBuffer);
                }*/
                forbidden(response);
                resplen = strlen(response);

            } else if (!stricmp(reqst, "/photo.png")) {
            
                int sz = file_size("photo.png");
                byte *fBuffer = (byte*)malloc(sz + 1);
                if (fBuffer == NULL) {
                    LogError("Null pointer (fBuffer).");
                    intServErr(response);
                } else {
                    readBuffer("photo.png", fBuffer, sz);
                    sprintf(response, "%s%s%s%s%d%s%s", "HTTP/1.0 200 OK\r\n",
                        "Version: HTTP/1.0\r\n",
                        "Content-Type: image/png\r\n",
                        "Content-Length: ", sz, "\r\nAccept-Ranges: bytes",
                        "\r\n\r\n");
                    resplen = strlen(response);
                    for (i = 0; i < sz; i++)
                        response[resplen + i] = fBuffer[i];
                    resplen += sz;
                    response[resplen - 1] = 0;
                    free(fBuffer);
                }
                
            } else {
                notFound(response);
                resplen = strlen(response);
            }
          SendAnswer:
            free((void*)reqst);
            result = send(client_socket, response, resplen, 0);
            if (result == SOCKET_ERROR) {
                fprintf(logFileG, "[%s] send(...) failed: %d\n", time, WSAGetLastError());
            }
            closesocket(client_socket);
        }
    }
    free(response);
    free(response_body);
    free(user_info);
    free(time);
    closesocket(listen_socket);
    freeaddrinfo(addr);
    WSACleanup();
    return 0;
}

static int readBuffer(const char *fName, byte *buffer, int size) { /* bin */
    FILE *f = fopen(fName, "rb");
    int i;
    if (!f) 
        return 404;
    else {
        for (i = 0; i < size; i++) {
            fread(buffer + i, 1, 1, f);
        }
        fclose(f);
        return 200;
    }
}

static int readTextFile(const char *fName, char *buffer, int size) { /* text */
    FILE *f = fopen(fName, "r");
    int i;
    if (!f) 
        return 404;
    else {
        for (i = 0; i < size; i++) {
            fread(buffer + i, 1, 1, f);
        }
        fclose(f);
        return 200;
    }
}

#endif
