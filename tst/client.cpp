#include <iostream>
#include <socketcd/src/socketcd.hpp>

using namespace std;
using namespace NS_SOCKETCD;

int main(void)
{
	try{
		socketc_tcp_v4 TCP;

		TCP.client_init("127.0.0.1", 80);

		/* MESSAGE HANDLER BELOW */

		char send[] = "client message : hello server";
		char recv[100];

		TCP.data_send(send,	sizeof(send), 0);
		TCP.data_recv(recv, sizeof(recv), 0);

		cout << recv << endl;
	}
	catch (const char *str)
	{
		cout << str << endl;
	}

	return 0;
}
