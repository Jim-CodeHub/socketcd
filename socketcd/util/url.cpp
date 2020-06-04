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
 **/
URL_Parser::URL_Parser( const char *URL )
{
	string url = URL;

	string::size_type pos = url.find("://");

	if ( pos != string::npos ) { url = url.substr( pos + 3); }

	pos = url.find("/"); /**< Maybe npos. */

	HOST = url.substr( 0, pos );

	hptr = gethostbyname( HOST.c_str() );

	if ( NULL == hptr ) { herror("gethostbyname error : "); exit(-1); }
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

