#ifndef __READ_CONFIG_FILE_H_INC
#define __READ_CONFIG_FILE_H_INC
#include <stdio.h>
#include <string.h>

struct __server_configuration_struct {
    char ip[16];
    char port[6];
    char *name;
    char *index;
    char *log;
    char *root;
    int  heap_sz_mb;
    bool security;
    bool filelist;
    bool cgi;
    char *py;
};
static struct __server_configuration_struct ServerConfig;

static int init_config(struct __server_configuration_struct *s) {
    strcpy(s->ip, "127.0.0.1");
    strcpy(s->port, "80");
    s->name = malloc(256);
    if (!s->name) return 1;
    s->index = malloc(1024);
    if (!s->index) return 1;
    s->log = malloc(1024);
    if (!s->log) return 1;
    s->root = malloc(1024);
    if (!s->root) return 1;
    s->heap_sz_mb = 32;
    strcpy(s->name, "");
    strcpy(s->index, "index.html");
    strcpy(s->log, ".");
    strcpy(s->root, ".");
    s->security = false;
    s->filelist = false;
    s->cgi = false;
    s->py = malloc(MAX_FILE_NAME_LEN);
    return 0;
}


int ReadConfigFile() {
    /*** Initaliztion ***/
    FILE *fconf = fopen("config", "r");
    char *curSetting, *curOption;
    if (!fconf) {
        return 1;
    }
    curSetting = malloc(128);
    if (!curSetting) return 1;
    curOption = malloc(1024);
    if (!curOption) return 1;
    if (init_config(&ServerConfig)) return 1;
    /***             ***/
    while (!feof(fconf)) {
        fscanf(fconf, "%s", curSetting);
        if (curSetting[0] == '#') {
            fgets(curOption, 1000, fconf);
            continue;
        }
        fscanf(fconf, "%s", curOption);

        if (!stricmp(curSetting, "root"))
            strcpy(ServerConfig.root, curOption);
        else if (!stricmp(curSetting, "log"))
            strcpy(ServerConfig.log, curOption);
        else if (!stricmp(curSetting, "index"))
            strcpy(ServerConfig.index, curOption);
        else if (!stricmp(curSetting, "server_ip"))
            strcpy(ServerConfig.ip, curOption);
        else if (!stricmp(curSetting, "listen_port"))
            strcpy(ServerConfig.port, curOption);
        else if (!stricmp(curSetting, "server_name"))
            strcpy(ServerConfig.name, curOption);
        else if (!stricmp(curSetting, "heap_mb"))
            ServerConfig.heap_sz_mb = atoi(curOption);
        else if (!stricmp(curSetting, "pycgi"))
            strcpy(ServerConfig.py, curOption);
        /* TODO: security, sendfilelist, cgi */
    }
    
    fclose(fconf);
}

#endif
