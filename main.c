#include "include/server.h"
#include "include/http_request.h"
#include "include/platform.h"
#include "include/logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8203
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 30000

void handle_request(int client_socket, const char* raw_request, size_t length) {
    LOG_REQUEST("Received request: %.*s", (int)length, raw_request);
    
    // Parse the request line (first line)
    char* request_line = strtok(strdup(raw_request), "\n");
    if (!request_line) {
        LOG_ERROR("Empty request line");
        return;
    }

    HttpRequest* request = parse_http_request(request_line);
    if (!request) {
        LOG_ERROR("Failed to parse request");
        HttpResponse* error_response = create_response(400, "text/plain", "Bad Request");
        send_response(client_socket, error_response);
        free_response(error_response);
        free(request_line);
        return;
    }

    // Parse the request body if it's a POST request
    if (strcmp(request->method, "POST") == 0) {
        parse_request_body(request, raw_request, length);
    }

    // Handle the request based on method
    if (strcmp(request->method, "GET") == 0) {
        LOG_REQUEST("Handling GET request");
        HttpResponse* response = create_response(200, "text/plain", "Hello, world!");
        send_response(client_socket, response);
        free_response(response);
    } else if (strcmp(request->method, "POST") == 0) {
        LOG_REQUEST("Handling POST request");
        if (request->body) {
            char response_body[1024];
            snprintf(response_body, sizeof(response_body), 
                    "Received POST data: %s", request->body);
            HttpResponse* response = create_response(200, "text/plain", response_body);
            send_response(client_socket, response);
            free_response(response);
        } else {
            HttpResponse* response = create_response(200, "text/plain", "POST received but no body");
            send_response(client_socket, response);
            free_response(response);
        }
    } else {
        LOG_ERROR("Unsupported method: %s", request->method);
        HttpResponse* error_response = create_response(405, "text/plain", "Method Not Allowed");
        send_response(client_socket, error_response);
        free_response(error_response);
    }

    free(request_line);
    free_http_request(request);
}

int main(void) {
    LOG_RESPONSE("Starting HTTP server");
    
    if (platform_init() != 0) {
        LOG_ERROR("Failed to initialize platform");
        return EXIT_FAILURE;
    }

    ServerConfig* config = init_server_config(PORT, MAX_CONNECTIONS);
    if (!config) {
        LOG_ERROR("Failed to initialize server config");
        platform_cleanup();
        return EXIT_FAILURE;
    }

    int server_fd = create_server_socket(config);
    if (server_fd < 0) {
        LOG_ERROR("Failed to create server socket");
        free(config);
        platform_cleanup();
        return EXIT_FAILURE;
    }

    LOG_RESPONSE("Server listening on port %d", PORT);

    while (1) {
        LOG_REQUEST("Waiting for new connection...");
        
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        
        if (client_socket < 0) {
            LOG_ERROR("Failed to accept connection");
            continue;
        }

        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_read = socket_read(client_socket, buffer, BUFFER_SIZE - 1);
        
        if (bytes_read > 0) {
            LOG_REQUEST("Received %zd bytes of data", bytes_read);
            handle_request(client_socket, buffer, bytes_read);
        } else if (bytes_read == 0) {
            LOG_REQUEST("Client closed connection");
        } else {
            LOG_ERROR("Error reading from socket");
        }

        close_socket(client_socket);
        LOG_REQUEST("Connection closed");
    }

    close_socket(server_fd);
    free(config);
    platform_cleanup();
    return 0;
}
