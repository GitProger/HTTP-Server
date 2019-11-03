#include "wide.h"
#include "nets.h"
#include "server.h"

int main(int argc, char *argv[]) {
    server_init();
    return server();
}
