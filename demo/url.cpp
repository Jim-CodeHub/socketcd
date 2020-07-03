#include <iostream>
#include <socketcd/socketcd.hpp>

using namespace std;
using namespace NS_SOCKETCD;


int main(void)
{
	string			Protocol;
	int				Port	; 
	string			Hostname;
	int				AddrType;

	list<string>	AliaList;
	list<string>	AddrList;

	try{
		//class URL_Parser URL("http://www.dumor.cn:80/index.aspx");
		class URL_Parser URL("http://www.dumor.cn/index.aspx");

		Protocol = URL.getProtocol();
		Port	 = URL.getPort	  ();
		Hostname = URL.getHostName();
		AddrType = URL.getAddrType();

		AliaList = URL.getAliaList();
		AddrList = URL.getAddrList();
	}
	catch ( const char *err )
	{
		cout << err << endl;
	}

	cout << "Protocol   :" << Protocol									   << endl;
	cout << "Port number:" << Port										   << endl;
	cout << "Host Name  :" << Hostname									   << endl;
	cout << "Addr Type  :" << ((AddrType == AF_INET)?"AF_INET":"AF_INET6") << endl;

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

