# server-side/client-side TCP/IP stack library with POSIX 1003.1g standard socket API and language C++

## Install

./configure
make
make install

## Note
* Client behavior is implementation dependent in function msg_cgi(). 

* Administrator privileges must be used 

## Demo
    //main.c

    #include "libsocket.h"

    #define PORT 808 
    #define ADDR "127.0.0.1" 

    void msg_handler(int cfd, const struct sockaddr_in *caddr)
    {
        char buff[1023];
        recv(cfd, buff, sizeof(buff), 0);
        send(cfd, "back", 5, 0);
    }

    int main(void)
    {
		socketd_tcp_v4 TCP;

		TCP.server_init("127.0.0.1", 80);
		TCP.server_emit(PPC);

        /*SHOULD NOT BE ANY CODES BELOW!*/

        return 0;
    }

    //command line
    #gcc main.c -lsocket -lpthread

## Performence
	TBD

