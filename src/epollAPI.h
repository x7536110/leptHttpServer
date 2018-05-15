#ifndef EPOLL_API_H
#define EPOLL_API_H

#include "utils.h"
#include "httpAPI.h"
void do_epoll(int listenfd);
void add_event(int epollfd,int fd,int state);
void handle_events(int epollfd,struct epoll_event* events,int num,int listenfd,string& content,string& responseheader);
void handle_accept(int epollfd,int listenfd);
void do_read(int epollfd,int fd,string& content,string& responseheader);
void do_write(int epollfd,int fd,const string& content,const string& responseheader);
void delete_event(int epollfd,int fd,int state);
void modify_event(int epollfd,int fd,int state);

#endif // EPOLL_API_H
