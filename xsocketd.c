//#include <xsocketd.h> 
#include "xsocketd.h" 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_handler(void *arg)
{
    struct thread_arg targ = *((struct thread_arg *)arg);  
    pthread_mutex_unlock(&mutex);

    targ.msg_handler(targ.cfd, &(targ.caddr));
    close(targ.cfd);

    pthread_exit(NULL);
}

struct param_s xsocketd_init(enum xtype type, in_port_t port, const char *ipaddr, int backlog,  enum xway way, nfds_t psize, bool daemon)
{
    struct param_s param;
    switch(type)
    {
        case xSOCK_STREAM: 
            param.type = SOCK_STREAM;
            break;
        case xSOCK_DGRAM:
            param.type = SOCK_DGRAM;
            break;
        case xSOCK_RAW:
            param.type = SOCK_RAW;
            break;
        default:
            puts("Param type is bad!");
    }

    param_juge(port, ipaddr, backlog, way, psize);

    param.way = way;
    param.port = port;
    strcpy(param.ipaddr, ipaddr);
    param.backlog = backlog;
    param.daemon = daemon;
    param.psize = psize;

    return param;
}

void param_juge(in_port_t port, const char *ipaddr, int backlog, enum xway way, nfds_t psize)
{
    if(port < 0)
    {
        puts("Param port is negative"); 
        exit(-1);
    }

    if(NULL == ipaddr)
    {
        puts("Param addr is null"); 
        exit(-1);
    }

    if(backlog <= 0)
    {
        puts("Param backlog should be positive"); 
        exit(-1);
    }
    else
    {
        int fd = open("/proc/sys/net/core/somaxconn", O_RDONLY);
        DEBUG(fd, ==, -1, "open error ", -1);

        char buff[10];
        read(fd, buff, 10);
        close(fd);

        if(backlog < atoi(buff))
            backlog = atoi(buff);
    }

    if(way == POLL_TPC && psize <= 0)
    {
        puts("Param way has been set POLL, param pize should be positive"); 
        exit(-1);
    }
}

void xsocketd_start(const struct param_s *param, vpfun msg_handler)
{
    int ret = 0;
    int opt = 1;
    int cfd, sfd;
    struct sockaddr_in saddr;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(param->port);
    saddr.sin_addr.s_addr = inet_addr(param->ipaddr);
    bzero(saddr.sin_zero, sizeof(saddr.sin_zero));

    if(param->daemon == true)
    {
        ret = daemon(0, 0);
        DEBUG(ret, ==, -1, "daemon error ", -1);
    }

    sfd = socket(AF_INET, param->type, 0);
    DEBUG(sfd, ==, -1, "socket error ", -1);

    ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    DEBUG(ret, ==, -1, "setsockopt error ", -1);

    ret = bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr)); 
    DEBUG(ret, ==, -1, "bind error ", -1);

    if(param->type == SOCK_STREAM)
    {
        ret = listen(sfd, param->backlog); 
        DEBUG(ret, ==, -1, "listen error ", -1);

        switch(param->way)
        {
            case NONE:
                none(sfd, msg_handler); 
                break;
            case PPC: 
                ppc(sfd, msg_handler);
                break;
            case TPC:
                tpc(sfd, msg_handler);
                break;
            case SELECT_TPC:
                select_tpc(sfd, msg_handler);
                break;
            case POLL_TPC:
                poll_tpc(sfd, param->psize, msg_handler);
                break;
            case EPOLL_TPC:
                epoll_tpc(sfd, param->psize, msg_handler);
                break;
        }
    }

    /*if(param->type == SOCK_DGRAM)*/ //<TBD>
    /*if(param->type == SOCK_RAW)*/ //<TBD>
} 

void none(int sfd, vpfun msg_handler)
{
    int ret, cfd;
    socklen_t len;
    struct sockaddr_in caddr;
    len = sizeof(caddr);

    bzero(&caddr, len);
    cfd = accept(sfd, (struct sockaddr*)&caddr, &len);
    DEBUG(cfd, ==, -1, "accept error ", -1);

    msg_handler(cfd, &caddr);

    close(cfd);
}

void ppc(int sfd, vpfun msg_handler)
{
    int ret, pid, cfd;
    socklen_t len;
    struct sockaddr_in caddr;
    len = sizeof(caddr);

    while(true)
    {
        bzero(&caddr, len);
        cfd = accept(sfd, (struct sockaddr*)&caddr, &len);
        DEBUG(cfd, ==, -1, "accept error ", -1);

        signal(SIGCHLD, SIG_IGN);

        pid = fork();
        if(0 == pid)
        {
            close(sfd);

            msg_handler(cfd, &caddr);

            close(cfd);
            raise(SIGKILL);
        }
        close(cfd);
    }
}

void tpc(int sfd, vpfun msg_handler)
{
    int ret, cfd;
    socklen_t len;
    pthread_t tid;
    struct thread_arg targ;
    struct sockaddr_in caddr;
    len = sizeof(caddr);

    while(true)
    {
        bzero(&caddr, len);
        cfd = accept(sfd, (struct sockaddr *)&caddr, &len);
        DEBUG(cfd, ==, -1, "accept error ", -1);

        pthread_mutex_lock(&mutex);

        bzero(&targ, sizeof(targ));
        targ.cfd = cfd;
        targ.caddr = caddr;
        targ.msg_handler = msg_handler;

        ret = pthread_create(&tid, NULL, thread_handler, &targ);
        DEBUG(ret, !=, 0, "pthread_create error ", -1);

        ret = pthread_detach(tid);
        DEBUG(ret, !=, 0, "pthread_detach error ", -1);
    }
}

void select_tpc(int sfd, vpfun msg_handler)
{
    int ret, i, cfd, maxfd;
    socklen_t len;
    pthread_t tid;
    int bakfd[FD_SETSIZE]; 
    fd_set tmp_set, all_set;
    struct thread_arg targ;
    struct sockaddr_in caddr;

    maxfd = sfd;
    len = sizeof(caddr);

    memset(bakfd, -1, sizeof(int)*FD_SETSIZE);
    FD_ZERO(&tmp_set);
    FD_ZERO(&all_set);
    FD_SET(sfd, &all_set);

    while(true)
    {
        tmp_set = all_set;
        ret = select(maxfd + 1, &tmp_set, NULL, NULL, NULL);
        DEBUG(ret, ==, -1, "select error ", -1);

        if(FD_ISSET(sfd, &tmp_set))
        {
            bzero(&caddr, len);
            cfd = accept(sfd, NULL, NULL);
            DEBUG(cfd, ==, -1, "accept error ", -1);

            FD_SET(cfd, &all_set);

            for(i=0; i<FD_SETSIZE; i++)
            {
                if(-1 == bakfd[i])
                {
                    bakfd[i] = cfd;
                    break;
                }
            }

            for(i=0; i<FD_SETSIZE; i++)
            {
                if(bakfd[i] > maxfd) 
                {
                    maxfd = bakfd[i]; 
                    break;
                }
            }
        }
        else //"else" can be ignored, the server performences depends on clients. 
        {
            for(i=0; i<FD_SETSIZE; i++)
            {
                if(-1 == bakfd[i]) 
                    continue;

                if(FD_ISSET(bakfd[i], &tmp_set))
                {
                    FD_CLR(bakfd[i], &all_set);

                    pthread_mutex_lock(&mutex);

                    ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);
                    DEBUG(ret, ==, -1, "getpeername error ", -1);

                    bzero(&targ, sizeof(targ));
                    targ.msg_handler = msg_handler;
                    targ.caddr = caddr;
                    targ.cfd = bakfd[i];

                    bakfd[i] = -1;

                    ret = pthread_create(&tid, NULL, thread_handler, &targ);
                    DEBUG(ret, !=, 0, "pthread_create error ", -1);

                    ret = pthread_detach(tid);
                    DEBUG(ret, !=, 0, "pthread_detach error ", -1);
                }
            }
        }
    }
    close(sfd);
}

void poll_tpc(int sfd, nfds_t psize, vpfun msg_handler)
{
    int ret, cfd, i;
    socklen_t len;
    pthread_t tid;
    nfds_t maxnfd;
    nfds_t countfd;
    struct pollfd pfd[psize];
    struct thread_arg targ;
    struct sockaddr_in caddr;

    maxnfd = 0;
    countfd = 0;
    memset(pfd, -1, sizeof(struct pollfd)*psize);
    pfd[0].fd = sfd;
    pfd[0].events = POLLIN;

    len = sizeof(caddr);

    while(true)
    {
        ret = poll(pfd, maxnfd+1, -1);
        DEBUG(ret, ==, -1, "poll erro ", -1);

        if(pfd[0].revents & POLLIN)
        {
            bzero(&caddr, len);
            cfd = accept(sfd, NULL, NULL); 
            DEBUG(cfd, ==, -1, "accept erro ", -1);

            for(i=1; i<psize; i++)
            {
                if(-1 == pfd[i].fd) 
                {
                    pfd[i].fd = cfd; 
                    pfd[i].events = POLLIN;
                    countfd++;
                    break;
                }
            }

            if(countfd > maxnfd)
                maxnfd = countfd;
        }
        else //"else" can be ignored, the server performences depends on clients. 
        {
            for(i=1; i<psize; i++)
            {  
                if(-1 == pfd[i].fd)
                    continue;

                if(pfd[i].revents & POLLIN)
                {  
                    pthread_mutex_lock(&mutex);

                    ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);
                    DEBUG(ret, ==, -1, "getpeername error ", -1);

                    bzero(&targ, sizeof(targ));
                    targ.msg_handler = msg_handler;
                    targ.caddr = caddr;
                    targ.cfd = pfd[i].fd;

                    pfd[i].fd = -1;
                    countfd--;

                    ret = pthread_create(&tid, NULL, thread_handler, &targ);
                    DEBUG(ret, !=, 0, "pthread_create error ", -1);

                    ret = pthread_detach(tid);
                    DEBUG(ret, !=, 0, "pthread_detach error ", -1);
                }  
            }  
        }
    }  
    close(sfd);  
}  

void epoll_tpc(int sfd, nfds_t psize, vpfun msg_handler)
{
    int ret, efd, cfd, nfd, i, max_event;
    socklen_t len;
    pthread_t tid;
    struct epoll_event ev;
    struct epoll_event ea[psize];
    struct thread_arg targ;
    struct sockaddr_in caddr;

    len = sizeof(caddr);
    bzero(&ev.data, sizeof(ev.data));//Init or valgrind errors appears on funciton epoll_ctl()  
    ev.events = EPOLLIN;
    ev.data.fd = sfd;

    efd = epoll_create(1);
    DEBUG(efd, ==, -1, "epoll_create error ", -1);

    ret = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &ev);
    DEBUG(ret, ==, -1, "epoll_ctl error ", -1);

    max_event = 1;
    while(true)
    {
        nfd = epoll_wait(efd, ea, max_event, -1); 
        DEBUG(nfd, ==, -1, "epoll_wait error ", -1);

        for(i=0; i<nfd; i++)
        {
           if(sfd == ea[i].data.fd)  
           {
                cfd = accept(sfd, NULL, NULL); 
                DEBUG(cfd, ==, -1, "accept error ", -1);

                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
                DEBUG(ret, ==, -1, "epoll_ctl error ", -1);

                max_event++;
           }
           else
           {
               pthread_mutex_lock(&mutex);

               cfd = ea[i].data.fd;
               ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);
               DEBUG(ret, ==, -1, "getpeername error ", -1);

               bzero(&targ, sizeof(targ));
               targ.msg_handler = msg_handler;
               targ.caddr = caddr;
               targ.cfd = cfd;

               ret = epoll_ctl(efd, EPOLL_CTL_DEL, cfd, &ev); 
               DEBUG(ret, ==, -1, "epoll_ctl error ", -1);

               max_event--;

               ret = pthread_create(&tid, NULL, thread_handler, &targ);
               DEBUG(ret, !=, 0, "pthread_create error ", -1);

               ret = pthread_detach(tid);
               DEBUG(ret, !=, 0, "pthread_detach error ", -1);
           }
        }
    }
    close(sfd);
}
