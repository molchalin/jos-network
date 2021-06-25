#ifndef JOS_KERN_ETH_H
#define JOS_KERN_ETH_H

#include <inc/types.h>
#include <kern/e1000.h>

struct eth_hdr {
    uint8_t eth_dmac[6];
    uint8_t eth_smac[6];
    uint16_t eth_type;
    uint8_t data[];
} __attribute__((packed));

int eth_send(struct eth_hdr* hdr, void* data, size_t len);
int eth_recv(struct eth_hdr* hdr, void* data);

char qemu_mac[6] = {0x52, 0x54, 0x0, 0x12, 0x34, 0x56};
#define ETH_TYPE_ARP 0x0806


#endif /* !JOS_KERN_ETH_H */
