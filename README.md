#HTTP Server
    This server is based on WinApi (WinSocket), it supports HTTP 1.0
## Plans
   * CGI
   * Python
   * More return HTTP codes
## Short manual
    All parameters must have length less than 1024 symbols
    If a parameter is file or dir it must exists.

    |--------------------|--------------------------------------|------------------|    
    |        ITEM        |              WHAT IS IT              |    SUPPORTING    |
    |--------------------|--------------------------------------|------------------|
    | root               | root directory of site               |        NO        |
    | index              | start page of site                   |        YES       |
    | server_ip          | IP-address of server                 |        YES       |
    | listen_port        | working port of site                 |        YES       |
    | log                | directory with logs                  |        YES       |
    | server_name        | name of site                         |    DECORATION    |
    | heap_mb            | size of server`s heap (in megabytes) |        YES       |
    | sendfilelist       | if 'on' server sends list of files   |        NO        |
    |                    |  if a request file hasn`t extension  |                  |
    | security           | if 'on' all requests that includes   |        NO        |
    |                    | '..' ignores                         |                  |
    | cgi                | cgi on/off (.cgi, .py .exe)          |        NO        |
    | pycgi              | path to python interpretor           |        NO        |
    |--------------------|--------------------------------------|------------------|

### C code of the configuration:
    struct __server_configuration_struct {
        char ip[16];       /* server_ip */
        char port[6];      /* listen_port */
        char *name;        /* server_name */
        char *index;       /* index */
        char *log;         /* log */
        char *root;        /* root */
        int  heap_sz_mb;   /* heap_mb */
        bool security;     /* security */
        bool filelist;     /* sendfilelist */
        bool cgi;          /* cgi */
        char *py;          /* pycgi */
    };
    
## References
    [Project site]("")
