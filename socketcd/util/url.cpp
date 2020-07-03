/**-----------------------------------------------------------------------------------------------------------------
 * @file	url.cpp
 * @brief	URL handler 
 *
 * Copyright (c) 2020-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <socketcd/util/url.hpp>

using namespace NS_SOCKETCD;


/*
--------------------------------------------------------------------------------------------------------------------
*			                                  FUNCTIONS PROTOTYPES
--------------------------------------------------------------------------------------------------------------------
*/


/*
--------------------------------------------------------------------------------------------------------------------
*			                                  FUNCTIONS IMPLEMENT
--------------------------------------------------------------------------------------------------------------------
*/

/**
 *	@brief	    Parse URL 
 *	@param[in]  URL 
 *	@param[out] None
 *	@return		None
 *	@note		exception : const char * 'gethostbyname' error 
 **/
URL_Parser::URL_Parser( const char *URL )
{
	string url = URL;

	/**< eg : https://www.dumor.cn:80/index.aspx */

	string::size_type pos_start = url.find("://");

	if ( pos_start != string::npos ) { url = url.substr( pos_start + 3); }


	/**< eg : www.dumor.cn:80/index.aspx		 */

	string::size_type pos_slash = url.find( "/" );

	url = url.substr( 0, pos_slash );


	/**< eg : www.dumor.cn:80					 */

	string::size_type pos_ports = url.find( ":" );

	this->PORT = (pos_ports != string::npos)?(atoi(url.substr(pos_ports + 1).c_str())):-1;
	this->HOST = url.substr( 0, pos_ports ); 


	hptr = gethostbyname( HOST.c_str() );

	if ( NULL == hptr ) { throw( hstrerror(h_errno) ); }
}

/**
 *	@brief	    Get the host name 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		Host name	
 **/
string			URL_Parser::getHostName( void )
{
	return this->HOST;
}

/**
 *	@brief	    Get the port number
 *	@param[in]  None 
 *	@param[out] None
 *	@return		Port number or -1 (port is not exist) 
 **/
int				URL_Parser::getPort	   ( void )
{
	return this->PORT; 
}

/**
 *	@brief	    Get the official host name 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		Official host name	
 **/
const char *	URL_Parser::getOfclName( void )
{
	return hptr->h_name;
}

/**
 *	@brief	    Get the IP version 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		AF_INET/AF_INET6	
 **/
int				URL_Parser::getAddrType( void )
{
	return hptr->h_addrtype;
}

/**
 *	@brief	    Get the Host alias
 *	@param[in]  None 
 *	@param[out] None
 *	@return		Alias list	
 **/
list<string>	URL_Parser::getAliaList( void )
{
	list<string> LS;

	for ( char **pptr = hptr->h_aliases; *pptr != NULL; pptr++ )
	{
		LS.push_back(*pptr);
	}

	return LS;
}

/**
 *	@brief	    Get the Address alias
 *	@param[in]  None 
 *	@param[out] None
 *	@return		Alias list	
 **/
list<string>	URL_Parser::getAddrList(void)
{
	list<string> LS;

	for ( char **pptr = hptr->h_addr_list; *pptr != NULL; pptr++ )
	{
		LS.push_back(inet_ntop(hptr->h_addrtype,*pptr, ipvx, INET6_ADDRSTRLEN));
	}

	return LS;
}

