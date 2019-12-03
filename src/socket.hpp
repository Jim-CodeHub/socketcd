
/**-----------------------------------------------------------------------------------------------------------------
 * @file	socket.hpp
 * @brief	standard macro and enumeration of TCP/IP stack with POSIX.1 (1003.1g) 
 *
 * Copyright (c) 2019-2019 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SOCKET_H__
#define __SOCKET_H__


namespace NS_SOCKETCD{


/*------------------------------------------------------------------------------------------------------------------
 *
 *											SOCKETCD INCLUDES
 *
 *------------------------------------------------------------------------------------------------------------------
*/


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *									   SOCKETCD MACRO FOR TCP/IP PORT 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  SOCKETCD_PORT_ECHO							7
#define  SOCKETCD_PORT_FTP_DATA						20
#define	 SOCKETCD_PORT_FTP_CONTROL					21
#define  SOCKETCD_PORT_SSH							22	
#define	 SOCKETCD_PORT_TELNET						23
#define	 SOCKETCD_PORT_SMTP							25
#define  SOCKETCD_PORT_HTTP							80
#define	 SOCKETCD_PORT_SFTP							117
#define	 SOCKETCD_PORT_HTTPS						443


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *									    SOCKETCD MACRO UNDER POSIX.1
 *
 *-----------------------------------------------------------------------------------------------------------------
*/
																			/*----------- Socket param ----------*/
#define  SOCKETCD_DOMAIN_POSIX1_AF_INET					AF_INET
#define  SOCKETCD_DOMAIN_POSIX1_AF_INET6				AF_INET6
#define  SOCKETCD_DOMAIN_POSIX1_AF_UNIX					AF_UNIX
#define  SOCKETCD_DOMAIN_POSIX1_AF_UPSPEC				AF_UPSPEC

#define  SOCKETCD_TYPE_POSIX1_SOCK_DGRAM				SOCK_DGRAM
#define  SOCKETCD_TYPE_POSIX1_SOCK_RAW					SOCK_RAW
#define  SOCKETCD_TYPE_POSIX1_SOCK_SEQPACKET			SOCK_SEQPACKET
#define  SOCKETCD_TYPE_POSIX1_SOCK_STREAM				SOCK_STREAM

#define  SOCKETCD_PROTOCOL_POSIX1_IPPROTO_IP			IPPROTO_IP
#define  SOCKETCD_PROTOCOL_POSIX1_IPPROTO_IPV6			IPPROTO_IPV6
#define  SOCKETCD_PROTOCOL_POSIX1_IPPROTO_ICMP			IPPROTO_ICMP
#define  SOCKETCD_PROTOCOL_POSIX1_IPPROTO_RAW			IPPROTO_RAW
#define  SOCKETCD_PROTOCOL_POSIX1_IPPROTO_TCP			IPPROTO_TCP
#define  SOCKETCD_PROTOCOL_POSIX1_IPPROTO_UDP			IPPROTO_UDP

#define	 SOCKETCD_LEVEL_SOL_SOCKET						SOL_SOCKET 

																			/*------ Socket general options ------*/
#define  SOCKETCD_OPT_SO_ACCEPTCONN						SO_ACCEPTCONN		/* Only getsocketopt				  */
#define  SOCKETCD_OPT_SO_BROADCAST						SO_BROADCAST
#define	 SOCKETCD_OPT_SO_DEBUG						    SO_DEBUG
#define	 SOCKETCD_OPT_SO_DONTROUTE						SO_DONTROUTE
#define	 SOCKETCD_OPT_SO_ERROR						    SO_ERROR			/* Only getsocketopt				  */
#define	 SOCKETCD_OPT_SO_KEEPALIVE						SO_KEEPALIVE
#define	 SOCKETCD_OPT_SO_LINGER							SO_LINGER
#define	 SOCKETCD_OPT_SO_OOBINLINE						SO_OOBINLINE
#define  SOCKETCD_OPT_SO_RCVBUF							SO_RCVBUF
#define	 SOCKETCD_OPT_SO_RCVLOWAT						SO_RCVLOWAT
#define	 SOCKETCD_OPT_SO_REUSEADDR					    SO_REUSEADDR
#define	 SOCKETCD_OPT_SO_SNDBUF							SO_SNDBUF
#define	 SOCKETCD_OPT_SO_SNDLOWAT						SO_SNDLOWAT
#define	 SOCKETCD_OPT_SO_SNDTIMEO						SO_SNDTIMO
#define	 SOCKETCD_OPT_SO_TYPE							SO_TYPE				/* Only getsocketopt				  */

																			/*------ Socket message flags --------*/
#define  SOCKETCD_RECV_MSG_OOB							MSG_OOB
#define  SOCKETCD_RECV_MSG_PEEK							MSG_PEEK
#define  SOCKETCD_RECV_MSG_WAITALL						MSG_WAITALL			/* Only for SOCK_STREAM				  */

#define  SOCKETCD_SEND_MSG_EOR							MSG_EOR
#define  SOCKETCD_SEND_MSG_NOSIGNAL						MSG_NOSIGNAL
#define	 SOCKETCD_SEND_MSG_OOB							MSG_OOB


/*-----------------------------------------------------------------------------------------------------------------
 * 
 *										   SOCKETCD DATA BLOCK
 *
 *-----------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief TCP/IP stack protocols
 **/
enum TCP_IP_STACK{
	/*< Transport layer protocol */
	TCPv4, TCPv6, UDPv4, UDPv6, DCCPv4, DCCPv6, SCTPv4, SCTPv6, RSVPv4, RSVPv6,

	/*< Internet  layer protocol */
	IPv4, IPv6, ICMP, ICMPv6, IGMPv4, IGMPv6, 
};


} /*< NS_SOCKETCD */


#endif /*__SOCKET_H__*/

