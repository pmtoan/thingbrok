#ifndef __UNIX_X86_64_LINUX_TCP_SOCKET_H__
#define __UNIX_X86_64_LINUX_TCP_SOCKET_H__
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define FAIL -1

/*===================================================================================
*       FUNCTION DECLARATION
*==================================================================================*/

/*
*   todo @tcp_socket_open_listener create, bind and make socket listen
*/
int unix_x86_64_linux_tcp_socket_open_listener(int port);

/*
*   todo @tcp_socket_send_msg send @msg to socket
*/
int unix_x86_64_linux_tcp_socket_send_msg(int socket, const char* msg);

/*
*   todo @tcp_socket_read_msg read array of char into @buf
*/
int unix_x86_64_linux_tcp_socket_read_msg(int socket, char* buf, int len);

/*
*   todo @unix_x86_64_linux_tcp_socket_connect make connection to @ip:@port
*/
int unix_x86_64_linux_tcp_socket_connect(const char* ip, int port);


/*===================================================================================
*       FUNCTION IMPLEMENT
*==================================================================================*/

int unix_x86_64_linux_tcp_socket_open_listener(int port)
{   
    int sd;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if ((bind(sd, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
        return FAIL;
    if (listen(sd, 10) != 0)
        return FAIL;
    return sd;
}

int unix_x86_64_linux_tcp_socket_send_msg(int socket, const char* msg)
{
	int result = send(socket, msg, strlen(msg), 0);
    return result;
}

int unix_x86_64_linux_tcp_socket_read_msg(int socket, char* buf, int len)
{
	char *s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    if (c < 0)
        return c;
    else if (c == 0)
        buf[0] = '\0';
    else
        s[c]='\0';
    return len - slen;
}

int unix_x86_64_linux_tcp_socket_connect(const char* ip, int port)
{
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in si;
    memset(&si, 0, sizeof(si));
    si.sin_family = AF_INET;
    si.sin_addr.s_addr = inet_addr(ip);
    si.sin_port = htons(port);
    if (connect(sd, (struct sockaddr *) &si, sizeof(si)) == -1)
    {
        printf("{\"msg\":\"connection failed\"}\n");
        return -1;
    }
    return sd;
}

#endif  // __UNIX_X86_64_LINUX_TCP_SOCKET_H__