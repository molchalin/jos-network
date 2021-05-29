struct ip_hdr {
    uint8_t    ip_verlen;        // 4-bit IPv4 version 4-bit header length (in 32-bit words)
    uint8_t    ip_tos;           // IP type of service
    uint16_t   ip_totallength;   // Total length
    uint16_t   ip_id;            // Unique identifier
    uint16_t   ip_offset;        // Fragment offset field
    uint8_t    ip_ttl;           // Time to live
    uint8_t    ip_protocol;      // Protocol(TCP,UDP etc)
    uint16_t   ip_checksum;      // IP checksum
    uint32_t   ip_srcaddr;       // Source address
    uint32_t   ip_destaddr;      // Source address
}  __attribute__((packed));


int ip_send(const char* buf, size_t size, uint32_t *src, uint32_t *dest, uint8_t ttl, uint8_t tos, uint8_t proto);




#define IPH_V(hdr)  ((hdr)->ip_verlen & 0xf)
#define IPH_HL(hdr) (((hdr)->ip_verlen >> 4) & 0x0f)

#define IP_PROTO_ICMP    1
#define IP_PROTO_UDP     17
#define IP_PROTO_TCP     6
