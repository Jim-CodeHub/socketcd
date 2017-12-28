# xSocketd
## Introduction:
    xSocketd is a series of server-side linux socket functions whit C language.
    x reference choice TCP/UDP(developing)/OTHERS(developing).

    For TCP/IP protocal:
        enum xway{NONE, PPC, TPC, SELECT_TPC, POLL_TPC, EPOLL_TPC};
        NONE-> single server
        PPC -> process per connection or multi process
        TPC -> thread per connection or multi thread
        SELECT_TPC -> select with multi thread 
        POLL_TPC -> poll with multi thread
        EPOLL_TPC -> epoll with multi thread

    It's hard to say, which one has a well performence for your project,so, just try them all if it's possible.

## Install
    #make && make install

## Usage 

### Init 
    struct param_s
    xsocketd_init(int type, in_port_t port, const char *ipaddr, int backlog,  enum xway way, nfds_t npfd, bool daemon);
    
### Start
    void
    xsocketd_start(const struct param_s *param, vpfun msg_handler);

### msg_handler
    void
    msg_handler(int cfd, struct sockaddr_in *caddr)
    {/*Your Code*/}

## Note
    xSocketd is flexible, it's not handle client's performances, include socket options(for client fd), recive, send etc.       
    the behavior is implementation dependent in function msg_handler().


## Demo
    //main.c

    #include "xsocketd.h"

    #define PORT 9095
    #define ADDR "192.168.14.144" 

    void msg_handler(int cfd, struct sockaddr_in *caddr)
    {
        char buff[1023];
        recv(cfd, buff, sizeof(buff), 0);
        send(cfd, "back", 5, 0);
    }

    int main(void)
    {
        struct param_s param = xsocketd_init(SOCK_STREAM, PORT, ADDR, 50, EPOLL_TPC, 1000, false);
        xsocketd_start(&param, msg_handler);

        /*SHOULD NOT BE ANY CODES BELOW!*/

        return 0;
    }

    //command line
    #gcc main.c -lxsocketd -lpthread
