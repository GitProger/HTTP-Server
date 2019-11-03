#ifndef __RESPONSES_H_INC
#define __RESPONSES_H_INC
#include "makecontent.h"
#include "dirs.h"
ErrorPage(badRequest , "400 Bad Request"           );
ErrorPage(authReq    , "401 Authorization Required");
ErrorPage(forbidden  , "403 Forbidden"             );
ErrorPage(notFound   , "404 Not Found"             );
ErrorPage(Imateapot  , "418 I`m a teapot"          );
ErrorPage(intServErr , "500 Internal Server Error" );

int do_GET(int, char *, char *, char *);
int do_HEAD(int, char *, char *, char *);

int CreateResponse(const char *method, char *uri, char *response) {
    int i, resplen = 0;
    if (uri[0] == '/')
        for (i = 0; i < strlen(uri); i++) /* remove '/' in file name beginnig */
            uri[i] = uri[i + 1];

    if (!stricmp(uri, ""))
        uri = ServerConfig.index;
    char MIME[128], gen[64];
    getMIME(uri, MIME);
    int sz = -1;
    if (file_exist(uri))
        sz = file_size(uri);

    if (!stricmp(method, "HEAD")) {
        if (sz == -1) {
            notFound(response);
            for (i = 0; i < strlen(response) - 3; i++) { /* to clear main content and there will be only header */
                if (response[i] == '\r' &&
                    response[i + 1] == '\n' &&
                    response[i + 2] == '\r' &&
                    response[i + 3] == '\n'
                )
                    response[i + 4] = 0;
            }
        } else
        MakeHeader(response, HTTP_VERSION, "200 OK", MIME, sz);        
        return strlen(response);
    } else if (!stricmp(method, "GET")) {
        return do_GET(sz, uri, MIME, response);
    } else if (!stricmp(method, "POST")) {
        return do_POST(sz, uri, MIME, response);
    } else {
        badRequest(response);
        return strlen(response);
    }
}
            
int do_GET(int sz, char *param, char *MIME, char *response) {
    if (sz == -1) {
        notFound(response);
        return strlen(response);
    }
    int resplen = 0, i;
    char *buffer = malloc(sz + 1), *gen = malloc(64);
    getGeneralMIME(MIME, gen);
    if (!stricmp(gen, "text")) {/* text */
        readTextFile(param, buffer, sz);
    } else /* bin */
        readBinFile(param, buffer, sz);

    MakeHeader(response, HTTP_VERSION, "200 OK", MIME, sz);
    resplen = strlen(response);
    for (i = 0; i < sz; i++)
        response[resplen + i] = buffer[i];
    resplen += sz;
    response[resplen] = 0;
    free_sev_blcks(2, buffer, gen);
    return resplen;
}

int do_POST(int sz, char *param, char *MIME, char *response) {
    return do_GET(sz, param, MIME, response);
}

int do_HEAD(int sz, char *param, char *MIME, char *response) {
    MakeHeader(response, HTTP_VERSION, "200 OK", MIME, sz);        
    return strlen(response);
}
#endif
