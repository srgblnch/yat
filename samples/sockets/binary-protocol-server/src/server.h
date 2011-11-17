#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF		      "\r\n"
#define PORT	      	5001
#define MAX_CLIENTS 	1

#define BUF_SIZE	128

#include "client.h"

typedef void(*SocketCallback)(SOCKET s, int e, size_t n, char * b);

void init(void);
void end(void);
void app(SocketCallback cb);
int init_connection(void);
void end_connection(int sock);
int read_client(SOCKET sock, char *buffer);
void write_client(SOCKET sock, const char *buffer);
void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
void remove_client(Client *clients, int to_remove, int *actual);
void clear_clients(Client *clients, int actual);

#endif /* guard */

