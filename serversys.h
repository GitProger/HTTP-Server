#ifndef __SERVERSYS_H_INC
#define __SERVERSYS_H_INC
#include <stdarg.h>
char *timeStr(void);
#define SERVER_NAME "Karel"
#define SERVION_VERSION "0.0b"
FILE *logFileG;
#define MAX_USER_INFO_LEN 0x800

#define LogEvent(text) \
        { \
        char *__M_tm_loc_var = timeStr(); \
        fprintf(logFileG, "[%s] %s\n", __M_tm_loc_var, (text));\
        free(__M_tm_loc_var); \
        }

#define LogError(text) \
        { \
        char *__M_tm_loc_var = timeStr(); \
        fprintf(logFileG, "[%s] %s | line: %d | function: %s | file: %s\n", __M_tm_loc_var, (text), __LINE__, \
             __PRETTY_FUNCTION__, __FILE__); \
        free(__M_tm_loc_var); \
        }

char *gets_s(char *s, int n) {
    fflush(stdin);
    int i, k = getchar();
    if (k == EOF)
         return NULL;
    for (i = 0; (k != EOF && k != '\n') && n--; i++) {
        s[i] = k;
        k = getchar();
        if (k == EOF && !feof(stdin))
            return NULL;
    }
    s[i] = '\0';
    return s;    
}

void free_sev_blocks(int am, ...) {
    va_list l;
    va_start(l, am);
    while (am--) 
        free(va_arg(l, void*));
    va_end(l);
}

static int file_size(const char *name) {
    struct stat f;
    stat(name, &f);
    return f.st_size;
}

static const char *isTwoSgn(int x) {
    return x < 10 ? "0" : "";
}

char *makeIP(unsigned int ip_int) {
    char *ip = (char*)malloc(16);
    sprintf(ip, "%u.%u.%u.%u", 
        ((byte*)&ip_int)[0],
        ((byte*)&ip_int)[1],
        ((byte*)&ip_int)[2],
        ((byte*)&ip_int)[3]
    );
    return ip;
}

static bool firstOfApril() {
    time_t t = time(NULL);
    struct tm *tmCl = localtime(&t);
    return (tmCl->tm_mday == 1) && (tmCl->tm_mon + 1 == 4);
}

char *timeStr() {
    char *str = (char*)malloc(64);
    time_t t = time(NULL);
    struct tm *tmCl = localtime(&t);
    sprintf(str, "%s%d.%s%d.%d %s%d:%s%d:%s%d", 
        isTwoSgn(tmCl->tm_mday), tmCl->tm_mday,
        isTwoSgn(tmCl->tm_mon), tmCl->tm_mon + 1, tmCl->tm_year + 1900,
        isTwoSgn(tmCl->tm_hour), tmCl->tm_hour,
        isTwoSgn(tmCl->tm_min), tmCl->tm_min,
        isTwoSgn(tmCl->tm_sec), tmCl->tm_sec);
    return str;
}

char *__err_page(const char *text, char *resp) {
    char *response_body = malloc(1024);
    if (!response_body) {
        LogError("Null pointer");
        server_kill();
        return "";
    }
    sprintf(response_body,
        "<html>"
           "<head>"
              "<title>%s</title>"
           "</head>"
           "<body>"
              "<center><h1>%s</h1></center>"
              "<hr><center>%s</center>"
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
    char *buffer = strtok(inf, "\r\n");
    if (!buffer) return 400; /* bad request */
    int i;
    char *bufMet = malloc(64);
    sscanf(buffer, "%s", bufMet);
    buffer = bufMet;
    if (stricmp(buffer, "GET")
        || stricmp(buffer, "HEAD")
        || stricmp(buffer, "POST")
    ) {
        free(inf); free(bufMet);
        return 400;
    } /* bad request */
    strcpy(method, buffer);
    buffer = strtok(NULL, "\r\n");
    if (!buffer) {
        free(inf); free(bufMet);
        return 400; /* bad request */
    }
    strcpy(param, buffer);
    free(inf); free(bufMet);
    return 200; /*ok*/
}

#define ErrorPage(funcName,text) \
    inline char *(funcName)(char *resp) { return __err_page(text, resp); }

#endif
