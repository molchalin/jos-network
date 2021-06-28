#ifndef JOS_KERN_ARP_H
#define JOS_KERN_ARP_H

#include <inc/types.h>

#define ARP_ETHERNET 0x0001
#define ARP_IPV4     0x0800
#define ARP_REQUEST  0x0001
#define ARP_REPLY    0x0002

#define ARP_CACHE_LEN 32

#define ARP_FREE     0
#define ARP_WAITING  1
#define ARP_RESOLVED 2

struct arp_hdr {
    uint16_t hwtype;
    uint16_t protype;
    uint8_t hwsize;
    uint8_t prosize;
    uint16_t opcode;
    uint8_t data[];
} __attribute__((packed));

struct arp_ipv4 {
    uint8_t smac[6];
    uint32_t sip;
    uint8_t dmac[6];
    uint32_t dip;
} __attribute__((packed));

struct arp_cache_entry {
    uint32_t sip;
    uint8_t smac[6];
    unsigned int state;
};

void arp_incoming(void* data, size_t size);
int arp_ipv4_get(uint32_t dip, uint8_t* mac);

#endif /* !JOS_KERN_ARP_H */
