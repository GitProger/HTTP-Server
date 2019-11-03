#ifndef __LOG_H_INC
#define __LOG_H_INC
#include "deepsys.h"
#include "readconf.h"
FILE *logFileG, *errFileG;

static void LogEvent(const char *text, FILE *log) {
    char *strTime = timeStr();
    fprintf(log, "[%s] %s\n", strTime, text);
    fflush(log);
    free(strTime);
}

#define LogError(__text, __err_f_nm) \
    { \
    char *__M_tm_loc_var = timeStr(); \
    fprintf(__err_f_nm, \
         "[%s] %s | line: %d | function: %s | file: %s\n", \
          __M_tm_loc_var, (__text), __LINE__, \
          __PRETTY_FUNCTION__, __FILE__); \
    free(__M_tm_loc_var); \
    fflush(__err_f_nm); \
    }

void InitLogsFiles()
{
    static char name[MAX_FILE_NAME_LEN];

    sprintf(name, "%s\\event_log.txt", ServerConfig.log);
    logFileG = fopen(name, "a");
    if (!logFileG)
        logFileG = fopen(name, "w");

    sprintf(name, "%s\\error_log.txt", ServerConfig.log);
    errFileG = fopen(name, "a");
    if (!errFileG)
        errFileG = fopen(name, "w");
        
    LogEvent("Initialized.", logFileG);    
}

#endif
