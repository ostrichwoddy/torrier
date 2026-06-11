#include "torrier.h"


int connect(int s2, const struct sockaddr *sock2, socklen_t addrlen)
{
    int s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buffer[res_size];
    int success;
    char tmp[512];
    int (*original_connect)(int, const struct sockaddr*, socklen_t);

    original_connect = dlsym(RTLD_NEXT, "connect");
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
    	perror("Socket");
    	return 1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if (original_connect(s, (struct sockaddr *)&sock, sizeof(sock)))
    {
        perror("Connect");
        return 1;
    }
    printf("Proxy connection has been established successfully\n");
    
    req = request((struct sockaddr_in *)sock2);
    if (req == NULL)
    {
        perror("Request");
        return 1;
    }

    write(s, req, req_size);
    memset(buffer, 0, res_size);
    if (read(s, buffer, res_size) < 1)
    {
        perror("Read");
        free(req);
        close(s);
        return 1;
    }

    res = (Res *)buffer;
    success = (res->cd == 90);
    if (!success)
    {
        fprintf(stderr, "Unable to traverse the proxy, Error code: %d", res->cd);
        close(s);
        free(req);
        return 1;
    }

    printf("Successfully connected through the proxy\n");   
    dup2(s, s2); 
    free(req);
    return 0;
}


Req *request(struct sockaddr_in * sock2)
{
    Req *req;
    
    req = malloc(req_size);
    if (req == NULL)
        return req;
    
    req->vn = 4;
    req->cd = 1;
    req->dst_port = sock2->sin_port;
    req->dst_ip = sock2->sin_addr.s_addr;
    strncpy((char *)req->userid, USERID, 8);

    return req;
}