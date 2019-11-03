#ifndef __SERVERSYS_H_INC
#define __SERVERSYS_H_INC
#include <stdarg.h>
#include "wide.h"
#include "deepsys.h"
#include "log.h"

static int readBinFile(const char *fName, byte *buffer, int size) { /* bin */
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

void MakeHeader(char *resph, const char *ver, const char *status,
        const char *contenttype, int contentlen/*, const char *accranges*/
    ) {
    sprintf(resph,
        "HTTP/%s %s\r\nVersion: HTTP/%s\r\nContent-Type: %s\r\n"
        "Content-Length: %d\r\nAccept-Ranges: bytes\r\n\r\n",
        ver, status, ver, contenttype, contentlen
        );
}

static bool firstOfApril() {
    time_t t = time(NULL);
    struct tm *tmCl = localtime(&t);
    return (tmCl->tm_mday == 1) && (tmCl->tm_mon + 1 == 4);
}

char *getName() {
    /*const int WSVer = MAKEWORD(2, 2);
    WSADATA wsaData;*/
    struct hostent *h;
    static char buf[128];
    int i;
    for (i = 0; i < 128; i++) buf[i] = 0;
    /*if (!WSAStartup(WSVer, &wsaData)) {
        if (!gethostname(buf, 128))
            h = gethostbyname(buf);
        WSACleanup();
    }*/
    if (!gethostname(buf, 128))
        h = gethostbyname(buf);
    return buf;
}

char *__err_page(const char *text, char *resp) {
    char *response_body = malloc(1024);
    if (!response_body) {
        LogError("Null pointer", errFileG);
        server_kill();
        return "";
    }
    sprintf(response_body,
        "<html>"
           "<head>"
               "<title>%s</title>"
           "</head>"
           "<body>"
               "<center>"
                  "<h1>"
                      "%s"
                  "</h1>"
               "</center>"
               "<hr>"
               "<center>%s</center>"
           "</body>"
        "</html>",
        text, text, SERVER_NAME);

    MakeHeader(resp, HTTP_VERSION, text, "text/html", strlen(response_body));
    strcat(resp, response_body);
    /*sprintf(resp, "HTTP/1.0 %s\r\nVersion: HTTP/1.0\r\n"
        "Content-Type: text/html; charset=ansi\r\n"
        "Content-Length: %d\r\n\r\n%s",
        text,
        strlen(response_body),
        response_body);*/

    free(response_body);
    return resp;
}

int parseMetReq(char *method, char *param, char *all_info) {
    char *inf = malloc(MAX_USER_INFO_LEN);
    strcpy(inf, all_info);
    sscanf(all_info, "%s", method);
    char *ptr = strtok(inf, " ");
    ptr = strtok(NULL, " ");
    strcpy(param, inf + strlen(method) + 1);
    free(inf);
}

#define ErrorPage(funcName,text) \
    inline char *(funcName)(char *resp) { \
        return __err_page(text, resp); \
    }

#endif
