#include <kern/udp.h>
#include <kern/inet.h>
#include <string.h>

int
udp_send(void* data, int length) {
    struct udp_pkt pkt;
    struct udp_hdr* hdr = &pkt.hdr;
    hdr->src_port = 0;
    hdr->dst_port = JHTONS(1234);
    hdr->length = JHTONS(length + sizeof(struct udp_hdr));
    hdr->checksum = 0;
    memcpy((void*)pkt.data, data, length);
    struct ip_pkt result;
    memcpy((void*)result.data, (void*)&pkt, length + sizeof(struct udp_hdr));
    return ip_send(&result, length + sizeof(struct udp_hdr));
}
