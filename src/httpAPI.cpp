#include "httpAPI.h"

void getRequestHead(char* buffer,int msgLen,string& method,string& uri,string& version){
    stringstream ss(string(buffer,buffer+msgLen));
    ss>>method;
    ss>>uri;
    ss>>version;
}
bool parseStaticHead(string& uri,string& cgiRequest){
    for(int i=0;i<uri.size();i++)
        if(uri[i]=='?')
            return DYNAMIC;
    return STATIC;
}
void do_response(int sockfd,string method,string uri,string& content,string& responseheader){
    if(method=="GET"){
        do_get_response(sockfd,method,uri,content,responseheader);
    }else
    if(method=="POST"){
        do_post_response(sockfd,method,uri,content,responseheader);
    }else
    if(method=="HEAD"){
        do_head_response(sockfd,method,uri,content,responseheader);
    }else
    if(method=="PUT"){
        do_put_response(sockfd,method,uri,content,responseheader);
    }else{
    }
}
void do_get_response(int sockfd,string method,string uri,string& content,string& responseheader){
    string contentType;
    string statusCode;
    string cgiRequest;
    char *emptylist[]={NULL};
    if(parseStaticHead(uri,cgiRequest)){
        content=getContent(statusCode,uri,contentType);
        responseheader=constructHeader(statusCode,content,contentType);
        send(sockfd,responseheader.data(),responseheader.size(),0);
        send(sockfd,content.data(),content.size(),0);
    }else{
        if(fork()==0){
            setenv("QUERY_STRING",uri.c_str(),1);
            //content=
            //responseheader=
            execve(cgiRequest.c_str(),emptylist,environ);
            send(sockfd,responseheader.data(),responseheader.size(),0);
            send(sockfd,content.data(),content.size(),0);
        }
        wait(NULL);
    }
}
void do_post_response(int sockfd,string method,string uri,string& content,string& responseheader){

}
void do_head_response(int sockfd,string method,string uri,string& content,string& responseheader){

}
void do_put_response(int sockfd,string method,string uri,string& content,string& responseheader){

}
string getContent(string &statusCode,string uri,string &contentType){
    string filename=parseURI(uri);
    parseContentType(filename,contentType);
    filebuf *fptr;
    char* buffer;
    ifstream fin(filename,ifstream::binary);
    fptr=fin.rdbuf();
    if(!fin){
        statusCode="404 Not Found";
        cerr<<"404 ERROR NO FILE NAMED "<<filename<<" CONTENT TYPE "<<contentType<<endl;
        return errorContent(404);
    }else{
        cerr<<"200 OK FILE FIND "<<filename<<" CONTENT TYPE "<<contentType<<endl;
    }
    string tmp;
    int filesize=fptr->pubseekoff(0,ios::end,ios::in);
    fptr->pubseekpos(0,ios::in);
    buffer=new char[filesize];
    fin.read(buffer,filesize);
    string content;
    content.assign(buffer,filesize);
    statusCode="200 OK";
    fin.close();
    delete []buffer;
    cout<<"Filesize: "<<content.size()<<" byte ("<<content.size()/1024<<"KB)"<<endl;
    return content;
}
string errorContent(int errorCode){
    ifstream fin;
    string tmp;
    string content;
    if(errorCode==404)
        fin.open("404.html");
    if(!fin.is_open()){
        content=to_string(errorCode);
        return content;
    }
    while(fin>>tmp)
        content+=tmp;
    fin.close();
    return content;
}
string parseURI(string uri){
    string weburi;
    if(uri=="/")
        weburi+="/index.html";
    else
        weburi+=uri;
    weburi.erase(weburi.begin());
    return weburi;
}
void parseContentType(string filename,string& contentType){
    string tmpExtension=getExtent(filename);
    ifstream mimefile("mime.types");
    string line;
    if(mimefile.is_open()==false){
        cerr<<"MIME CONFIG ERROR"<<endl;
    }
    while(getline(mimefile,line)){
        if(line[0]!='#'){
            stringstream lineStream(line);
            contentType.clear();
            lineStream>>contentType;
            vector<string>extensions;
            string extension;
            while(lineStream>>extension)
                extensions.push_back(extension);
            for(int i=0;i<extensions.size();i++){
                if(tmpExtension==extensions[i]){
                    mimefile.close();
                    return ;
                }
            }
        }
    }
    contentType="text/plain";
    mimefile.close();
    return ;
}
string getExtent(string filename){
    int i;
    for(i=filename.size()-1;;--i){
        if(filename[i]=='.')
            break;
    }
    return string(filename.begin()+i+1,filename.end());
}
string constructHeader(string status,const string& content,const string& contentType){
    string statusCode(status);
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
    return message;
}
