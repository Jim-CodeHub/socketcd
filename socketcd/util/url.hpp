/**-----------------------------------------------------------------------------------------------------------------
 * @file	url.hpp
 * @brief	URL handler 
 *
 * Copyright (c) 2020-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SOCKETCD_URL___
#define __SOCKETCD_URL__


/*-----------------------------------------------------------------------------------------------------------------
 *											SOCKETCD/URL INCLUDES 
 *------------------------------------------------------------------------------------------------------------------
*/

#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <list>


using namespace std;

namespace NS_SOCKETCD{


/*-----------------------------------------------------------------------------------------------------------------
 *											SOCKETCD/URL  MACRO
 *------------------------------------------------------------------------------------------------------------------
*/


/*-----------------------------------------------------------------------------------------------------------------
 *											SOCKETCD/URL DATA BLOCK
 *-----------------------------------------------------------------------------------------------------------------
*/

class URL_Parser{
	public:
		URL_Parser( const char *URL		);

		string			getHostName(void);
		const char *	getOfclName(void);
		int				getAddrType(void);
		list<string>	getAliaList(void);
		list<string>	getAddrList(void);

		~URL_Parser(){ delete [] ipvx;	};

	private:
		string			HOST;
		char		   *ipvx = new char [INET6_ADDRSTRLEN];
		struct hostent *hptr = NULL;
};


} /*< NS_SOCKETCD */


#endif /**< __SOCKETCD_URL__ */

