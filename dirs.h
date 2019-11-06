#ifndef __DIRS_H_INC
#define __DIRS_H_INC

typedef struct file_list {
    char **names;
    int n;
} file_list;

static file_list getList(const char *dir) {
    file_list res = {.names = NULL, .n = 0};
    
    return res;
}

static void free_list(file_list x) {
    int i = 0;
    for (i = 0; i < x.n; i++)
        free(x.names[i]);
    free(x.names);
}

char *list_page_m() {

}

#endif
