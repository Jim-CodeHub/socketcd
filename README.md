# TCP/IP stack library with language C++ and POSIX 1003.1g standard API

## Install

make CXX=compiler INCLUDE=install dir
make install

## Note
* Client behavior is implementation dependent in function msg_cgi(). 

* Administrator privileges must be used 

## Demo

###Server side

    #include <socketd.hpp>

	using namespace NS_LIBSOCKET;

    void msg_handler(int cfd, const struct sockaddr_in *caddr)
    {
        char buff[1000];

        recv(cfd, buff, sizeof(buff), 0);
        send(cfd, "back", 5, 0);
    }

    int main(void)
    {
		socketd_tcp_v4 TCP;

		TCP.server_init("127.0.0.1", 80);
		TCP.server_emit(PPC);

        /* SHOULD NOT BE ANY CODES BELOW! */

        return 0;
    }


###Client side

    #include <socketc.hpp>

	using namespace NS_LIBSOCKET;

    int main(void)
    {
		socketc_tcp_v4 TCP;

		TCP.server_init("127.0.0.1", 80);

        /* MESSAGE HANDLER BELOW */
		// Such as data_send(...)/data_recv(...) etc.

        return 0;
    }

###Command line
    gcc app.c -lsocketcd -lpthread

## Performence
	TBD

