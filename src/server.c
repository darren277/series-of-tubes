#include "../include/server.h"
#include "../include/platform.h"
#include "../include/logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#endif

ServerConfig* init_server_config(int port, int max_connections) {
    LOG_RESPONSE("Initializing server configuration");
    LOG_RESPONSE("Port: %d, Max Connections: %d", port, max_connections);

    ServerConfig* config = malloc(sizeof(ServerConfig));
    if (!config) return NULL;

    config->port = port;
    config->max_connections = max_connections;
    
    config->address.sin_family = AF_INET;
    config->address.sin_addr.s_addr = INADDR_ANY;
    config->address.sin_port = htons(port);
    memset(config->address.sin_zero, '\0', sizeof(config->address.sin_zero));

    return config;
}

int create_server_socket(ServerConfig* config) {
    LOG_RESPONSE("Creating server socket");

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        LOG_ERROR("Failed to create socket");
        return -1;
    }

    if (bind(server_fd, (struct sockaddr *)&config->address, sizeof(config->address)) < 0) {
        LOG_ERROR("Failed to bind socket");
        close_socket(server_fd);
        return -1;
    }

    if (listen(server_fd, config->max_connections) < 0) {
        LOG_ERROR("Failed to listen on socket");
        close_socket(server_fd);
        return -1;
    }

    LOG_RESPONSE("Server socket created and listening");
    return server_fd;
}

HttpResponse* create_response(int status_code, const char* content_type, const char* body) {
    LOG_RESPONSE("Creating HTTP response");
    LOG_RESPONSE("Status: %d, Content-Type: %s", status_code, content_type);
    if (body) {
        LOG_RESPONSE("Body length: %zu", strlen(body));
    }

    HttpResponse* response = malloc(sizeof(HttpResponse));
    if (!response) return NULL;

    response->status_code = status_code;
    response->content_type = content_type;
    response->body = body;
    response->body_length = body ? strlen(body) : 0;

    return response;
}

void free_response(HttpResponse* response) {
    if (response) {
        LOG_RESPONSE("Freeing response");
        free(response);
    }
}

int send_response(int socket, HttpResponse* response) {
    if (!response) return -1;

    LOG_RESPONSE("Sending HTTP response");
    LOG_RESPONSE("Status: %d %s", response->status_code, 
                response->status_code == 200 ? "OK" : 
                response->status_code == 400 ? "Bad Request" :
                response->status_code == 405 ? "Method Not Allowed" : "Unknown");

    char status_line[32];
    snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d OK\r\n", response->status_code);
    LOG_RESPONSE("Status Line: %s", status_line);

    char content_type[64];
    snprintf(content_type, sizeof(content_type), "Content-Type: %s\r\n", response->content_type);
    LOG_RESPONSE("Content-Type: %s", response->content_type);

    char content_length[32];
    snprintf(content_length, sizeof(content_length), "Content-Length: %zu\r\n\r\n", response->body_length);
    LOG_RESPONSE("Content-Length: %zu", response->body_length);

    // Send headers
    socket_write(socket, status_line, strlen(status_line));
    socket_write(socket, content_type, strlen(content_type));
    socket_write(socket, content_length, strlen(content_length));

    // Send body if present
    if (response->body && response->body_length > 0) {
        LOG_RESPONSE("Sending response body");
        socket_write(socket, response->body, response->body_length);
    }

    LOG_RESPONSE("Response sent successfully");
    return 0;
} 