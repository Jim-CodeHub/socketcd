/**-----------------------------------------------------------------------------------------------------------------
 * @file	socketd.hpp
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

#include "socket.hpp"


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

typedef void (*CGI_T)(int cfd, const struct sockaddr_in *caddr);

/**
 *	@brief Socket server implement method 
 **/
enum method{
	BLOCK, PPC, TPC, SELECT_TPC, POLL_TPC, EPOLL_TPC	
}; 

/**
 *	@brief Socket server arguments of TPCs
 **/
struct thread_args{
	void (*msg_cgi)(int cfd, const struct sockaddr_in *caddr);
    int cfd;
    struct sockaddr_in caddr;
};

/**
 *	@brief Socket server foundational class 
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

		void server_init(const char *ip, in_port_t port, CGI_T msg_cgi  );
		void server_emit(enum method m, int backlog=128, nfds_t nfds=128);
		//void server_over(void);

		static pthread_mutex_t mutex;
		static void *thread_hook(void *arg);

	private:
		struct sockaddr_in saddr;
		nfds_t			   nfds;
		enum method		   m;
		CGI_T			   msg_cgi;

		void block		(void); /**< Blocking TCP/IP socket server				   */
		void ppc		(void); /**< Multi process TCP/IP socket server			   */
		void tpc		(void); /**< Multi thread TCP/IP socket server			   */
		void select_tpc (void); /**< Select with multi thread TCP/IP socket server */
		void poll_tpc   (void); /**< Poll with multi thread TCP/IP socket server   */
		void epoll_tpc	(void); /**< Epoll with multi thread TCP/IP socket server  */
};


} /*< NS_LIBSOCKET */


#endif /*__SOCKETD_H__*/

