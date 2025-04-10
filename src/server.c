#include "../include/server.h"
#include "../include/platform.h"
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
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return -1;
    }
#endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("In socket");
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    if (bind(server_fd, (struct sockaddr *)&config->address, sizeof(config->address)) < 0) {
        perror("In bind");
#ifdef _WIN32
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        return -1;
    }

    if (listen(server_fd, config->max_connections) < 0) {
        perror("In listen");
#ifdef _WIN32
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        return -1;
    }

    return server_fd;
}

HttpResponse* create_response(int status_code, const char* content_type, const char* body) {
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
        free(response);
    }
}

int send_response(int socket, HttpResponse* response) {
    if (!response) return -1;

    char status_line[32];
    snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d OK\r\n", response->status_code);

    char content_type[64];
    snprintf(content_type, sizeof(content_type), "Content-Type: %s\r\n", response->content_type);

    char content_length[32];
    snprintf(content_length, sizeof(content_length), "Content-Length: %zu\r\n\r\n", response->body_length);

    // Send headers
#ifdef _WIN32
    send(socket, status_line, strlen(status_line), 0);
    send(socket, content_type, strlen(content_type), 0);
    send(socket, content_length, strlen(content_length), 0);
#else
    write(socket, status_line, strlen(status_line));
    write(socket, content_type, strlen(content_type));
    write(socket, content_length, strlen(content_length));
#endif

    // Send body if present
    if (response->body && response->body_length > 0) {
#ifdef _WIN32
        send(socket, response->body, response->body_length, 0);
#else
        write(socket, response->body, response->body_length);
#endif
    }

    return 0;
} 