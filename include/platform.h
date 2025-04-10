#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#define close_socket(s) closesocket(s)
#define socket_error() WSAGetLastError()
#define socket_send(s, buf, len) send(s, buf, len, 0)
#define socket_recv(s, buf, len) recv(s, buf, len, 0)
#define socket_write(s, buf, len) send(s, buf, len, 0)
#define socket_read(s, buf, len) recv(s, buf, len, 0)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#define close_socket(s) close(s)
#define socket_error() errno
#define socket_send(s, buf, len) write(s, buf, len)
#define socket_recv(s, buf, len) read(s, buf, len)
#define socket_write(s, buf, len) write(s, buf, len)
#define socket_read(s, buf, len) read(s, buf, len)
#endif

// Platform-specific initialization
int platform_init(void);
void platform_cleanup(void);

#endif // PLATFORM_H 