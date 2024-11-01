// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8203


int parse(const char* line)
{
    /* Find out where everything is */
    const char *start_of_path = strchr(line, ' ') + 1;
    const char *start_of_query = strchr(start_of_path, '?');
    const char *end_of_query = strchr(start_of_query, ' ');

    /* Get the right amount of memory */
    char path[start_of_query - start_of_path];
    char query[end_of_query - start_of_query];

    /* Copy the strings into our memory */
    strncpy(path, start_of_path,  start_of_query - start_of_path);
    strncpy(query, start_of_query, end_of_query - start_of_query);

    /* Null terminators (because strncpy does not provide them) */
    path[sizeof(path)] = 0;
    query[sizeof(query)] = 0;

    /*Print */
    printf("%s%ld\n", query, sizeof(query));
    printf("%s%ld\n", path, sizeof(path));
}


int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);

        char *ch;
        int i = 0;
        ch = strtok(buffer, "\n");
        while (ch != NULL) {
            printf("%s\n", ch);
            i++;
            printf("%d\n", i);
            if (ch != NULL && i == 1 && strstr(ch, "?") != NULL) {parse(ch);};
            //parse("GET /path/script.cgi?field1=value1&field2=value2 HTTP/1.1");
            ch = strtok(NULL, "\n");
        }
        
        char *body = "Hello, world!";
        char *new_lines = "\n\n";
        char *payload = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
        char buf[256];
        snprintf(buf, sizeof(buf), "%s%ld%s%s", payload, strlen(body), new_lines, body);
        write(new_socket, buf, strlen(buf));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}
