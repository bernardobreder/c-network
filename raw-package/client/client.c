#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEST "127.0.0.1"

int main(void)
{
    
    int s;
    struct sockaddr_in daddr;
    unsigned char packet[60];
    /* point the iphdr to the beginning of the packet */
    struct ip *ip = (struct ip *)packet;
    
    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    
    daddr.sin_family = AF_INET;
    daddr.sin_port = 0; /* not needed in SOCK_RAW */
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(packet, 'A', sizeof(packet));   /* payload will be all As */
    
    ip->ip_hl = 5;
    ip->ip_v = 4;
    ip->ip_tos = 0;
    ip->ip_len = htons(40);    /* 16 byte value */
    ip->ip_off = 0;        /* no fragment */
    ip->ip_ttl = 64;            /* default value */
    ip->ip_p = IPPROTO_RAW;    /* protocol at L4 */
    ip->ip_sum = 0xEEEE;
    ip->ip_src = daddr.sin_addr;
    ip->ip_dst = daddr.sin_addr;

    int i = 0;
    while (i < sizeof(packet)) {
        fprintf(stderr, "%02X ", packet[i]);
        i++;
    }
    printf("\n");
    
    printf("sizeof(sockaddr_in) = %ld\n", sizeof(struct sockaddr_in));
    
    while(1) {
        if (sendto(s, (char *)packet, sizeof(packet), 0,
                   (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0)
            perror("packet send error:");
        usleep(0.1f * 1000000.0f);
    }
}
