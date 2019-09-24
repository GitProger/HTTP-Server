#ifndef __SERVERSYS_H_INC
#define __SERVERSYS_H_INC
#include <stdarg.h>
#include "deepsys.h"
#include "log.h"

#define SERVER_NAME "Karel"
#define SERVION_VERSION "0.0b"
#define HTTP_VERSION "1.0"
#define FULL_SERV_NAME SERVER_NAME"/"SERVION_VERSION

#define MAX_USER_INFO_LEN 0x800

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
    /*
    åñëè "r" à íå "rb" òî
    êîãäà ôîðìèðóåòñÿ îòâåò ñåðâåðà, òî '\r' íå ñ÷èòàþåòñÿ êàê ñèìâîë, íî
    îíè âõîäÿò â ðàçìåð ôàéëà, èòîãî, îòïðâëÿåì x áàéò, è èç íèõ n - '\r'
    òî åñòü ïîñëå òåëà ôàéëà áóäåò åùå n ñëó÷àéíûõ áàéòîâ èç ïàìÿòè 
    */
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
               "<hr> <center>%s</center>"
           "</body>"
        "</html>",
        text, text, SERVER_NAME);
    sprintf(resp, "HTTP/1.0 %s\r\nVersion: HTTP/1.0\r\n"
        "Content-Type: text/html; charset=ansi\r\n"
        "Content-Length: %d\r\n\r\n%s",
        text,
        strlen(response_body),
        response_body);
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

/*    char *inf = malloc(MAX_USER_INFO_LEN); 
    strcpy(inf, all_info);
    char *buffer = strtok(inf, "\r\n");
    if (!buffer) return 400; /* bad request * /
    int i;
    char *bufMet = malloc(64);
    sscanf(buffer, "%s", bufMet);
    buffer = bufMet;
    if (!stricmp(buffer, "GET")
        || !stricmp(buffer, "HEAD")
        || !stricmp(buffer, "POST")
    ) {
        free(inf); free(bufMet);
        return 400;
    } /* bad request * /
    strcpy(method, buffer);
    buffer = strtok(NULL, "\r\n");
    if (!buffer) {
        free(inf); free(bufMet);
        return 400; /* bad request * /
    }
    strcpy(param, buffer);
    free(inf); free(bufMet);
    return 200; /*ok*/
}

void GET(const char *);
void HEAD(const char *);
void POST(const char *);

void formResponse(char *param, char *response)
{
}

void GET(const char *param)
{
}

void HEAD(const char *param)
{
}


void POST(const char *param)
   {}
   
#define ErrorPage(funcName,text) \
    inline char *(funcName)(char *resp) { return __err_page(text, resp); }

#endif
