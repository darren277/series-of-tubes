#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stddef.h>

typedef struct {
    char *method;
    char *path;
    char *query;
    char *version;
    char *body;
    size_t body_length;
} HttpRequest;

/**
 * @brief Parse an HTTP request line into its components
 * @param line The HTTP request line to parse
 * @return A new HttpRequest structure, or NULL on error
 */
HttpRequest* parse_http_request(const char* line);

/**
 * @brief Parse the request body from the raw request data
 * @param request The request structure to populate
 * @param raw_request The complete raw request data
 * @param length The length of the raw request data
 */
void parse_request_body(HttpRequest* request, const char* raw_request, size_t length);

/**
 * @brief Free the memory allocated for an HttpRequest
 * @param request The request to free
 */
void free_http_request(HttpRequest* request);

#endif // HTTP_REQUEST_H 