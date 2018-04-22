#ifndef UTIL_H
#define UTIL_H
#define LISTENQ 5
#define EPOLLEVENTS 100
#define BUFFER_SIZE 1024
#define FDSIZE 1000
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "unp.h"
using namespace std;
extern string ROOT;

#endif // UTIL_H
