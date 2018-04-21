#include "socketAPI.h"

int Socket(int family,int type,int protocol){
    int n;
    if((n=socket(family,type,protocol))<0){
        printf("Socket error\r\n");
        return -1;
    }
    return n;
}
void Bind(int fd,const struct sockaddr* sa,socklen_t salen){
    if(bind(fd,sa,salen)<0){
        printf("Bind error\r\n");
        exit(-1);
    }
}
void Listen(int fd,int backlog){
    char *ptr;
    if((ptr=getenv("LISTENQ"))!=NULL)
        backlog=atoi(ptr);
    if(listen(fd,backlog)<0){
        printf("Listen error\r\n");
        return ;
    }
}
int Accept(int fd,struct sockaddr* sa,socklen_t* salenptr){
    int n;
    while(1){
        if((n=accept(fd,sa,salenptr))<0){
#ifdef EPROTO
            if(errno==EPROTO||errno==ECONNABORTED)
#else
            if(errno==ECONNABORTED)
#endif // EPROTO
                continue;
            else{
                printf("Accept error\r\n");
                return -1;
            }
        }
        else break;
    }
    return n;

}
