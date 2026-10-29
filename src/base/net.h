 
 

#ifndef BASE_NET_H
#define BASE_NET_H

#include "types.h"

#include <chrono>
#include <cstddef>
#include <string>

#ifdef CONF_FAMILY_UNIX
#include <sys/un.h> 
#endif

 \
\


 \
\
\
\


 \
\
\
\


 \
\

extern const NETADDR NETADDR_ZEROED;

#ifdef CONF_FAMILY_UNIX
 \
\

typedef int UNIXSOCKET;

 \
\

typedef struct sockaddr_un UNIXSOCKETADDR;
#endif

 \
\
\
\
\
\
\
\
\
\
\

int net_addr_comp(const NETADDR *a, const NETADDR *b);

 \
\
\
\
\
\
\
\
\
\
\

int net_addr_comp_noport(const NETADDR *a, const NETADDR *b);

 \
\
\
\
\
\
\
\
\
\
\

void net_addr_str(const NETADDR *addr, char *string, int max_length, bool add_port);

 \
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\
\

int net_addr_from_url(NETADDR *addr, const char *string, char *host_buf, size_t host_buf_size);

 \
\
\
\
\
\
\
\

bool net_addr_is_local(const NETADDR *addr);

 \
\
\
\
\
\
\
\
\

int net_addr_from_str(NETADDR *addr, const char *string);

 \
\
\
\
\
\
\
\
\
\

int net_host_lookup(const char *hostname, NETADDR *addr, int types);

 \
\
\
\
\
\

void net_init();

 \
\
\
\
\
\

int net_errno();

 \
\
\
\
\
\

std::string net_error_message();

 \
\
\
\
\
\

void net_stats(NETSTATS *stats);

 \
\
\
\
\
\
\
\
\

int net_socket_type(NETSOCKET sock);

 \
\
\
\
\
\
\
\

int net_set_non_blocking(NETSOCKET sock);

 \
\
\
\
\
\
\
\

int net_set_blocking(NETSOCKET sock);

 \
\
\
\
\
\

int net_would_block();

 \
\
\
\
\
\
\
\
\

int net_socket_read_wait(NETSOCKET sock, std::chrono::nanoseconds nanoseconds);

 \
\
\
\


 \
\
\
\
\
\
\
\

NETSOCKET net_udp_create(NETADDR bindaddr);

 \
\
\
\
\
\
\
\
\
\
\

int net_udp_send(NETSOCKET sock, const NETADDR *addr, const void *data, int size);

 \
\
\
\
\
\
\
\
\
\

int net_udp_recv(NETSOCKET sock, NETADDR *addr, unsigned char **data);

 \
\
\
\
\
\
\
\
\
\
\

bool net_udp_is_broken(NETSOCKET sock);

 \
\
\
\
\
\

void net_udp_close(NETSOCKET sock);

 \
\
\
\


 \
\
\
\
\
\
\
\

NETSOCKET net_tcp_create(NETADDR bindaddr);

 \
\
\
\
\
\
\
\
\

int net_tcp_listen(NETSOCKET sock, int backlog);

 \
\
\
\
\
\
\
\
\
\

int net_tcp_accept(NETSOCKET sock, NETSOCKET *new_sock, NETADDR *addr);

 \
\
\
\
\
\
\
\
\
\

int net_tcp_connect(NETSOCKET sock, const NETADDR *addr);

 \
\
\
\
\
\
\
\
\

int net_tcp_connect_non_blocking(NETSOCKET sock, NETADDR bindaddr);


 \
\
\
\
\
\
\
\
\
\

int net_tcp_connect_timeout(NETSOCKET sock, const NETADDR *addr, int TimeoutMs);


 \
\
\
\
\
\
\
\
\
\

int net_tcp_send(NETSOCKET sock, const void *data, int size);

 \
\
\
\
\
\
\
\
\
\
\

int net_tcp_recv(NETSOCKET sock, void *data, int maxsize);

 \
\
\
\
\
\

void net_tcp_close(NETSOCKET sock);

#if defined(CONF_FAMILY_UNIX)
 \
\
\
\


 \
\
\
\
\
\

UNIXSOCKET net_unix_create_unnamed();

 \
\
\
\
\
\
\
\
\
\
\

int net_unix_send(UNIXSOCKET sock, UNIXSOCKETADDR *addr, void *data, int size);

 \
\
\
\
\
\
\

void net_unix_set_addr(UNIXSOCKETADDR *addr, const char *path);

 \
\
\
\
\
\

void net_unix_close(UNIXSOCKET sock);
#endif

#endif
