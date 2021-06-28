#include <inc/string.h>
#include <inc/stdio.h>

#include <kern/arp.h>
#include <kern/eth.h>
#include <kern/inet.h>

#define MY_IP               0xA000004
#define ETH_MAX_PACKET_SIZE 1500

static struct arp_cache_entry arp_cache[ARP_CACHE_LEN];

static int
insert_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data) {
    struct arp_cache_entry *entry;
    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) {
            entry->state = ARP_RESOLVED;

            entry->sip = data->sip;
            memcpy(entry->smac, data->smac, sizeof(entry->smac));

            return 0;
        }
    }

    return -1;
}

static int
update_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data) {
    struct arp_cache_entry *entry;

    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) continue;

        if (entry->sip == data->sip) {
            memcpy(entry->smac, data->smac, 6);
            return 1;
        }
    }

    return 0;
}

void
arp_reply(struct arp_hdr *arphdr) {
    struct arp_ipv4 *arpdata;
    int len;

    arpdata = (struct arp_ipv4 *)arphdr->data;

    memcpy(arpdata->dmac, arpdata->smac, 6);
    arpdata->dip = arpdata->sip;
    memcpy(arpdata->smac, qemu_mac, 6);
    arpdata->sip = MY_IP;

    arphdr->opcode = ARP_REPLY;

    arphdr->opcode = htons(arphdr->opcode);
    arphdr->hwtype = htons(arphdr->hwtype);
    arphdr->protype = htons(arphdr->protype);

    len = sizeof(struct arp_hdr) + sizeof(struct arp_ipv4);
    struct eth_hdr rpl_hdr;
    memcpy((void *)rpl_hdr.eth_dmac, (void *)arpdata->dmac, 6);
    rpl_hdr.eth_type = ETH_TYPE_ARP;

    int res = eth_send(&rpl_hdr, arphdr, len);
    if (res < 0) cprintf("arp_reply err: %i\n", res);
}


void
arp_incoming(void *data, size_t size) {
    struct arp_hdr *arphdr;
    struct arp_ipv4 *arpdata;
    int merge = 0;

    arphdr = (struct arp_hdr *)data;

    arphdr->hwtype = ntohs(arphdr->hwtype);
    arphdr->protype = ntohs(arphdr->protype);
    arphdr->opcode = ntohs(arphdr->opcode);

    if (arphdr->hwtype != ARP_ETHERNET) {
        cprintf("Unsupported HW type\n");
        return;
    }

    if (arphdr->protype != ARP_IPV4) {
        cprintf("Unsupported protocol\n");
        return;
    }

    arpdata = (struct arp_ipv4 *)arphdr->data;

    merge = update_arp_translation_table(arphdr, arpdata);

    if (arpdata->dip != MY_IP) {
        cprintf("bad ip\n");
        return;
    }

    if (!merge && insert_arp_translation_table(arphdr, arpdata) != 0) {
        cprintf("ERR: No free space in ARP translation table\n");
        return;
    }

    switch (arphdr->opcode) {
    case ARP_REQUEST:
        arp_reply(arphdr);
        break;
    default:
        cprintf("Opcode not supported\n");
        break;
    }
}

int
arp_ipv4_get(uint32_t dip, uint8_t *mac) {

    struct arp_cache_entry *entry;

    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) continue;

        if (entry->sip == dip) {
            if (entry->state == ARP_RESOLVED) {
                memcpy(mac, entry->smac, 6);
                return 0;
            } else if (entry->state == ARP_WAITING) {
                return -1; // EAGAIN
            }
        }
    }

    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) {
            entry->state = ARP_WAITING;

            entry->sip = dip;

            char buf[ETH_MAX_PACKET_SIZE + 1];
            struct arp_hdr *arphdr = (struct arp_hdr *)buf;
            /* TODO(a.eremeev) FILL arp_hdr fields  */
            struct arp_ipv4 *arpdata = (struct arp_ipv4 *)(arphdr->data);
            arpdata->sip = MY_IP;
            memcpy(arpdata->smac, qemu_mac, 6);

            int len = sizeof(struct arp_hdr) + sizeof(struct arp_ipv4);


            struct eth_hdr ethhdr;
            ethhdr.eth_type = ETH_TYPE_ARP;
            memset(ethhdr.eth_dmac, 0xFF, 6);

            int res = eth_send(&ethhdr, arphdr, len);
            if (res < 0) cprintf("arp_reply err: %i\n", res);
            return 0;
        }
    }
    return -1; // EAGAIN
}
