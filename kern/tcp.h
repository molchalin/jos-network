#ifndef JOS_KERN_TCP_H
#define JOS_KERN_TCP_H

#include <inc/types.h>
#include <kern/ip.h>



struct tcp_hdr {
    uint16_t th_sport;  /* source port */
    uint16_t th_dport;  /* destination port */
    uint32_t th_seq;    /* sequence number */
    uint32_t th_ack;    /* acknowledgement number */
    uint8_t th_off : 4;     /* data offset */
    uint8_t th_reserve : 4; /* unused */
    uint8_t fin : 1, /* tcp flags */
            syn : 1,
            rst : 1,
            psh : 1,
            ack : 1,
            urg : 1,
            ece : 1,
            cwr : 1;
    uint16_t th_win;    /* window */
    uint16_t th_sum;    /* checksum */
    uint16_t th_urp;    /* urgent pointer (unused in STCP) */
} __attribute__((packed));

#define TCP_DATA_LENGTH 1024
#define TCP_HEADER_LENGTH sizeof(struct tcp_hdr)
#define TCP_WIN_SIZE (1 << 10)

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
    LISTEN
};

struct tcp_block {
    uint32_t seq;
    uint32_t ack;
};

struct tcp_context {
    int socket_id;
    enum tcp_state state;
    struct tcp_endpoint tcp_local;
    struct tcp_endpoint tcp_foreign;
    struct tcp_block tcp_block;
    uint8_t data[1024];
    uint32_t data_len;
};

#define TCP_CXT_NUM 32
struct tcp_context tcp_ctx[TCP_CXT_NUM];

/* syscalls wrapper from user space */
int tcp_bind(uint32_t soket, uint32_t ip, uint16_t port);
int tcp_connect(uint32_t socket, struct tcp_endpoint* endpoint);
int tcp_listen(uint32_t socket);
int tcp_accept(uint32_t socket, struct tcp_endpoint* client_endpoint);

int tcp_write(uint32_t socket, uint8_t* data, size_t lenght);
int tcp_read(uint32_t soket, uint8_t* data);

/* to network serve */
int tcp_send(struct tcp_context* ctx, struct tcp_pkt* pkt, size_t length);
int tcp_recv(struct ip_pkt* recv_ip);


#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20

#endif /* !JOS_KERN_TCP_H */
