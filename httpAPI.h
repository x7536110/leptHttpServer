#ifndef HTTP_API_H
#define HTTP_API_H

#include "utils.h"
#include "socketAPI.h"
void handleAccept(int listenfd);
void getRequestHead(char* buffer,int msgLen,string& method,string& uri,string& version);
void do_response(int sockfd,string method,string uri,string& content,string& responseheader);
void do_get_response(int sockfd,string method,string uri,string& content,string& responseheader);
void do_post_response(int sockfd,string method,string uri,string& content,string& responseheader);
void do_head_response(int sockfd,string method,string uri,string& content,string& responseheader);
void do_put_response(int sockfd,string method,string uri,string& content,string& responseheader);
string getContent(string &statusCode,string uri,string &contentType);
string errorContent(int errorCode);
string parseURI(string uri);
void parseContentType(string filename,string& contentType);
string getExtent(string filename);
string constructHeader(string status,const string& content,const string& contentType);

#endif // HTTP_API_H
