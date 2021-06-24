#include <kern/tcp.h>
#include <kern/inet.h>
#include <kern/ip.h>
#include <string.h>

#include <inc/stdio.h>
// internal begin

uint32_t
_get_isn() {
    static uint32_t isn = 42;
    isn = (isn << 2) + 42;
    return isn;
}

int
_tcp_send_syn_ack(struct tcp_context* ctx) {
    struct tcp_pkt result;
    struct tcp_hdr* hdr = &result.hdr;
    hdr->th_sport = ctx->tcp_local.port;
    hdr->th_dport = ctx->tcp_foreign.port;
    hdr->th_seq = ctx->tcp_block.seq;
    hdr->th_ack = ctx->tcp_block.ack;
    hdr->th_off = TCP_HEADER_LENGTH >> 4;
    hdr->syn = 1;
    hdr->ack = 1;
    hdr->th_win = TCP_WIN_SIZE;
    hdr->th_urp = 0;

    return tcp_send(ctx, &result, 0);
}

int
_tcp_listen(struct tcp_context* ctx, struct tcp_pkt* pkt) {
    struct tcp_hdr* hdr = &pkt->hdr;
    if (hdr->syn) {
        ctx->tcp_block.ack = hdr->th_seq + 1;
    }
    ctx->tcp_block.seq = _get_isn();
    ctx->state = SYN_RECEIVED;
    return _tcp_send_syn_ack(ctx);
}


// internal end


int
tcp_create(uint32_t* socket) {
    return 0;
}

int
tcp_bind(uint32_t socket, uint32_t ip, uint16_t port) {
    for (size_t i = 0; i < TCP_CXT_NUM; ++i) {
        if (tcp_ctx[i].socket_id == -1) {
            tcp_ctx[i].socket_id = socket;
            tcp_ctx[i].state = CSTATE_CLOSED;
            tcp_ctx[i].tcp_local.ip = ip;
            tcp_ctx[i].tcp_local.port = port;
            return 0;
        }
    }
    return -1;
}


int
tcp_connect(uint32_t socket, struct tcp_endpoint* endpoint) {
    return 0;
}

int
tcp_write(uint32_t socket, uint8_t* data, size_t lenght) {
    return 0;
}

int
tcp_read(uint32_t socket, uint8_t* data) {
    return 0;
}

int
tcp_send(struct tcp_context* ctx, struct tcp_pkt* pkt, size_t length) {
    struct ip_pkt result;
    struct ip_hdr* hdr = &result.hdr;

    hdr->ip_protocol = IP_PROTO_TCP;
    hdr->ip_srcaddr = ctx->tcp_local.ip;
    hdr->ip_destaddr = ctx->tcp_foreign.ip;

    size_t payload_sz = TCP_HEADER_LENGTH + length;
    memcpy((void*)result.data, (void*)pkt, payload_sz);

    int res = ip_send(&result, payload_sz);
    return res;
}

int
tcp_recv(struct ip_pkt* recv_ip) {
    cprintf("tcp recv enter\n");
    struct tcp_pkt pkt;
    memcpy((void*)&pkt, (void*)recv_ip->data, recv_ip->hdr.ip_totallength - IP_HEADER_LEN);
    for (size_t i = 0; i < TCP_CXT_NUM; ++i) {
        if (tcp_ctx[i].tcp_local.port == pkt.hdr.th_dport) {
            switch (tcp_ctx[i].state) {
            case LISTEN:
                tcp_ctx[i].tcp_foreign.ip = recv_ip->hdr.ip_srcaddr;
                tcp_ctx[i].tcp_foreign.port = pkt.hdr.th_sport;
                return _tcp_listen(&tcp_ctx[i], &pkt);
            case CSTATE_CLOSED:
                return -1;
            default:
                return -1;
            }
        }
    }
    return 0;
}


int tcp_listen(uint32_t socket) {
    for (size_t i = 0; i < TCP_CXT_NUM; ++i) {
        if (tcp_ctx[i].socket_id == socket) {
            tcp_ctx[i].state = LISTEN;
            return 0;
        }
    }
    return -1;
}

void
tcp_init() {
    for (size_t i = 0; i < TCP_CXT_NUM; ++i) {
        tcp_ctx[i].socket_id = -1;
    }
}
