/* hello, world */
#include <inc/lib.h>

void
umain(int argc, char **argv) {
    cprintf("hello, world\n");

    envid_t env = sys_getenvid();

    uint32_t socket = sys_create_socket(env, 1);
    if (socket < 0) {
        cprintf("no socket\n");
        return;
    }
    cprintf("socket: %d\n", socket);
    int res = 0;
    uint32_t ip = 1;
    uint16_t port = 80;
    res = sys_bind_socket(env, socket, ip, port);
    if (res < 0) {
        cprintf("no bind\n");
        return;
    }
    cprintf("bind\n");
    res = sys_listen_socket(env, socket);
    if (res < 0) {
        cprintf("no listen\n");
        return;
    }
    cprintf("listen\n");
}
