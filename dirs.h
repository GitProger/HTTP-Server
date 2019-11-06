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

/* TODO: base the following code to 'struct file_list' for better cross-platform */
char *list_page_m() { /* Sorry ^_^ */
    char *cmd = malloc(MAX_FILE_NAME_LEN);
    char *page;
    char *buffer = calloc(MAX_FILE_NAME_LEN, 1);
    const char *nm = "_fd_list.txt";
    sprintf(cmd, "echo .>%s&echo ..>>%s&dir /b \"./%s\" >>%s", nm, nm, uri, nm);
    system(cmd);
    page = calloc(1024 + file_size(nm) << 1, 1);
    FILE *list = fopen(nm, "r");
    if (!list) {
        LogError("Null file decryptor", errFileG);
        intServErr(page);
        return page;
    }
    sprintf(page,
        "<html>"
            "<head><title>Listing for /%s</title></head>"
        "<body>"
        "<h1>Listing for /%s:</h1>"
        "<hr><ul>\n", uri, uri
    );
    while (!feof(list)) {
        ZeroMemory(buffer, strlen(buffer));

        fgets(buffer, MAX_FILE_NAME_LEN, list);
        buffer[strlen(buffer) - 1] = 0; /* replace '\n' */
        if (strlen(buffer)) {
            strcat(page, "<li><a href=\"");
            strcat(page, buffer);
            strcat(page, "\">");
            strcat(page, buffer);
            if (!file_exist(buffer)) /*dir*/
                strcat(page, "/");
            strcat(page, "</a></li>\n");
        }
    }
    strcat(page, "</ul><hr></body></html>\0\0");
    
    fclose(list);
    remove(nm);
    free(cmd); free(buffer);
    return page;
}

#endif
