#include "include/server.h"
#include "include/http_request.h"
#include "include/platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8203
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 30000

void handle_request(int client_socket, const char* request_line) {
    HttpRequest* request = parse_http_request(request_line);
    if (!request) {
        HttpResponse* error_response = create_response(400, "text/plain", "Bad Request");
        send_response(client_socket, error_response);
        free_response(error_response);
        return;
    }

    // Example request handling - you can expand this with more sophisticated routing
    if (strcmp(request->method, "GET") == 0) {
        HttpResponse* response = create_response(200, "text/plain", "Hello, world!");
        send_response(client_socket, response);
        free_response(response);
    } else {
        HttpResponse* error_response = create_response(405, "text/plain", "Method Not Allowed");
        send_response(client_socket, error_response);
        free_response(error_response);
    }

    free_http_request(request);
}

int main(void) {
    if (platform_init() != 0) {
        fprintf(stderr, "Failed to initialize platform\n");
        return EXIT_FAILURE;
    }

    ServerConfig* config = init_server_config(PORT, MAX_CONNECTIONS);
    if (!config) {
        perror("Failed to initialize server config");
        platform_cleanup();
        return EXIT_FAILURE;
    }

    int server_fd = create_server_socket(config);
    if (server_fd < 0) {
        free(config);
        platform_cleanup();
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        
        if (client_socket < 0) {
            perror("In accept");
            continue;
        }

        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_read = socket_read(client_socket, buffer, BUFFER_SIZE - 1);
        
        if (bytes_read > 0) {
            // Get the first line (request line)
            char* request_line = strtok(buffer, "\n");
            if (request_line) {
                handle_request(client_socket, request_line);
            }
        }

        close_socket(client_socket);
    }

    close_socket(server_fd);
    free(config);
    platform_cleanup();
    return 0;
}
