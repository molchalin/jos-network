#ifndef JOS_KERN_SOCKET_H
#define JOS_KERN_SOCKET_H

#include <inc/types.h>
#include <inc/env.h>



enum socket_state {
    SS_FREE = 0, /* not allocated                */
    SS_UNCONNECTED,                 /* unconnected to any socket    */
    SS_CONNECTING,                  /* in process of connecting     */
    SS_CONNECTED,                   /* connected to socket          */
    SS_DISCONNECTING                /* in process of disconnecting  */
};

enum socket_protocol {
    SP_TCP = 0,
    SP_UDP
};

struct socket {
    uint32_t socket_id;
    envid_t socket_env;
    enum socket_state state;
    enum socket_protocol socket_protocol;
};

#define SOCKET_NUM 64

int create_socket(envid_t env, int protocol);
int bind_socket(envid_t env, uint32_t socket, uint32_t ip, uint16_t port);
int listen_socket(envid_t env, uint32_t socket);

int read_socket(envid_t env, uint32_t socket, uint8_t* data, size_t length);
int write_socket(envid_t env, uint32_t socket, uint8_t* data);

#endif /* !JOS_KERN_SOCKET_H */
