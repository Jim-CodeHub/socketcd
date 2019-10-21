#include "../src/server/socketd.hpp" 
#include <iostream>

using namespace std;
using namespace NS_LIBSOCKET;

void msg_cgi(int cfd, const struct sockaddr_in *caddr)
{
    int ret;
    char buff[1024];

	cout << "msg_handler work" << endl;

    struct timeval timeout = {0 ,0};    
    setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(struct timeval));

    while(1)
    {
        bzero(buff, sizeof(buff));
        ret = recv(cfd, buff, sizeof(buff), 0);
        if(ret <= 0)
            break;

		cout << buff << endl;
    }
}

int main(void)
{
	socketd_tcp_v4 TCP;

	try
	{
		TCP.server_init("127.0.0.1", 80, msg_cgi);
		TCP.server_emit(PPC);
	}
	catch(const char *str)
	{
		cout << str << endl;
	}

    return 0;
}

