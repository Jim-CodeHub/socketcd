#include <xsocketd.h>

void msg_handler(int cfd, struct sockaddr_in *caddr)
{
    int ret;
    char buff[1024];

    puts("msg_handler work!");

    struct timeval timeout = {0 ,0};    
    setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(struct timeval));
    while(1)
    {
        bzero(buff, sizeof(buff));
        ret = recv(cfd, buff, sizeof(buff), 0);
        if(ret <= 0)
            break;
        puts(buff); 
    }
}

int main(void)
{
    struct param_s param;
    param = xsocketd_init(xSOCK_STREAM, 8080, "192.168.14.144", 50, PPC, 0, false);
    xsocketd_start(&param, msg_handler);

    return 0;
}
