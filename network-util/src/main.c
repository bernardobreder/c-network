//
//  main.c
//  network-util
//
//  Created by Bernardo Breder on 04/03/19.
//  Copyright © 2019 Bernardo Breder. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>

#include <signal.h>
#include <sysexits.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/errno.h>
#include <sys/fcntl.h>

typedef void signal_func (int);

ssize_t socket_read(int fd, unsigned char* buffer, size_t length) {
    return recv(fd, &buffer, length, MSG_DONTWAIT);
}

ssize_t socket_send(int fd, unsigned char* buffer, size_t length) {
    return send(fd, buffer, length, MSG_DONTWAIT);
}

int socket_accept(int fd) {
    struct sockaddr cliaddr;
    socklen_t clilen;
    int cfd = accept(fd, &cliaddr, &clilen);
    if (cfd <= 0) {
        return -1;
    }
    fcntl(cfd, F_SETFL, O_NONBLOCK);
    return fd;
}

void socket_close(int fd) {
    close(fd);
}

signal_func *set_signal_handler (int signo, signal_func * func) {
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction (signo, &act, &oact) < 0) return SIG_ERR;
    return oact.sa_handler;
}

int socket_create(const char *addr, uint16_t port, int max_queue) {
    struct addrinfo hints, *result;
    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    char portstr[6];
    snprintf (portstr, sizeof (portstr), "%d", port);
    
    if (getaddrinfo (addr, portstr, &hints, &result) != 0) {
        strerror(errno);
        return -1;
    }
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        strerror(errno);
        freeaddrinfo(result);
        return -1;
    }
    
    struct sockaddr_in ad;
    memset (&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = htons(INADDR_ANY);
    ad.sin_port = htons(port);
    
    const int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0) {
        strerror(errno);
        freeaddrinfo(result);
        close(fd);
        return -1;
    }
    
    if (hints.ai_family == AF_INET6) {
        if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) != 0) {
            strerror(errno);
            freeaddrinfo(result);
            close(fd);
            return -1;
        }
    }
    
    freeaddrinfo(result);

    if (bind(fd, (struct sockaddr *) &ad, sizeof(ad)) != 0) {
        strerror(errno);
        close(fd);
        return -1;
    }
    
    if (listen(fd, max_queue) != 0) {
        strerror(errno);
        close(fd);
        return -1;
    }
    return fd;
}

char* shell_user_info_dir() {
    struct passwd *thisuser = getpwnam("bernardobreder");
    return thisuser->pw_dir;
}

struct linkedlist_int_var_t {
    size_t size;
    struct linkedlist_int_var_node_t* first;
    struct linkedlist_int_var_node_t* last;
};

struct linkedlist_int_var_node_t {
    void* value;
    struct linkedlist_int_var_node_t* next;
};

struct linkedlist_int_var_t* linkedlist_int_var_create() {
    struct linkedlist_int_var_t* self = (struct linkedlist_int_var_t*) calloc(1, sizeof(struct linkedlist_int_var_t));
    return self;
}


void linkedlist_int_var_add_last(struct linkedlist_int_var_t* self, void* value) {
    struct linkedlist_int_var_node_t* node = (struct linkedlist_int_var_node_t*) calloc(1, sizeof(struct linkedlist_int_var_node_t));
    node->value = value;
    if (self->last) {
        node->next = self->last;
        self->last = node;
    } else {
        self->first = self->last = node;
    }
}

void linkedlist_int_var_remove_node(struct linkedlist_int_var_t* self, struct linkedlist_int_var_node_t* node) {
    
}

#define SOCKET_MAX 1024

int main() {
    if (set_signal_handler (SIGPIPE, SIG_IGN) == SIG_ERR) { exit(EX_OSERR); }
    
    int fd = socket_create("127.0.0.1", 3333, SOCKET_MAX);
    if (fd < 0) {
        return 1;
    }
    
    int cfds[SOCKET_MAX];
    int cfdi = 0;
    
//    while(1) {
//        unsigned found = 0;
//        int cdf = socket_accept(fd);
//        if (cdf > 0) {
//            found = 1;
//        }
//        
//    }
    
    close(fd);
    return 0;
}
