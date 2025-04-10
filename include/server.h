#ifndef SERVER_H
#define SERVER_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

typedef struct {
    int port;
    int max_connections;
    struct sockaddr_in address;
} ServerConfig;

typedef struct {
    int status_code;
    const char* content_type;
    const char* body;
    size_t body_length;
} HttpResponse;

/**
 * @brief Check if a port is available
 * @param port The port to check
 * @return 0 if port is available, -1 if in use
 */
int is_port_available(int port);

/**
 * @brief Initialize server configuration
 * @param port The port to listen on
 * @param max_connections Maximum number of queued connections
 * @return A new ServerConfig structure
 */
ServerConfig* init_server_config(int port, int max_connections);

/**
 * @brief Create and configure a server socket
 * @param config Server configuration
 * @return Socket file descriptor, or -1 on error
 */
int create_server_socket(ServerConfig* config);

/**
 * @brief Create an HTTP response
 * @param status_code HTTP status code
 * @param content_type Content type of the response
 * @param body Response body
 * @return A new HttpResponse structure
 */
HttpResponse* create_response(int status_code, const char* content_type, const char* body);

/**
 * @brief Free the memory allocated for a response
 * @param response The response to free
 */
void free_response(HttpResponse* response);

/**
 * @brief Send an HTTP response over a socket
 * @param socket The socket to write to
 * @param response The response to send
 * @return Number of bytes written, or -1 on error
 */
int send_response(int socket, HttpResponse* response);

#endif // SERVER_H 