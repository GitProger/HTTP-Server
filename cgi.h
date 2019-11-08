#ifndef __CGI_H_INC
#define __CGI_H_INC

char *run_cgi(const char *req, char *resp) {
    char *cmd = calloc(1024, 1);
    char *out = calloc(1024, 1);
    char *mime = calloc(1024, 1);
    sprintf(out, "outp.x");
    strcat(cmd, "cgi.exe \"");
    strcat(cmd, req);
    strcat(cmd, "\"");
    system(cmd);
    readTextFile(out, resp, file_size(out));
    free(cmd);
    free(out);
   /*TODO mime!!!*/
    free(mime);
    return "text/html";
}

#endif
