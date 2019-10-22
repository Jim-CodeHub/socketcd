/**-----------------------------------------------------------------------------------------------------------------
 * @file	socketc.hpp
 * @brief	Client-side TCP/IP stack library with POSIX 1003.1g standard socket API
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SOCKETC_H__
#define __SOCKETC_H__


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *											SOCKETC INCLUDES 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

#include "socket.hpp"


namespace NS_LIBSOCKET{


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *											 SOCKETC MACRO
 *
 *------------------------------------------------------------------------------------------------------------------
*/


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *										   SOCKETC DATA BLOCK
 *
 *-----------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief Socket client foundational class 
 **/
class socketc_client{
	public:
		socketc_client(enum TCP_IP_STACK _P = TCPv4);

		void set_socket_opt(int level, int option, bool _switch);
		void set_socket_opt(int level, int option, void *optval, socklen_t optlen);

		ssize_t data_recv(void *data, size_t len, int flags);	
		ssize_t data_send(void *data, size_t len, int flags);

	protected:
		int socketfd;
};

/**
 *	@brief socket client TCP IPv4 class 
 **/
class socketc_tcp_v4 : public socketc_client{
	public:
		socketc_tcp_v4(void):socketc_client(TCPv4){};

		void client_init(const char *ip, in_port_t port);
		//void client_emit(void);
		//void client_over(void);

	private:
		struct sockaddr_in caddr;
};


} /*< NS_LIBSOCKET */


#endif /*__SOCKETC_H__*/

