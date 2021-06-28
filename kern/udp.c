#include <kern/udp.h>
#include <kern/inet.h>
#include <string.h>
#include <inc/stdio.h>

int
udp_send(void* data, int length) {
    struct udp_pkt pkt;
    struct udp_hdr* hdr = &pkt.hdr;
    hdr->src_port = JHTONS(8081);
    hdr->dst_port = JHTONS(1234);
    hdr->length = JHTONS(length + sizeof(struct udp_hdr));
    hdr->checksum = 0;
    memcpy((void*)pkt.data, data, length);
    struct ip_pkt result;
    result.hdr.ip_protocol = IP_PROTO_UDP;
    memcpy((void*)result.data, (void*)&pkt, length + sizeof(struct udp_hdr));
    return ip_send(&result, length + sizeof(struct udp_hdr));
}

int
udp_recv(struct ip_pkt* pkt) {
    struct udp_pkt upkt;
    int size = JNTOHS(pkt->hdr.ip_totallength - IP_HEADER_LEN);
    memcpy((void*)&upkt, (void*)pkt->data, size);
    struct udp_hdr* hdr = &upkt.hdr;
    cprintf("port: %d\n", hdr->dst_port);
    for (size_t i = 0; i < hdr->length; ++i) {
        cprintf("%c", upkt.data[i]);
    }
    cprintf("\n");
    return 0;
}
