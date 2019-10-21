/**-----------------------------------------------------------------------------------------------------------------
 * @file	socketd.c
 * @brief	Server-side TCP/IP stack library with POSIX 1003.1g standard socket API
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/
#include "socketd.hpp" 


using namespace NS_LIBSOCKET;

/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS PROTOTYPES
*
--------------------------------------------------------------------------------------------------------------------
*/
pthread_mutex_t socketd_tcp_v4::mutex = PTHREAD_MUTEX_INITIALIZER;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief	    Create TCP/IP socket 
 *	@param[in]  _P	- TCP/IP stack protocol 
 *	@param[out] None
 *	@return		None
 **/
socketd_server::socketd_server(enum TCP_IP_STACK _P)
{
	int domain, type, protocol;

	switch (_P)
	{
		case TCPv4:
			domain   = LIBSOCKET_DOMAIN_POSIX1_AF_INET			;
			type     = LIBSOCKET_TYPE_POSIX1_SOCK_STREAM		; 
			protocol = LIBSOCKET_PROTOCOL_POSIX1_IPPROTO_TCP	; 
			break;
		case TCPv6:
			domain   = LIBSOCKET_DOMAIN_POSIX1_AF_INET6			;
			type     = LIBSOCKET_TYPE_POSIX1_SOCK_STREAM		; 
			protocol = LIBSOCKET_PROTOCOL_POSIX1_IPPROTO_TCP	; 
			break;
		case UDPv4:
			domain   = LIBSOCKET_DOMAIN_POSIX1_AF_INET			;
			type     = LIBSOCKET_TYPE_POSIX1_SOCK_DGRAM			; 
			protocol = LIBSOCKET_PROTOCOL_POSIX1_IPPROTO_UDP	; 
			break;
		case UDPv6:
			domain   = LIBSOCKET_DOMAIN_POSIX1_AF_INET6			;
			type     = LIBSOCKET_TYPE_POSIX1_SOCK_DGRAM			; 
			protocol = LIBSOCKET_PROTOCOL_POSIX1_IPPROTO_UDP	; 
			break;
		default: ;
	}

	socketfd = socket(domain, type, protocol);

	if (-1 == socketfd) {throw "Socket create failure";}
}

/**
 *	@brief	    Set TCP/IP socket options
 *	@param[in]  level	-	LIBSOCKET_LEVEL_XXX 
 *	@param[in]  option  -	LIBSOCKET_OPT_XXX
 *	@param[in]  _switch -	true/false
 *	@param[out] None
 *	@return		None
 *	@note		The function suitable the option which value is bool 
 **/
void socketd_server::set_socket_opt(int level, int option, bool _switch)
{
	int ret = 0, optval = _switch;

	ret = setsockopt(socketfd, level, option, &optval, sizeof(optval));

	if (-1 == ret) {throw "Socket option set failure";}

	return;
}

/**
 *	@brief	    Set TCP/IP socket options
 *	@param[in]  level	-	LIBSOCKET_LEVEL_XXX 
 *	@param[in]  option  -	LIBSOCKET_OPT_XXX
 *	@param[in]  optval	-   option value
 *	@param[in]  optlen	-   option value length	
 *	@param[out] None
 *	@return		None
 *	@note		The function suitable the option which value is structure 
 **/
void socketd_server::set_socket_opt(int level, int option, void *optval, socklen_t optlen)
{
	int ret = 0;

	ret = setsockopt(socketfd, level, option, optval, optlen);

	if (-1 == ret) {throw "Socket option set failure";}

	return;
}

/**
 *	@brief	    Set TCP/IP socket deamon 
 *	@param[in]  message - true/false 
 *	@param[out] None
 *	@return		None
 *	@note	    If param message is false, exception message will still work on currunt terminal	
 *				, which is helpful for debugging
 **/
void socketd_server::set_deamon(bool message)
{
	int ret = 0;

	ret = daemon(0, message);

	if (-1 == ret) {throw "Socket deamon set failure";}

	return;
}

/*
--------------------------------------------------------------------------------------------------------------------
*			                                   TCP/IP IMPLEMENT
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief	    Initial socket server 
 *	@param[in]  ip 
 *	@param[in]  port	- Application layer protocol port 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::server_init(const char *ip, in_port_t port, CGI_T msg_cgi)
{
	int ret = 0, opt = 1;

    ret = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    saddr.sin_family	  = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port		  = htons(port);
    bzero(saddr.sin_zero, sizeof(saddr.sin_zero));

    ret = bind(socketfd, (struct sockaddr*)&saddr, sizeof(saddr)); 

	if (-1 == ret) {throw "Socket server init failure";}

	this->msg_cgi = msg_cgi;

	return;
}

/**
 *	@brief	    Initial socket server 
 *	@param[in]  method	-  BLOCK/PPC/TPC/SELECT_TPC/POLL_TPC/EPOLL_TPC 
 *	@param[in]  backlog	- Size of listen queue 
 *	@param[in]  nfds	- Number of poll/epoll structure 
 *	@param[out] None
 *	@return		None
 *	@note		Param nfds onley works when using method POLL/EPOLL 
 **/
void socketd_tcp_v4::server_emit(enum method m, int backlog, nfds_t nfds)
{
	int ret = 0;

	ret = listen(socketfd, backlog); 

	if (-1 == ret) {throw "Socket server emit failure";}

	this->nfds = nfds; /**< Only for xPOLL */

	switch(m)
	{
		case PPC	   : ppc();		   break;
		case TPC	   : tpc();		   break;
		case SELECT_TPC: select_tpc(); break;
		case POLL_TPC  : poll_tpc();   break;
		case EPOLL_TPC : epoll_tpc();  break;
		default		   : block(); 
	}

	return;
}

/**
 *	@brief	    Private function for TCP/IP server blocking method 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::block(void)
{
    int				   ret = 0;
	int				   cfd;
    socklen_t		   len;
    struct sockaddr_in caddr;

    len = sizeof(caddr);

    bzero(&caddr, len);
    cfd = accept(socketfd, (struct sockaddr*)&caddr, &len);

	if (-1 == cfd) {throw "Socket server accept failure";}

    msg_cgi(cfd, &caddr);

    close(cfd);

	return;
}

/**
 *	@brief	    Private function for TCP/IP server PPC method 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::ppc(void)
{
    int				   ret = 0;
	int				   cfd;
    socklen_t		   len;
    struct sockaddr_in caddr;

    len = sizeof(caddr);

    while(true)
    {
        bzero(&caddr, len);
        cfd = accept(socketfd, (struct sockaddr*)&caddr, &len);

		if (-1 == cfd) {throw "Socket server accept failure";}

        signal(SIGCHLD, SIG_IGN);

        if(fork() == 0) /**< Child process */
        {
            close(socketfd);

            msg_cgi(cfd, &caddr);

            close(cfd);
            raise(SIGKILL);
        }

        close(cfd);
    }

	return;
}

/**
 *	@brief	    Private function for TCP/IP server TPC method 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::tpc(void)
{
    int				   ret = 0;
	int				   cfd;
    socklen_t		   len;
    pthread_t		   tid;
    struct thread_args targs;
    struct sockaddr_in caddr;

    len = sizeof(caddr);

    while(true)
    {
        bzero(&caddr, len);
        cfd = accept(socketfd, (struct sockaddr *)&caddr, &len);

		if (-1 == cfd) {throw "Socket server accept failure";}

        pthread_mutex_lock(&socketd_tcp_v4::mutex);

		targs.msg_cgi	 = msg_cgi;
        targs.cfd		 = cfd;
        targs.caddr		 = caddr;

        ret = pthread_create(&tid, NULL, thread_hook, &targs);

		if (-1 == ret) {throw "Socket server pthread create failure";}

        ret = pthread_detach(tid);

		if (-1 == ret) {throw "Socket server pthread detach failure";}
    }

	return;
}

/**
 *	@brief	    Private function for TCP/IP server SELECT_TPC method 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::select_tpc(void)
{
    int				   ret = 0;
	int				   cfd;
	int				   maxfd;
    int                bakfd[FD_SETSIZE]; 
    fd_set			   tmp_set;
	fd_set			   all_set;
    socklen_t		   len;
    pthread_t		   tid;
    struct thread_args targs;
    struct sockaddr_in caddr;

    maxfd = socketfd;
    len = sizeof(caddr);

    memset  (bakfd, -1, sizeof(int)*FD_SETSIZE);
    FD_ZERO (&tmp_set);
    FD_ZERO (&all_set);
    FD_SET  (socketfd, &all_set);

    while(true)
    {
        tmp_set = all_set;

        ret = select(maxfd + 1, &tmp_set, NULL, NULL, NULL);

		if (-1 == ret) {throw "Socket server select failure";}

        if(FD_ISSET(socketfd, &tmp_set))
        {
            bzero(&caddr, len);
            cfd = accept(socketfd, NULL, NULL);

			if (-1 == cfd) {throw "Socket server accept failure";}

            FD_SET(cfd, &all_set);

            for(int i = 0; i < FD_SETSIZE; i++)
            {
                if(-1 == bakfd[i])
                {
                    bakfd[i] = cfd;
                    break;
                }
            }

            for(int i = 0; i < FD_SETSIZE; i++)
            {
                if(bakfd[i] > maxfd) 
                {
                    maxfd = bakfd[i]; 
                    break;
                }
            }
        }
        else /**< "else" can be ignored, the server performences depends on clients */
        {
            for(int i = 0; i < FD_SETSIZE; i++)
            {
                if(-1 == bakfd[i]) {continue;}

                if(FD_ISSET(bakfd[i], &tmp_set))
                {
                    FD_CLR(bakfd[i], &all_set);

                    pthread_mutex_lock(&socketd_tcp_v4::mutex);

                    ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);

					if (-1 == ret) {throw "Socket server getpeername failure";}

                    targs.msg_cgi = msg_cgi;
                    targs.caddr	  = caddr;
                    targs.cfd	  = bakfd[i];

                    bakfd[i] = -1;

                    ret = pthread_create(&tid, NULL, thread_hook, &targs);

					if (-1 == ret) {throw "Socket server pthread create failure";}

                    ret = pthread_detach(tid);

					if (-1 == ret) {throw "Socket server pthread detach failure";}
                }
            }
        }
    }

    close(socketfd);

	return;
}

/**
 *	@brief	    Private function for TCP/IP server POLL_TPC method 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::poll_tpc(void)
{
    int				   ret = 0;
	int				   cfd;
    socklen_t		   len;
    pthread_t		   tid;
    nfds_t		       maxnfd = 0;
    nfds_t			   countfd = 0;
    struct pollfd	   pfd[nfds];
    struct thread_args targs;
    struct sockaddr_in caddr;

    memset(pfd, -1, sizeof(struct pollfd)*nfds);
    pfd[0].fd = socketfd;
    pfd[0].events = POLLIN;

    len = sizeof(caddr);

    while(true)
    {
        ret = poll(pfd, maxnfd+1, -1);

		if (-1 == ret) {throw "Socket server poll failure";}

        if(pfd[0].revents & POLLIN)
        {
            bzero(&caddr, len);
            cfd = accept(socketfd, NULL, NULL); 

			if (-1 == cfd) {throw "Socket server accept failure";}

            for(int i = 1; i < nfds; i++)
            {
                if(-1 == pfd[i].fd) 
                {
                    pfd[i].fd = cfd; 
                    pfd[i].events = POLLIN;
                    countfd++;

                    break;
                }
            }

            if(countfd > maxnfd) {maxnfd = countfd;}
        }
        else /**< "else" can be ignored, the server performences depends on clients */
        {
            for(int i = 1; i < nfds; i++)
            {  
                if(-1 == pfd[i].fd) {continue;}

                if(pfd[i].revents & POLLIN)
                {  
                    pthread_mutex_lock(&socketd_tcp_v4::mutex);

                    ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);

					if (-1 == ret) {throw "Socket server getpeername failure";}

                    targs.msg_cgi = msg_cgi;
                    targs.caddr	  = caddr;
                    targs.cfd	  = pfd[i].fd;

                    pfd[i].fd = -1;
                    countfd--;

                    ret = pthread_create(&tid, NULL, thread_hook, &targs);

					if (-1 == ret) {throw "Socket server pthread create failure";}

                    ret = pthread_detach(tid);

					if (-1 == ret) {throw "Socket server pthread detach failure";}
                }  
            }
        }
    }

    close(socketfd);  

	return;
}

/**
 *	@brief	    Private function for TCP/IP server EPOLL_TPC method 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::epoll_tpc(void)
{
    int				   ret = 0;
	int				   efd;
	int				   cfd;
	int				   nfd;
	int				   max_event;
    socklen_t		   len;
    pthread_t		   tid;
    struct epoll_event ev;
    struct epoll_event ea[nfds];
    struct thread_args targs;
    struct sockaddr_in caddr;

    len = sizeof(caddr);
    bzero(&ev.data, sizeof(ev.data)); /**< Init or valgrind errors appears on funciton epoll_ctl() */
    ev.events = EPOLLIN;
    ev.data.fd = socketfd;

    efd = epoll_create(1);

	if (-1 == ret) {throw "Socket server epoll create failure";}

    ret = epoll_ctl(efd, EPOLL_CTL_ADD, socketfd, &ev);

	if (-1 == ret) {throw "Socket server epoll ctl failure";}

    max_event = 1;
    while(true)
    {
        nfd = epoll_wait(efd, ea, max_event, -1); 

		if (-1 == nfd) {throw "Socket server epoll wait failure";}

        for(int i = 0; i < nfd; i++)
        {
           if(socketfd == ea[i].data.fd)  
           {
                cfd = accept(socketfd, NULL, NULL); 

				if (-1 == cfd) {throw "Socket server accept failure";}

                ev.events = EPOLLIN;
                ev.data.fd = cfd;

                ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);

			   if (-1 == ret) {throw "Socket server epoll ctl failure";}

                max_event++;
           }
           else
           {
               pthread_mutex_lock(&socketd_tcp_v4::mutex);

               cfd = ea[i].data.fd;
               ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);

			   if (-1 == ret) {throw "Socket server getpeername failure";}

			   targs.msg_cgi = msg_cgi;
			   targs.caddr	 = caddr;
               targs.cfd	 = cfd;

               ret = epoll_ctl(efd, EPOLL_CTL_DEL, cfd, &ev); 

			   if (-1 == ret) {throw "Socket server epoll ctl failure";}

               max_event--;

			   ret = pthread_create(&tid, NULL, thread_hook, &targs);

			   if (-1 == ret) {throw "Socket server pthread create failure";}

			   ret = pthread_detach(tid);

			   if (-1 == ret) {throw "Socket server pthread detach failure";}
           }
        }
    }

    close(socketfd);

	return;
}

/**
 *	@brief	    Thread hook function for TCP/IP server TPCs method 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void *socketd_tcp_v4::thread_hook(void *arg)
{
    struct thread_args targs = *((struct thread_args *)arg);  

    pthread_mutex_unlock(&mutex);

	targs.msg_cgi(targs.cfd, &(targs.caddr));

    close(targs.cfd);

    pthread_exit(NULL);
}

