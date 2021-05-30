#include <kern/tcp.h>
#include <kern/inet.h>
#include <kern/ip.h>


int tcp_create(uint32_t* socket) {

}

int tcp_bind(uint32_t soket, struct tcp_endpoint* endpoint) {

}

int tcp_connect(uint32_t socket, struct tcp_endpoint* endpoint) {

}

int tcp_write(uint32_t socket, uint8_t* data, size_t lenght) {

}

int tcp_read(uint32_t soket, uint8_t* data) {

}

int tcp_send(struct tcp_context* ctx, struct tcp_pkt* pkt, size_t length);
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

int tcp_recv(struct ip_pkt* recv_ip, struct tcp_pkt* pkt) {
    int res = ip_recv(recv_ip);
    if (res < 0) {
        return res;
    }
    memcpy((void*)pkt, (void*)recv_ip->data, recv_ip.hdr.ip_totallength - IP_HEADER_LEN);
    return 0;
}
