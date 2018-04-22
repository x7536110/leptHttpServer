#include "epollAPI.h"
void do_epoll(int listenfd){
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    epollfd=epoll_create(FDSIZE);
    string content;
    string responseheader;
    add_event(epollfd,listenfd,EPOLLIN);
    while(1){
        ret=epoll_wait(epollfd,events,EPOLLEVENTS,-1);
        handle_events(epollfd,events,ret,listenfd,content,responseheader);
    }
    close(epollfd);
}

void add_event(int epollfd,int fd,int state){
    struct epoll_event ev;
    ev.events=state;
    ev.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
    cout<<"Add events success, epoll fd= "<<epollfd<<endl;
}

void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,string &content,string&responseheader){
    int fd;
    for(int i=0;i<num;i++){
        fd=events[i].data.fd;
        if((fd==listenfd)&&(events[i].events&EPOLLIN))
            handle_accept(epollfd,fd);
        else if(events[i].events&EPOLLIN)
            do_read(epollfd,fd,content,responseheader);
        else if(events[i].events&EPOLLOUT)
            do_write(epollfd,fd,content,responseheader);
    }

}

void handle_accept(int epollfd,int listenfd){
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen=sizeof(sockaddr);
    clifd=accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
    if(clifd==-1)
        cerr<<"Accept Error!"<<endl;
    else{
        cout<<"Accept a new client: "<<inet_ntoa(cliaddr.sin_addr)<<":"<<cliaddr.sin_port<<endl;
        add_event(epollfd,clifd,EPOLLIN);
    }
}
void do_read(int epollfd,int fd,string& content,string& responseheader){
    int msgLen=0;
    char buffer[BUFFER_SIZE];
    memset(buffer,'\0',BUFFER_SIZE);
    cout<<"Do socket read..."<<endl;
    if((msgLen=recv(fd,buffer,BUFFER_SIZE,0))==-1){
        cerr<<"Error handling incoming request"<<endl;
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
        return;
    }
    else if(msgLen==0){
        cout<<"Client Closed"<<endl;
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else{
        string method;
        string uri;
        string version;
        cout<<"Getting http responsehead..."<<endl;
        getRequestHead(buffer,msgLen,method,uri,version);
        cout<<"Doing http response..."<<endl;
        do_response(fd,method,uri,content,responseheader);
        modify_event(epollfd,fd,EPOLLOUT);
        cout<<"Method:"<<method<<endl;
        cout<<"URI:"<<uri<<endl;
        cout<<"HTTP VERSION:"<<version<<endl;
    }
    cout<<"Socket read success"<<endl;
    cout<<"-----------------------------------"<<endl;
}
void do_write(int epollfd,int fd,const string& content,const string& responseheader){
    send(fd,responseheader.c_str(),responseheader.size(),0);
    send(fd,content.c_str(),content.size(),0);
    delete_event(epollfd,fd,EPOLLOUT);
    close(fd);
}
void delete_event(int epollfd,int fd,int state){
    struct epoll_event ev;
    ev.events=state;
    ev.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}
void modify_event(int epollfd,int fd,int state){
    struct epoll_event ev;
    ev.events=state;
    ev.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}




