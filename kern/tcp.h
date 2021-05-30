#ifndef JOS_KERN_TCP_H
#define JOS_KERN_TCP_H

#include <inc/types.h>

enum tcp_state {
    CSTATE_ESTABLISHED,
    SYN_SENT,
    SYN_RECEIVED,
    SYN_ACK_RECEIVED,
    SYN_ACK_SENT,
    CSTATE_CLOSED,
    FIN_SENT,
};

struct tcp_hdr {
    uint16_t th_sport;  /* source port */
    uint16_t th_dport;  /* destination port */
    uint32_t th_seq;    /* sequence number */
    uint32_t th_ack;    /* acknowledgement number */
    uint8_t th_off;     /* data offset */
    uint8_t th_reserve; /* unused */
    uint8_t th_flags;   /* tcp flags */
    uint16_t th_win;    /* window */
    uint16_t th_sum;    /* checksum */
    uint16_t th_urp;    /* urgent pointer (unused in STCP) */
} __attribute__((packed));

#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20

#endif /* !JOS_KERN_TCP_H */
