#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080 
#define SERVER_HTTP "192.168.14.144"
#define ERR_JUGE(RET, VAL, ERROR, RETURN)\
    do{if(RET == VAL){perror(ERROR);return RETURN;}}while(0)

int main(void)
{
    int sockfd, ret;
    struct sockaddr_in addr, caddr;
    char buff[1023];

    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    ERR_JUGE(sockfd, -1, "socket error", -1);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_HTTP, &addr.sin_addr);

    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERR_JUGE(ret, -1, "connect error", -1);

    while(1)
    {
        bzero(buff, sizeof(buff));
        gets(buff);
        ret = write(sockfd, buff, sizeof(buff));
        ERR_JUGE(ret, -1, "write error", -1);
    }

    puts("write pass");

    return 0;
}
