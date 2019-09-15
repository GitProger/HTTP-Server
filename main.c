#include "wide.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include "server.h"
#include "client.h"
#include "bit.h"

int main(int argc, char *argv[]) {
    server_init();
    char ip[16] = "127.0.0.1", port[6] = "80", buffer[16];
    if (argc >= 3) {
        strcpy(ip, argv[1]);
        strcpy(port, argv[2]);
    } else if (argc == 2) {
        strcpy(ip, argv[1]);
        printf("Порт (default=%s): ", port);
        gets_s(buffer, 5);
        sscanf(buffer, "%s", port);
    } else if (argc <= 1) {
        printf("IP адрес (default=%s): ", ip);
        gets_s(buffer, 15);
        sscanf(buffer, "%s", ip);
        printf("Порт (default=%s): ", port);
        gets_s(buffer, 5);
        sscanf(buffer, "%s", port);
    }
    printf("Server run.\n");
    return server(ip, port);
}
