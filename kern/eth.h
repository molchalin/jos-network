#ifndef JOS_KERN_ETH_H
#define JOS_KERN_ETH_H

#include <inc/types.h>
#include <kern/e1000.h>

struct eth_hdr {
    uint8_t eth_dmac[6];
    uint8_t eth_smac[6];
    uint16_t eth_type;
} __attribute__((packed));

int eth_send(struct eth_hdr* hdr, char* data, size_t len);
int eth_recv(struct eth_hdr* hdr, char* data);


#endif /* !JOS_KERN_ETH_H */
