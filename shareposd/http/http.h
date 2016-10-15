#ifndef HTTP_H
#define HTTP_H

#include "evself.h"
#include "klhttpd/evbuffer.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct http
{
    struct reactor *reactor;
    int httpsock;/*服务器套接字*/
};

struct http *createhttp(struct eventtop *etlist, int selmode, char *ip, int port);

int dispatchhttp(struct http *server);

int destroyhttp(struct http *server);
    
#ifdef __cplusplus
}
#endif

#endif
