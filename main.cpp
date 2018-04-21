#include "utils.h"
#include "socketAPI.h"
#include "httpAPI.h"
#include "epollAPI.h"

int main(int argc,char** argv)
{
    int port;
    cin>>port;
    int listenfd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=INADDR_ANY;
    serverAddr.sin_port=htons(port);
    Bind(listenfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    Listen(listenfd,5);
    while(1)
        handleAccept(listenfd);
    return 0;

}


