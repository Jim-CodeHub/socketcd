#include <iostream>
#include <socketcd/socketcd.hpp>

using namespace std;
using namespace NS_SOCKETCD;


int main(void)
{
	class URL_Parser URL("https://www.baidu.com/ABC");

	string			Hostname = URL.getHostName();
	int				AddrType = URL.getAddrType();

	list<string>	AliaList = URL.getAliaList();
	list<string>	AddrList = URL.getAddrList();


	cout << "Host Name : " << Hostname << endl;
	cout << "Addr Type : " << ((AddrType == AF_INET)?"AF_INET":"AF_INET6") << endl;

	for ( list<string>::iterator _big = AliaList.begin(), _end = AliaList.end(); _big != _end; _big++ )
	{
		cout << "Alias is : " << *_big << endl;
	}

	for ( list<string>::iterator _big = AddrList.begin(), _end = AddrList.end(); _big != _end; _big++ )
	{
		cout << "Addrs is : " << *_big << endl;
	}

    return 0;
}

