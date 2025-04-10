#include "unity/unity.h"
#include "../src/http_request.c"  // Include the source file directly for testing
#include <string.h>

void setUp(void) {
    // Set up any test fixtures
}

void tearDown(void) {
    // Clean up any test fixtures
}

void test_parse_http_request(void) {
    const char* request_line = "GET /path?query=value HTTP/1.1";
    HttpRequest* request = parse_http_request(request_line);
    
    TEST_ASSERT_NOT_NULL(request);
    TEST_ASSERT_EQUAL_STRING("GET", request->method);
    TEST_ASSERT_EQUAL_STRING("/path", request->path);
    TEST_ASSERT_EQUAL_STRING("query=value", request->query);
    TEST_ASSERT_EQUAL_STRING("HTTP/1.1", request->version);
    
    free_http_request(request);
}

void test_parse_request_body(void) {
    const char* raw_request = "POST / HTTP/1.1\r\n"
                             "Content-Length: 11\r\n"
                             "\r\n"
                             "Hello World";
    HttpRequest* request = malloc(sizeof(HttpRequest));
    request->method = strdup("POST");
    request->path = strdup("/");
    request->query = NULL;
    request->version = strdup("HTTP/1.1");
    request->body = NULL;
    request->body_length = 0;
    
    parse_request_body(request, raw_request, strlen(raw_request));
    
    TEST_ASSERT_NOT_NULL(request->body);
    TEST_ASSERT_EQUAL(11, request->body_length);
    TEST_ASSERT_EQUAL_STRING("Hello World", request->body);
    
    free_http_request(request);
}

void test_parse_empty_request(void) {
    HttpRequest* request = parse_http_request("");
    TEST_ASSERT_NULL(request);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_parse_http_request);
    RUN_TEST(test_parse_request_body);
    RUN_TEST(test_parse_empty_request);
    
    return UNITY_END();
} 