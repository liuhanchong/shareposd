#include "http.h"
#include "log.h"
#include "util.h"
#include "../control/control.h"

static void handle(struct reactor *reactor)
{
    //处理事件
    forlist(reactor->uatvevelist)
    {
        struct event *uevent = (struct event *)headlistnode->data;
        
        //调用接口函数
        if (uevent->call(uevent, uevent->arg) == fastr)
        {
            ploginfo(LERROR, "handle->call failed sock=%d", uevent->fd);
        }
    }
}

struct http *createhttp(struct eventtop *etlist, int selmode, char *ip, int port)
{
    struct http *server = cnew(struct http);
    if (!server)
    {
        return NULL;
    }
    
    //创建反应堆模型
    if ((server->reactor = createreactor(etlist, selmode)) == NULL)
    {
        ploginfo(LERROR, "createhttp->createreactor failed");
        return NULL;
    }
    
    //创建服务器
    server->httpsock = sock(AF_INET, SOCK_STREAM, 0);
    if (server->httpsock < 0)
    {
        ploginfo(LERROR, "createhttp->socket failed");
        return NULL;
    }
    
    //设置sock选项
    int flags = 1;
    struct linger linger = {0, 0};
    setsocketopt(server->httpsock, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
    setsocketopt(server->httpsock, SOL_SOCKET, SO_LINGER, (void *)&linger, sizeof(linger));
    setsocketopt(server->httpsock, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));
    
    struct sockaddr_in sockaddr;
    setsockaddrin(&sockaddr, AF_INET, port, ip);
    if (bindsock(server->httpsock, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_in)) != 0)
    {
        ploginfo(LERROR, "createhttp->bindsock failed %d", errno);
        return NULL;
    }
    
    if (listensock(server->httpsock, 5) == -1)
    {
        ploginfo(LERROR, "createhttp->listensock failed");
        return NULL;
    }
    
    //控制器添加接收
    if (control::accept(server) == 0)
    {
        ploginfo(LERROR, "createhttp->accept failed");
        return NULL;
    }
    
    //创建资源
    control::createres();
    
    return server;
}

int dispatchhttp(struct http *server)
{
    while (server->reactor->listen)
    {
        //获取活动的事件
        if (dispatchreactor(server->reactor) == 0)
        {
            return 0;
        }
        
        //处理活动事件
        handle(server->reactor);
    }
    
    return 1;
}

int destroyhttp(struct http *server)
{
    close(server->httpsock);
    
    control::destroyres();
    
    if (destroyreactor(server->reactor) == 0)
    {
        ploginfo(LERROR, "destroyhttp->destroyreactor failed");
        return 0;
    }
    
    cfree(server);
    
    return 1;
}

