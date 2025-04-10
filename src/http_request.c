#include "../include/http_request.h"
#include "../include/logging.h"
#include <string.h>
#include <stdlib.h>

HttpRequest* parse_http_request(const char* line) {
    if (!line) return NULL;

    LOG_REQUEST("Parsing request line: %s", line);

    HttpRequest* request = malloc(sizeof(HttpRequest));
    if (!request) return NULL;

    // Initialize all pointers to NULL
    request->method = NULL;
    request->path = NULL;
    request->query = NULL;
    request->version = NULL;

    // Find the method
    const char* method_end = strchr(line, ' ');
    if (!method_end) {
        LOG_ERROR("Invalid request: missing method");
        goto error;
    }
    
    size_t method_len = method_end - line;
    request->method = malloc(method_len + 1);
    if (!request->method) goto error;
    strncpy(request->method, line, method_len);
    request->method[method_len] = '\0';
    LOG_REQUEST("Method: %s", request->method);

    // Find the path
    const char* path_start = method_end + 1;
    const char* path_end = strchr(path_start, ' ');
    if (!path_end) {
        LOG_ERROR("Invalid request: missing path");
        goto error;
    }

    // Check for query parameters
    const char* query_start = strchr(path_start, '?');
    if (query_start && query_start < path_end) {
        // Path without query
        size_t path_len = query_start - path_start;
        request->path = malloc(path_len + 1);
        if (!request->path) goto error;
        strncpy(request->path, path_start, path_len);
        request->path[path_len] = '\0';
        LOG_REQUEST("Path: %s", request->path);

        // Query string
        size_t query_len = path_end - query_start;
        request->query = malloc(query_len + 1);
        if (!request->query) goto error;
        strncpy(request->query, query_start, query_len);
        request->query[query_len] = '\0';
        LOG_REQUEST("Query: %s", request->query);
    } else {
        // No query parameters
        size_t path_len = path_end - path_start;
        request->path = malloc(path_len + 1);
        if (!request->path) goto error;
        strncpy(request->path, path_start, path_len);
        request->path[path_len] = '\0';
        LOG_REQUEST("Path: %s", request->path);
    }

    // Find the version
    const char* version_start = path_end + 1;
    const char* version_end = strchr(version_start, '\r');
    if (!version_end) version_end = strchr(version_start, '\n');
    if (!version_end) version_end = version_start + strlen(version_start);

    size_t version_len = version_end - version_start;
    request->version = malloc(version_len + 1);
    if (!request->version) goto error;
    strncpy(request->version, version_start, version_len);
    request->version[version_len] = '\0';
    LOG_REQUEST("HTTP Version: %s", request->version);

    return request;

error:
    LOG_ERROR("Failed to parse request");
    free_http_request(request);
    return NULL;
}

void free_http_request(HttpRequest* request) {
    if (!request) return;
    
    free(request->method);
    free(request->path);
    free(request->query);
    free(request->version);
    free(request);
} 