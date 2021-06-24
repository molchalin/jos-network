#include <kern/net.h>

#include <kern/inet.h>
#include <kern/ip.h>
#include <kern/tcp.h>
#include <inc/stdio.h>

#define NET_LIMIT 2
#define BUF_SIZE  1024


void
net_init() {
    tcp_init();
}

void
net_serve() {
    for (size_t i = 0; i < NET_LIMIT; ++i) {
        struct ip_pkt pkt;
        int res = ip_recv(&pkt);
        if (res < 0) {
            //cprintf("error in ip_recv\n");
            continue;
        }
        cprintf("normal ip_recv\n");
    }
}
