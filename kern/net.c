#include <kern/net.h>

#include <kern/inet.h>
#include <kern/ip.h>
#include <kern/eth.h>
#include <kern/tcp.h>
#include <inc/stdio.h>
#include <inc/string.h>

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
            cprintf("error in ip_recv\n");
            cprintf("%d\n", res);
            continue;
        }
        cprintf("normal ip_recv\n");
    }

    for (size_t i = 0; i < NET_LIMIT; ++i) {
        struct eth_hdr hdr;
        memset(hdr.eth_dmac, 0xFF, 6);
        char buf[666];
        memset(buf, 0xEE, 666);
        int res = eth_send(&hdr, &buf, 666);
        if (res < 0) {
            cprintf("error in eth_send\n");
            cprintf("%d\n", res);
            continue;
        }
        cprintf("normal\n");
    }
}
