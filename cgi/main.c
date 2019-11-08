#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include "cgi.h"
/**
 * output - outp.x
 * input  - inp.x
 */

int main(int argc, char *argv[]) {
    if (argc != 2)
        return 1;
    char *uri = argv[1];
    char *script = malloc(1024);
    int i;
    for (i = 0; i < strlen(uri); i++) {
        if (uri[i] == '?') break;
        script[i] = uri[i];
    }
    script[i] = 0;
    FILE *f = fopen("inp.x", "w");
    fclose(f);
    redirect_streams();
    run(script);
    end_work();
    free(script);
    return 0;
}
