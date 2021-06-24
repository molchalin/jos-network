#include <kern/socket.h>
#include <kern/tcp.h>


struct socket* freep = NULL;
struct socket sockets[SOCKET_NUM];

int
create_socket(envid_t env, int protocol) {
    for (size_t i = 0; i < SOCKET_NUM; ++i) {
        if (sockets[i].state == SS_FREE) {
            sockets[i].state = SS_CONNECTING;
            sockets[i].socket_id = i;
            sockets[i].socket_protocol = protocol;
            sockets[i].socket_env = env;
            return sockets[i].socket_id;
        }
    }
    return -1;
}

int
bind_socket(envid_t env, uint32_t socket, uint32_t ip, uint16_t port) {
    if (socket >= SOCKET_NUM) {
        return -1;
    }
    struct socket* s = &sockets[socket];

    if (s->state != SS_CONNECTING) {
        return -1;
    }
    if (s->socket_env != env) {
        return -1;
    }
    int res = 0;
    if (s->socket_protocol == SP_TCP) {
        res = tcp_bind(socket, ip, port);
    }
    return res;
}


int listen_socket(envid_t env, uint32_t socket) {
    if (socket >= SOCKET_NUM) {
        return -1;
    }

    struct socket* s = &sockets[socket];

    if (s->socket_env != env) {
        return -1;
    }

    int res = 0;
    if (s->socket_protocol == SP_TCP) {
        res = tcp_listen(socket);
    }
    return res;
}
