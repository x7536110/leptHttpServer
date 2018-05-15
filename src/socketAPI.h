#ifndef SOCK_API_H
#define SOCK_API_H

#include "utils.h"

int Socket(int,int,int);
void Bind(int,const struct sockaddr* sa,socklen_t salen);
void Listen(int,int);
int Accept(int,struct sockaddr*,socklen_t*);

#endif
