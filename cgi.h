#ifndef __CGI_H_INC
#define __CGI_H_INC

static const char *cgiext[] = {
    "cgi", "py", "exe"
}

void run_cgi(const char *req) {
    if (!ServerConfig.cgi)
        return;
    /*...*/
}

#endif
