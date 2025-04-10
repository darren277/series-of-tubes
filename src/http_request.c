#include "../include/http_request.h"
#include "../include/logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void log_bytes(const char* prefix, const char* data, size_t length) {
    char hex[length * 3 + 1];
    char ascii[length + 1];
    
    for (size_t i = 0; i < length; i++) {
        sprintf(hex + i * 3, "%02X ", (unsigned char)data[i]);
        ascii[i] = (data[i] >= 32 && data[i] <= 126) ? data[i] : '.';
    }
    hex[length * 3] = '\0';
    ascii[length] = '\0';
    
    LOG_REQUEST("%s (hex): %s", prefix, hex);
    LOG_REQUEST("%s (ascii): %s", prefix, ascii);
}

HttpRequest* parse_http_request(const char* line) {
    LOG_REQUEST("Parsing request line: %s", line);
    
    HttpRequest* request = malloc(sizeof(HttpRequest));
    if (!request) {
        LOG_ERROR("Failed to allocate memory for request");
        return NULL;
    }
    
    // Initialize all fields
    request->method = NULL;
    request->path = NULL;
    request->query = NULL;
    request->version = NULL;
    request->body = NULL;
    request->body_length = 0;
    
    // Parse method
    char* token = strtok(strdup(line), " ");
    if (!token) {
        LOG_ERROR("Invalid request: missing method");
        free_http_request(request);
        return NULL;
    }
    request->method = strdup(token);
    LOG_REQUEST("Method: %s", request->method);
    
    // Parse path and query
    token = strtok(NULL, " ");
    if (!token) {
        LOG_ERROR("Invalid request: missing path");
        free_http_request(request);
        return NULL;
    }
    
    // Split path and query
    char* query_start = strchr(token, '?');
    if (query_start) {
        *query_start = '\0';
        request->path = strdup(token);
        request->query = strdup(query_start + 1);
        LOG_REQUEST("Path: %s, Query: %s", request->path, request->query);
    } else {
        request->path = strdup(token);
        request->query = NULL;
        LOG_REQUEST("Path: %s", request->path);
    }
    
    // Parse HTTP version
    token = strtok(NULL, "\r\n");
    if (!token) {
        LOG_ERROR("Invalid request: missing HTTP version");
        free_http_request(request);
        return NULL;
    }
    request->version = strdup(token);
    LOG_REQUEST("HTTP Version: %s", request->version);
    
    return request;
}

void parse_request_body(HttpRequest* request, const char* raw_request, size_t length) {
    LOG_REQUEST("Starting request body parsing");
    log_bytes("Raw request", raw_request, length);
    
    // Find the end of headers (double newline)
    const char* body_start = strstr(raw_request, "\r\n\r\n");
    if (!body_start) {
        LOG_REQUEST("No \\r\\n\\r\\n found, trying \\n\\n");
        body_start = strstr(raw_request, "\n\n");
    }
    
    if (!body_start) {
        LOG_REQUEST("No body found - no double newline detected");
        request->body = NULL;
        request->body_length = 0;
        return;
    }
    
    // Move past the newlines
    size_t header_end_len = (body_start[1] == '\n') ? 4 : 2;
    body_start += header_end_len;
    LOG_REQUEST("Header end length: %zu", header_end_len);
    
    // Calculate body length
    size_t body_length = length - (body_start - raw_request);
    LOG_REQUEST("Calculated body length: %zu", body_length);
    
    if (body_length == 0) {
        LOG_REQUEST("No body found - empty body after headers");
        request->body = NULL;
        request->body_length = 0;
        return;
    }
    
    // Allocate and copy the body
    request->body = malloc(body_length + 1);
    if (!request->body) {
        LOG_ERROR("Failed to allocate memory for request body");
        request->body_length = 0;
        return;
    }
    
    memcpy(request->body, body_start, body_length);
    request->body[body_length] = '\0';
    request->body_length = body_length;
    
    log_bytes("Parsed body", request->body, body_length);
    LOG_REQUEST("Successfully parsed request body of length %zu", body_length);
}

void free_http_request(HttpRequest* request) {
    if (request) {
        free(request->method);
        free(request->path);
        free(request->query);
        free(request->version);
        free(request->body);
        free(request);
    }
} 