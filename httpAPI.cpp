#include "httpAPI.h"

void handleAccept(int listenfd){
    sockaddr_in clientAddr;
    socklen_t clientLen=sizeof(clientAddr);
    int connfd=Accept(listenfd,(sockaddr*)&clientAddr,&clientLen);
    handleHttp(connfd);
    printf("Client addr:%u\n",clientAddr.sin_addr);
    close(connfd);
}

void handleHttp(int connfd){
    if(getRequest(connfd)<0){
        perror("http request get error");
        exit(-1);
    }
}
int getRequest(int socket){
    int msgLen=0;
    char buffer[BUFFER_SIZE];
    memset(buffer,'\0',BUFFER_SIZE);
    if((msgLen=recv(socket,buffer,BUFFER_SIZE,0))==-1){
        printf("Error handling incoming request\r\n");
        return -1;
    }
    stringstream ss;
    ss<<buffer;
    string method;
    ss>>method;
    string uri;
    ss>>uri;
    string version;
    ss>>version;

    uri.erase(uri.begin());
    ifstream file(uri);
    cout<<method<<endl;
    cout<<uri<<endl;
    cout<<version<<endl;
    string line;
    string content;
    if(file.is_open()){
        while(getline(file,line)){
            content+=line;
        }
        file.close();
    }
    else{
        printf("404\r\n");
        content="404";
    }

    string statusCode("200 OK");
    string contentType("text/html");
    //string content("<html><head><title>my simple httpserver</title></head><h1>hello bx world</h1></hody></html>");
    string contentSize(to_string(content.size()));
    string head("\r\nHTTP/1.1 ");
    string ContentType("\r\nContent-Type: ");
    string ServerHead("\r\nServer: localhost");
    string ContentLength("\r\nContent-Length: ");
    string Date("\r\nDate: ");
    string Newline("\r\n");
    time_t rawtime;
    time(&rawtime);
    string message;
    message+=head;
    message+=statusCode;
    message+=ContentType;
    message+=contentType;
    message+=ServerHead;
    message+=ContentLength;
    message+=contentSize;
    message+=Date;
    message+=(string)ctime(&rawtime);
    message+=Newline;

    int messageLength=message.size();
    int n;
    n=send(socket,message.c_str(),messageLength,0);
    n=send(socket,content.c_str(),content.size(),0);
    return n;
}


