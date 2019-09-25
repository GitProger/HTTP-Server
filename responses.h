#ifndef __RESPONSES_H_INC
#define __RESPONSES_H_INC
#include "makecontent.h"
ErrorPage(badRequest , "400 Bad Request"           );
ErrorPage(authReq    , "401 Authorization Required");
ErrorPage(forbidden  , "403 Forbidden"             );
ErrorPage(notFound   , "404 Not Found"             );
ErrorPage(Imateapot  , "418 I`m a teapot"          );
ErrorPage(intServErr , "500 Internal Server Error" );


void MakeHeader(char *resph, const char *ver, const char *status,
        const char *contenttype, int contentlen/*, const char *accranges*/
    ) {
    sprintf(resph,
        "HTTP/%s %s\r\nVersion: HTTP/%s\r\nContent-Type: %s\r\n"
        "Content-Length: %d\r\nAccept-Ranges: bytes\r\n\r\n",
        ver, status, ver, contenttype, contentlen
        );
    /*MakeHeader(resph, "1.0", "200 OK", "image/gif", 132435, bytes)*/
}


int CreateResponse(const char *method, char *uri, char *response) {
    int i, resplen = 0;
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
    
        if (sz == -1) {
            notFound(response);
            return strlen(response);
        }
        char *buffer = malloc(sz + 1);
        getGeneralMIME(MIME, gen);
        if (!stricmp(gen, "text")) {/* text */
            readTextFile(uri, buffer, sz);
        } else /* bin */
            readBinFile(uri, buffer, sz);

        MakeHeader(response, HTTP_VERSION, "200 OK", MIME, sz);
        resplen = strlen(response);
        for (i = 0; i < sz; i++)
            response[resplen + i] = buffer[i];
        resplen += sz;
        response[resplen] = 0;

        free(buffer);
        return resplen;
                   
    } else if (!stricmp(method, "POST")) {
        MakeHeader(response, HTTP_VERSION, "200 OK", "text/plain", 0);
        return strlen(response);
    } else {
        badRequest(response);
        return strlen(response);
    }
}
            
#endif
