#include "../include/platform.h"
#include <stdio.h>

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

int platform_init(void) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return -1;
    }
#endif
    return 0;
}

void platform_cleanup(void) {
#ifdef _WIN32
    WSACleanup();
#endif
} 