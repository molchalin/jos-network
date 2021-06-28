#include <kern/ip.h>
#include <inc/string.h>
#include <kern/inet.h>
#include <inc/error.h>
#include <kern/eth.h>
#include <kern/tcp.h>

uint16_t packet_id = 0;

uint16_t
ip_checksum(void* vdata, size_t length) {
    char* data = vdata;
    uint32_t acc = 0xffff;
    for (size_t i = 0; i + 1 < length; i += 2) {
        uint16_t word;
        memcpy(&word, data + i, 2);
        acc += ntohs(word);
        if (acc > 0xffff) {
            acc -= 0xffff;
        }
    }
    // Handle any partial block at the end of the data.
    if (length & 1) {
        uint16_t word = 0;
        memcpy(&word, data + length - 1, 1);
        acc += ntohs(word);
        if (acc > 0xffff) {
            acc -= 0xffff;
        }
    }
    // Return the checksum in network byte order.
    return htons(~acc);
}

/*
 * pkt - ip packet
 * length - size of data in pkt in bytes
*/

int
ip_send(struct ip_pkt* pkt, uint16_t length) {
    uint16_t id = packet_id++;

    struct ip_hdr* hdr = &pkt->hdr;
    hdr->ip_verlen = IP_VER_LEN;
    hdr->ip_tos = 0;
    hdr->ip_totallength = htons((length + IP_HEADER_LEN) / sizeof(uint8_t));
    hdr->ip_id = htons(id);
    hdr->ip_offset = 0; // TODO fragmentation
    hdr->ip_protocol = htons(17);
    hdr->ip_ttl = IP_TTL;

    hdr->ip_checksum = ip_checksum((void*)pkt, IP_HEADER_LEN);
    struct eth_hdr e_hdr;
    e_hdr.eth_type = 0x0008;
    char hard_code[6] = {0x3a, 0xbe, 0x6d, 0xa0, 0xaf, 0x00};
    memcpy(e_hdr.eth_dmac, hard_code, 6);
    return eth_send(&e_hdr, (void*)pkt, sizeof(struct ip_hdr) + length);
}

int
ip_recv(struct ip_pkt* pkt) {
    //int res = rx_packet((void*)pkt, sizeof(*pkt));
    int res = rx_packet((void*)pkt);
    if (res < 0) {
        return res;
    }
    struct ip_hdr* hdr = &pkt->hdr;
    if (hdr->ip_verlen != IP_VER_LEN) {
        return -E_UNS_VER;
    }
    uint16_t checksum = hdr->ip_checksum;
    hdr->ip_checksum = 0;
    if (checksum != ip_checksum((void*)pkt, IP_HEADER_LEN)) {
        return -E_INV_CHS;
    }
    if (hdr->ip_protocol == IP_PROTO_TCP) {
        tcp_recv(pkt);
    }
    return 0;
}
