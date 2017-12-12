#ifndef __XSOCKET_H__
#define __XSOCKET_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#define DEBUG(x, Y, z, m, r) do{        \
    if(x Y z)                           \
    {                                   \
        perror(m);                      \
        printf("FILE ; %s", __FILE__);  \
        printf("LINE ; %s", __LINE__);  \
        exit(r);                        \
    }                                   \
}while(0)

#if defined(__cplusplus)
extern "C"
{
#endif

typedef void (*vpfun)(int cfd, struct sockaddr_in *caddr);
void *thread_handler(void *arg);

enum xway{PPC, TPC, SELECT_TPC, POLL_TPC, EPOLL_TPC};

struct param_s 
{
    enum xway way;
    int type;       //type for socket
    in_port_t port; //port for bind
    char ipaddr[16];//IPv4 for bind
    int backlog;    //backlog for listen
    bool daemon;    //deamon for xSocketd
    nfds_t psize;   //file descripter for poll or epoll
};

struct thread_arg
{
    int cfd;
    struct sockaddr_in caddr;
    vpfun msg_handler; 
};

struct param_s xsocketd_init(int type, in_port_t port, const char *ipaddr, int backlog,  enum xway way, nfds_t npfd, bool daemon);
void param_juge(int type, in_port_t port, const char *ipaddr, int backlog, enum xway way, nfds_t npfd);
void xsocketd_start(const struct param_s *param, vpfun msg_handler);

void ppc(int sfd, vpfun msg_handler);                       //multi process 
void tpc(int sfd, vpfun msg_handler);                       //multi thread
void select_tpc(int sfd, vpfun msg_handler);                //select with multi thread 
void poll_tpc(int sfd, nfds_t psize, vpfun msg_handler);    //poll with multi thread 
void epoll_tpc(int sfd, nfds_t psize, vpfun msg_handler);   //epoll with multi thread

#if defined(__cplusplus)
}
#endif

#endif /*__XSOCKET_H__*/
