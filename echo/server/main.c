/*** IPPROTO_RAW receiver ***/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int s;
    struct sockaddr_in saddr;
    unsigned char packet[60];
    
    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }
    
    memset(packet, 0, sizeof(packet));
    socklen_t fromlen = sizeof(saddr);
    
    while(1) {
        ssize_t readed = recvfrom(s, (char *)&packet, sizeof(packet), 0, (struct sockaddr *)&saddr, &fromlen);
        if (readed < 0) {
            perror("packet receive error:");
            break;
        }
        printf("Package received: %ld\n", readed);
        printf("Package size: %ld\n", sizeof(struct ip));
        int i = 0;
        while (i < sizeof(packet)) {
            fprintf(stderr, "%02X ", packet[i]);
            i++;
        }
        printf("\n");
    }
    close(s);
}
