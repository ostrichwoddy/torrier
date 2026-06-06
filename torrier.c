#include "torrier.h"


int main(int argc, char *argv[])
{
    char *host;
    int port;
    
    int s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buffer[res_size];
    int success;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return 1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
    	perror("Socket");
    	return 1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if (connect(s, (struct sockaddr *)&sock, sizeof(sock)))
    {
        perror("Connect");
        return 1;
    }
    printf("Proxy connection has been established successfully\n");
    
    req = request(host, port);
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

    printf("Successfully connected through the proxy to %s:%d\n", host, port);    
    close(s);
    free(req);

    return 0;
}

Req *request(const char *dst_ip, const int dst_port)
{
    Req *req;
    
    req = malloc(req_size);
    if (req == NULL)
        return req;
    
    req->vn = 4;
    req->cd = 1;
    req->dst_port = htons(dst_port);
    req->dst_ip = inet_addr(dst_ip);
    strncpy((char *)req->userid, USERID, 8);

    return req;
}