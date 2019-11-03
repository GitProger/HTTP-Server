#include "wide.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include "nets.h"
#include "server.h"

int main(int argc, char *argv[]) {
    server_init();
    return server();
}
