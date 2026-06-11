#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h>

#define PROXY "127.0.0.1"
#define PROXYPORT 9050
#define USERID "torrier"
#define req_size sizeof(struct proxy_request)
#define res_size sizeof(struct proxy_response)

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

struct proxy_request
{
    int8 vn;
    int8 cd;
    int16 dst_port;
    int32 dst_ip;
    unsigned char userid[8];
};

struct proxy_response
{
    int8 vn;
    int8 cd;
    int16 _;
    int32 __;    
};

typedef struct proxy_request Req;
typedef struct proxy_response Res;

Req *request(struct sockaddr_in *);
int connect(int, const struct sockaddr *, socklen_t);