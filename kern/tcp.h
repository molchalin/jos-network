#ifndef JOS_KERN_TCP_H
#define JOS_KERN_TCP_H

#include <inc/types.h>



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

#define TCP_DATA_LENGTH 1024
#define TCP_HEADER_LENGTH sizeof(struct tcp_hdr)

struct tcp_pkt {
    struct tcp_hdr hdr;
    uint8_t data[TCP_DATA_LENGTH];
} __attribute__((packed));

struct tcp_endpoint {
    uint32_t ip;
    uint16_t port;
};

enum tcp_state {
    CSTATE_ESTABLISHED,
    SYN_SENT,
    SYN_RECEIVED,
    SYN_ACK_RECEIVED,
    SYN_ACK_SENT,
    CSTATE_CLOSED,
    FIN_SENT,
};

struct tcp_block {
    uint32_t snd_uno;
    uint32_t snd_nxt;
    uint32_t snd_wnd;
    uint32_t iss;

    uint32_t rcv_nxt;
    uint32_t rcv_wnd;
    uint32_t irs;
};

struct tcp_context {
    enum tcp_state state;
    struct tcp_endpoint tcp_local;
    struct tcp_endpoint tcp_foreign;
};

#define TCP_CXT_LEN 32
struct tcp_context tcp_ctx[TCP_CXT_LEN];

/* syscalls wrapper from user space */
int tcp_create(uint32_t* socket);
int tcp_bind(uint32_t soket, struct tcp_endpoint* endpoint);
int tcp_connect(uint32_t socket, struct tcp_endpoint* endpoint);

int tcp_write(uint32_t socket, uint8_t* data, size_t lenght);
int tcp_read(uint32_t soket, uint8_t* data);

/* to network serve */
int tcp_send(struct tcp_context* ctx, struct tcp_pkt* pkt, size_t length);
int tcp_recv(struct ip_pkt* recv_ip, struct tcp_pkt* pkt);

void serve();

#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20

#endif /* !JOS_KERN_TCP_H */
