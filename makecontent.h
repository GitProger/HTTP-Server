#ifndef __MAKECONTENT_H_INC
#define __MAKECONTENT_H_INC
#include "serversys.h"

int getMIME(const char *uri, char *MIME) {
    char *ext = FileExt(uri);
    if (!stricmp(ext, "gif")) strcpy(MIME, "image/gif"); else
    if (!stricmp(ext, "jpeg") || !stricmp(ext, "jpg")) strcpy(MIME, "image/jpeg"); else
    if (!stricmp(ext, "png")) strcpy(MIME, "image/png"); else

    if (!stricmp(ext, "js")) strcpy(MIME, "application/javascript"); else
    if (!stricmp(ext, "json")) strcpy(MIME, "application/json"); else
    if (!stricmp(ext, "zip")) strcpy(MIME, "application/zip"); else
    if (!stricmp(ext, "pdf")) strcpy(MIME, "application/pdf"); else
    
    if (!stricmp(ext, "mp4")) strcpy(MIME, "video/mp4"); else

    if (!stricmp(ext, "mp3") || !stricmp(ext, "mpeg")) strcpy(MIME, "audio/mpeg"); else

    if (!stricmp(ext, "html") || !stricmp(ext, "htm")) strcpy(MIME, "text/html"); else
    if (!stricmp(ext, "css")) strcpy(MIME, "text/css"); else
    /*if (!stricmp(ext, "js")) strcpy(MIME, "text/javascript"); else*/
    if (!stricmp(ext, "csv")) strcpy(MIME, "text/csv"); else
    if (!stricmp(ext, "php")) strcpy(MIME, "text/php"); else
    if (!stricmp(ext, "xml")) strcpy(MIME, "text/xml"); else
    strcpy(MIME, "text/plain");
    free(ext);
}

void getGeneralMIME(char *MIME, char *gen) {
    int i;
    for (i = 0; i < strlen(MIME); i++) {
        if (MIME[i] == '/')
            break;
        gen[i] = MIME[i];        
    }
    gen[i] = '\0';
}

#endif
