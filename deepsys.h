#ifndef __DEEP_SYS_H_INC
#define __DEEP_SYS_H_INC
#define MAX_FILE_NAME_LEN 1024
#define MAX_USER_INFO_LEN 0x800
#define SERVER_NAME "Server"
#define SERVION_VERSION "0.0b"
#define HTTP_VERSION "1.0"
#define FULL_SERV_NAME SERVER_NAME"/"SERVION_VERSION
/************************/
void boot_pic(int loop) {
    loop *= 4;
    while (loop--) {
        /*putchar('\b');*/
        switch (loop % 4) {
            case 0: putchar('/');
                    break;
            case 1: putchar('-');
                    break;
            case 2: putchar('\\');
                    break;
            case 3: putchar('|');
        }
        Sleep(200); /*'sleep()' - in seconds*/
        putchar('\b');
    }
}
/************************/
char *makeIP(unsigned int ip_int) { /*alloc*/
    char *ip = (char*)malloc(16);
    sprintf(ip, "%u.%u.%u.%u", 
        ((byte*)&ip_int)[0],
        ((byte*)&ip_int)[1],
        ((byte*)&ip_int)[2],
        ((byte*)&ip_int)[3]
    );
    return ip;
}

char *FileExt(const char *fn) { /*alloc*/
    char *res;
    int i = strlen(fn) - 1, j;
    while (fn[i] != '.' && i >= 0) i--;
    res = calloc(strlen(fn) - i, 1);
    for (j = i + 1; j < strlen(fn); j++)
        res[j - i - 1] = fn[j];   
    return res;
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

bool file_exist(const char *name) {
    FILE *f = fopen(name, "rb");
    if (!f) return false;
    fclose(f);
    return true;
}

static const char *isTwoSgn(int x) {
    return x < 10 ? "0" : "";
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

#endif
