/**-----------------------------------------------------------------------------------------------------------------
 * @file	socketd.cpp
 * @brief	Server-side TCP/IP stack library with POSIX 1003.1g standard socket API
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <socketcd/server/socketd.hpp>


using namespace NS_SOCKETCD;

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
			domain   = SOCKETCD_DOMAIN_POSIX1_AF_INET			;
			type     = SOCKETCD_TYPE_POSIX1_SOCK_STREAM		; 
			protocol = SOCKETCD_PROTOCOL_POSIX1_IPPROTO_TCP	; 
			break;
		case TCPv6:
			domain   = SOCKETCD_DOMAIN_POSIX1_AF_INET6			;
			type     = SOCKETCD_TYPE_POSIX1_SOCK_STREAM		; 
			protocol = SOCKETCD_PROTOCOL_POSIX1_IPPROTO_TCP	; 
			break;
		case UDPv4:
			domain   = SOCKETCD_DOMAIN_POSIX1_AF_INET			;
			type     = SOCKETCD_TYPE_POSIX1_SOCK_DGRAM			; 
			protocol = SOCKETCD_PROTOCOL_POSIX1_IPPROTO_UDP	; 
			break;
		case UDPv6:
			domain   = SOCKETCD_DOMAIN_POSIX1_AF_INET6			;
			type     = SOCKETCD_TYPE_POSIX1_SOCK_DGRAM			; 
			protocol = SOCKETCD_PROTOCOL_POSIX1_IPPROTO_UDP	; 
			break;
		default: ;
	}

	socketfd = socket(domain, type, protocol);

	if (-1 == socketfd){perror("Socket create failure");exit(-1);}
}

/**
 *	@brief	    Set TCP/IP socket options
 *	@param[in]  level	-	SOCKETCD_LEVEL_XXX 
 *	@param[in]  option  -	SOCKETCD_OPT_XXX
 *	@param[in]  _switch -	true/false
 *	@param[out] None
 *	@return		None
 *	@note		The function suitable the option which value is bool 
 **/
void socketd_server::set_socket_opt(int level, int option, bool _switch)
{
	int ret = 0, optval = _switch;

	ret = setsockopt(socketfd, level, option, &optval, sizeof(optval));

	if (-1 == ret) {perror("Socket option set failure"); exit(-1);}

	return;
}

/**
 *	@brief	    Set TCP/IP socket options
 *	@param[in]  level	-	SOCKETCD_LEVEL_XXX 
 *	@param[in]  option  -	SOCKETCD_OPT_XXX
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

	if (-1 == ret) {perror("Socket option set failure"); exit(-1);}

	return;
}

/**
 *	@brief	    Get TCP/IP socket options
 *	@param[in]  level	-	SOCKETCD_LEVEL_XXX 
 *	@param[in]  option  -	SOCKETCD_OPT_XXX
 *	@param[in]  optval	-   option value
 *	@param[in]  optlen	-   option value length	
 *	@param[out] None
 *	@return		None
 *	@note		The function suitable the option which value is structure 
 **/
void socketd_server::get_socket_opt(int level, int optname, void *optval, socklen_t *optlen)
{
	int ret = 0;

	ret = getsockopt(socketfd, level, optname, optval, optlen);

	if (-1 == ret) {perror("Socket option get failure"); exit(-1);}

	return;
}

/**
 *	@brief	    Set TCP/IP socket daemon 
 *	@param[in]  message - true/false 
 *	@param[out] None
 *	@return		None
 *	@note	    If param message is true, exception message will still work on currunt terminal	
 *				, which is helpful for debugging
 **/
void socketd_server::set_daemon(bool message)
{
	int ret = 0;

	ret = daemon(0, message);

	if (-1 == ret) {perror("Socket daemon set failure"); exit(-1);}

	return;
}

/**
 *	@brief	    Recive data from client socket
 *	@param[in]  socketfd - client socket file descriptor 
 *	@param[in]  len	     - data buffer length 
 *	@param[in]  flags    - SOCKETCD_RECV_MSG_XXX or 0 
 *	@param[out] None 
 *	@return		Bytes length of data/0 when no data	or peer has been over
 *	@note		1. The function is in blocking mode, and perform a loop style while recive 
 *				2. READ END will be SHUT DOWN after recive 
 **/
ssize_t socketd_server::data_recv(int socketfd, void *buff, size_t len)
{
	ssize_t size = 0, recv_byte = 0;

	char *p = (char *)buff;

	while ((recv_byte = ::recv(socketfd, p, len, 0)) > 0)
	{
		size += recv_byte; p += recv_byte;
	}

	if (-1 == recv_byte) {perror("Data recive error"); exit(-1);}

	::shutdown(socketfd, SHUT_RD);

	return size;
}

/**
 *	@brief	    Recive data from client socket
 *	@param[in]  socketfd - client socket file descriptor 
 *	@param[in]  len		 - data buffer length 
 *	@param[in]  flags	 - SOCKETCD_RECV_MSG_XXX or 0 
 *	@param[out] None 
 *	@return		Bytes length of data/0 when no data	or peer has been over
 **/
ssize_t socketd_server::data_recv(int socketfd, void *buff, size_t len, int flags)
{
	ssize_t size = -1;

	size = ::recv(socketfd, buff, len, flags);

	if (-1 == size) {perror("Data recive error"); exit(-1);}

	return size;
}

/**
 *	@brief	    Send data into client socket
 *	@param[in]  socketfd - client socket file descriptor 
 *	@param[in]  data 
 *	@param[in]  len	   - data length 
 *	@param[in]  flags  - SOCKETCD_SEND_MSG_XXX or 0 
 *	@param[out] None
 *	@return		Bytes length of data
 *	@note		WRITE END will be SHUT DOWN after send
 **/
ssize_t socketd_server::data_send(int socketfd, void *data, size_t len, int flags)
{
	ssize_t size = -1;

	size = send(socketfd, data, len, flags);

	::shutdown(socketfd, SHUT_WR);

	if (-1 == size) {perror("Data send error"); exit(-1);}

	return size;
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
 *	@param[in]  msg_cgi - User's client message handler  
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

	if (-1 == ret) {perror("Socket server init failure");exit(-1);}

	this->msg_cgi = msg_cgi;

	return;
}

/**
 *	@brief	    Start socket server 
 *	@param[in]  method	- BLOCK/PPC/TPC/SELECT_TPC/POLL_TPC/EPOLL_TPC 
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

	if (-1 == ret) {perror("Socket server emit failure"); exit(-1);}

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
	int				   cfd;
    socklen_t		   len;
    struct sockaddr_in caddr;

    len = sizeof(caddr);

    bzero(&caddr, len);
    cfd = accept(socketfd, (struct sockaddr*)&caddr, &len);

	if (-1 == cfd) {perror("Socket server accept failure"); exit(-1);}

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
	int				   cfd;
    socklen_t		   len;
    struct sockaddr_in caddr;

    len = sizeof(caddr);

    while(true)
    {
        bzero(&caddr, len);
        cfd = accept(socketfd, (struct sockaddr*)&caddr, &len);

		if (-1 == cfd) {perror("Socket server accept failure"); exit(-1);}

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
    pthread_t		   tid; /**< Declare sub thread id */
    struct thread_args targs;
    struct sockaddr_in caddr;

    len = sizeof(caddr);

    while(true)
    {
        bzero(&caddr, len);
        cfd = accept(socketfd, (struct sockaddr *)&caddr, &len);

		if (-1 == cfd) {perror("Socket server accept failure" ); exit(-1);}

        pthread_mutex_lock(&socketd_tcp_v4::mutex);

		targs.msg_cgi	 = msg_cgi;
        targs.cfd		 = cfd;
        targs.caddr		 = caddr;

        ret = pthread_create(&tid, NULL, thread_hook, &targs);

		if (-1 == ret) {perror("Socket server pthread create failure"); exit(-1);}

        ret = pthread_detach(tid);

		if (-1 == ret) {perror("Socket server pthread detach failure"); exit(-1);}
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
    pthread_t		   tid; /**< Declare sub thread id */
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

		if (-1 == ret) {perror("Socket server select failure"); exit(-1);}

        if(FD_ISSET(socketfd, &tmp_set))
        {
            bzero(&caddr, len);
            cfd = accept(socketfd, NULL, NULL);

			if (-1 == cfd) {perror("Socket server accept failure"); exit(-1);}

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

					if (-1 == ret) {perror("Socket server getpeername failure"); exit(-1);}

                    targs.msg_cgi = msg_cgi;
                    targs.caddr	  = caddr;
                    targs.cfd	  = bakfd[i];

                    bakfd[i] = -1;

                    ret = pthread_create(&tid, NULL, thread_hook, &targs);

					if (-1 == ret) {perror("Socket server pthread create failure"); exit(-1);}

                    ret = pthread_detach(tid);

					if (-1 == ret) {perror("Socket server pthread detach failure"); exit(-1);}
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
    pthread_t		   tid; /**< Declare sub thread id */
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

		if (-1 == ret) {perror("Socket server poll failure"); exit(-1);}

        if(pfd[0].revents & POLLIN)
        {
            bzero(&caddr, len);
            cfd = accept(socketfd, NULL, NULL); 

			if (-1 == cfd) {perror("Socket server accept failure"); exit(-1);}

            for(nfds_t i = 1; i < nfds; i++)
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
            for(nfds_t i = 1; i < nfds; i++)
            {  
                if(-1 == pfd[i].fd) {continue;}

                if(pfd[i].revents & POLLIN)
                {  
                    pthread_mutex_lock(&socketd_tcp_v4::mutex);

                    ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);

					if (-1 == ret) {perror("Socket server getpeername failure"); exit(-1);}

                    targs.msg_cgi = msg_cgi;
                    targs.caddr	  = caddr;
                    targs.cfd	  = pfd[i].fd;

                    pfd[i].fd = -1;
                    countfd--;

                    ret = pthread_create(&tid, NULL, thread_hook, &targs);

					if (-1 == ret) {perror("Socket server pthread create failure");exit(-1);}

                    ret = pthread_detach(tid);

					if (-1 == ret) {perror("Socket server pthread detach failure");exit(-1);}
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
    pthread_t		   tid; /**< Declare sub thread id */
    struct epoll_event ev;
    struct epoll_event ea[nfds];
    struct thread_args targs;
    struct sockaddr_in caddr;

    len = sizeof(caddr);
    bzero(&ev.data, sizeof(ev.data)); /**< Init or valgrind errors appears on funciton epoll_ctl() */
    ev.events = EPOLLIN;
    ev.data.fd = socketfd;

    efd = epoll_create(1);

	if (-1 == ret) {perror("Socket server epoll create failure"); exit(-1);}

    ret = epoll_ctl(efd, EPOLL_CTL_ADD, socketfd, &ev);

	if (-1 == ret) {perror("Socket server epoll ctl failure"); exit(-1);}

    max_event = 1;
    while(true)
    {
        nfd = epoll_wait(efd, ea, max_event, -1); 

		if (-1 == nfd) {perror("Socket server epoll wait failure"); exit(-1);}

        for(int i = 0; i < nfd; i++)
        {
           if(socketfd == ea[i].data.fd)  
           {
                cfd = accept(socketfd, NULL, NULL); 

				if (-1 == cfd) {perror("Socket server accept failure"); exit(-1);}

                ev.events = EPOLLIN;
                ev.data.fd = cfd;

                ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);

			   if (-1 == ret) {perror("Socket server epoll ctl failure"); exit(-1);}

                max_event++;
           }
           else
           {
               pthread_mutex_lock(&socketd_tcp_v4::mutex);

               cfd = ea[i].data.fd;
               ret = getpeername(cfd, (struct sockaddr *)&caddr, &len);

			   if (-1 == ret) {perror("Socket server getpeername failure"); exit(-1);}

			   targs.msg_cgi = msg_cgi;
			   targs.caddr	 = caddr;
               targs.cfd	 = cfd;

               ret = epoll_ctl(efd, EPOLL_CTL_DEL, cfd, &ev); 

			   if (-1 == ret) {perror("Socket server epoll ctl failure"); exit(-1);}

               max_event--;

			   ret = pthread_create(&tid, NULL, thread_hook, &targs);

			   if (-1 == ret) {perror("Socket server pthread create failure"); exit(-1);}

			   ret = pthread_detach(tid);

			   if (-1 == ret) {perror("Socket server pthread detach failure"); exit(-1);}
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
 *	@note		!!! HEAP SOURCE ARE SHARED BY ALL THREAD, TO RELEASE IT
 *					BEFORE 'msg_cgi()' TERMINATED
 **/
void *socketd_tcp_v4::thread_hook(void *arg)
{
    struct thread_args targs = *((struct thread_args *)arg);  

    pthread_mutex_unlock(&mutex);

	targs.msg_cgi(targs.cfd, &(targs.caddr));

    close(targs.cfd);

    pthread_exit(NULL);
}

/**
 *	@brief	    Close server socket file descriptor 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None
 **/
void socketd_tcp_v4::server_over(void)
{
	close(socketfd);
}

