#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 256

void send_file(int client_socket)
{
    char file_name[256];
    recv(client_socket, file_name, BUFFER_SIZE, 0);

    FILE* file = fopen(file_name, "rb");
    if (file == NULL)
    {
        char error_open_file[256] = "Open file failed.";
        send(client_socket, error_open_file, strlen(error_open_file), 0);
        return;
    }


    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    // Send file size first
    send(client_socket, &file_size, sizeof(int), 0);

    // Send file
    char buffer[BUFFER_SIZE];
    int bytes_sent;
    int total_bytes_sent = 0;
    while (total_bytes_sent < file_size)
    {
        bytes_sent = fread(buffer, 1, BUFFER_SIZE, file);
        total_bytes_sent += send(client_socket, buffer, bytes_sent, 0);
    }
    fclose(file);
}

int main(int argc, char const *argv[])
{
    int server_socket, client_socket, c, read_size;
    struct sockaddr_in server, client;
    char client_message[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("Socket creation failed");
    }
    puts("Socket created");

    // Prepare the socket address structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind
    if(bind(server_socket,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(server_socket , 3);

    // Accept connection
    puts("Waiting for connection");
    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_socket < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    // Receive file name
    recv(client_socket, client_message, BUFFER_SIZE, 0);
    
    // Send file 
    if (strcmp(client_message, "SEND_FILE") == 0)
    {
        send_file(client_socket);
    }

    close(client_socket);
    return 0;
}


