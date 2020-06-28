#include <iostream>
#include <socketcd/socketcd.hpp>

using namespace std;
using namespace NS_SOCKETCD;

int main(void)
{
	try{
		socketc_tcp_v4 TCP;

		int cErr = TCP.client_init("127.0.0.1", 80);

		switch ( cErr ) /**< Client connect error */
		{
			case ENETUNREACH : 
				cout << "Network is unreachable" << endl;
				exit(-1);
				break;
			case ETIMEDOUT	 :
				cout << "Timeout while attempting connection" << endl;
				exit(-1);
				break;
			case ECONNREFUSED:
				cout << "Connect refused" << endl;
				exit(-1);
				break;

				/**< MAY BE SOME HANDLER FOR UPON ERRORS, INSTEAD OF 'exit()'. */

			default: 
				cout << "sockect bind error" <<endl;
				exit(-1);
		}

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
