/**-----------------------------------------------------------------------------------------------------------------
 * @file	socketd.h;
 * @brief	Server-side TCP/IP stack library with POSIX 1003.1g standard socket API
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SOCKETD_H__
#define __SOCKETD_H__


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *											SOCKETD INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include "../socket.h"


namespace NS_LIBSOCKET{


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *											 SOCKETD MACRO
 *
 *------------------------------------------------------------------------------------------------------------------
*/



/*-----------------------------------------------------------------------------------------------------------------
 * 
 *										   SOCKETD DATA BLOCK
 *
 *-----------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief socket server implement method 
 **/
enum method{
	BLOCK, PPC, TPC, SELECT_TPC, POLL_TPC, EPOLL_TPC	
}; 

/**
 *	@brief socket server base class 
 **/
class socketd_server{
	public:
		socketd_server(enum TCP_IP_STACK _P = TCPv4);

		void set_socket_opt(int level, int option, bool _switch);
		void set_socket_opt(int level, int option, void *optval, socklen_t optlen);

		void set_deamon(bool message);

	protected:
		int socketfd;
};

/**
 *	@brief socket server TCP IPv4 class 
 **/
class socketd_tcp_v4 : public socketd_server{
	public:
		socketd_tcp_v4(void):socketd_server(TCPv4){};

		void server_init(enum method m, const char *ip, in_port_t port, int backlog=128);
		void server_emit();
		void server_over();

	private:
		struct sockaddr_in saddr;
		int backlog;

		enum method m;

		nfds_t psize;

		void (*msg_cgi)(int cfd, const struct sockaddr_in *caddr);
};


void *thread_handler(void *arg);


struct thread_arg
{
    int cfd;
    struct sockaddr_in caddr;
    vpfun msg_handler; 
};

/*-----------------------------------------------------------------------------------------------------------------
 * 
 *				    				       SOCKETD FUNCTIONS DECLARATION
 *
 *------------------------------------------------------------------------------------------------------------------
*/
struct param_s xsocketd_init(enum xtype type, in_port_t port, const char *ipaddr, int backlog,  enum xway way, nfds_t psize, bool daemon);
void param_juge(in_port_t port, const char *ipaddr, int backlog, enum xway way, nfds_t npfd);
void xsocketd_start(const struct param_s *param, vpfun msg_handler);

void none(int sfd, vpfun msg_handler);                      //single
void ppc(int sfd, vpfun msg_handler);                       //multi process 
void tpc(int sfd, vpfun msg_handler);                       //multi thread
void select_tpc(int sfd, vpfun msg_handler);                //select with multi thread 
void poll_tpc(int sfd, nfds_t psize, vpfun msg_handler);    //poll with multi thread 
void epoll_tpc(int sfd, nfds_t psize, vpfun msg_handler);   //epoll with multi thread


} /*< NS_LIBSOCKET */


#endif /*__SOCKETD_H__*/

