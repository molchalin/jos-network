#include <kern/tcp.h>

#define RECV_NUM 10
#define SEND_NUM 10

void serve() {
    int res = 0;
    for (size_t i = 0; i < RECV_NUM; ++i) {
        struct ip_pkt recv_ip;
        struct tcp_pkt recv_tcp;
        res = tcp_recv(&recv_ip, &recv_tcp);
        if (res < 0) {
            break;
        }
    }

    for (size_t i = 0; i < TCP_CTX_LEN; ++i) {
        if (tcp_ctx[i].state == tcp_state::CSTATE_CLOSE) {
            continue;
        }
    }
}



