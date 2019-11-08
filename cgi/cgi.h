#ifndef __CGI_H_INC
#define __CGI_H_INC
/** if file is not perl or py
 *  then it should be exe
 *
 * Python3 - .py
 * Perl - .pl  --not supported
 * Exe - .exe  --not supported
 * #!path
 */

bool file_exist(const char *name) {
    FILE *f = fopen(name, "rb");
    if (!f) return false;
    fclose(f);
    return true;
}

static char *fext(const char *fn) { /*alloc*/
    char *res;
    int i = strlen(fn) - 1, j;
    while (fn[i] != '.' && i >= 0) i--;
    res = calloc(strlen(fn) - i, 1);
    for (j = i + 1; j < strlen(fn); j++)
        res[j - i - 1] = fn[j];   
    return res;
}

static void redirect_streams() {
    freopen("inp.x", "r", stdin);
    freopen("outp.x", "w", stdout);
}

void run(const char *name) {
    char *ext = fext(name);
    char *inter = malloc(1024);
    char *cmd = calloc(1024, 1);

    if (strcmp(ext, "py") && strcmp(ext, "pl")) {
        system(name);
    } else {
        FILE *sc = fopen(name, "r");
        fgets(inter, 1024, sc);
        inter[strlen(inter) - 1] = 0; /* remove '\n' at the end */
        
        int i; /* remove '#!' */
        for (i = 0; i < strlen(inter) - 2; i++)
            inter[i] = inter[i + 2];
        inter[i] = 0;

        strcat(cmd, "\" ");
        strcat(cmd, inter);
        strcat(cmd, "\" ");
        strcat(cmd, name);
        system(cmd);
        free(inter);
        free(cmd);
        fclose(sc);
    }
    free(ext);
}

static void end_work() {
    fflush(stdin);
    fflush(stdout);
    fclose(stdin);
    fclose(stdout);
}

#endif
