#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main(int argc, char const *argv[])
{
    int socket1;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];

    // Create socket
    socket1 = socket(AF_INET, SOCK_STREAM, 0);
    if (socket1 == -1)
    {
        perror("Socket creation failed");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    // Connect to server
    if (connect(socket1, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected to server");

    // Get command from the user
    printf("Use SEND_FILE command to get a file from  the server.\n");
    printf("Use QUIT command to close the program\n");
    printf("Enter a command: ");
    scanf("%s", message);

	if ( strcmp( message, "SEND_FILE" ) == 0)
	{
	    // Send the  command to the server
	    if (send(socket1, message, strlen(message), 0) < 0)
	    {
		puts("Send failed");
		return 1;
	    }
	
	    //Get the file name
	    printf("Enter the file name: ");
	    scanf("%s", message);
	    send(socket1, message, BUFFER_SIZE, 0);

	    // Receive file size
	    int file_size;
	    recv(socket1, &file_size, sizeof(int), 0);

	    // Open file 
	    FILE* file = fopen(message, "wb");
	    if (file == NULL)
	    {
		printf("Error opening file %s\n", message);
		return 1;
	    }


	    // Write file
	    int bytes;
	    int bytes_received = 0;
	    
	    while (bytes_received < file_size && (bytes = recv(socket1, server_reply, BUFFER_SIZE, 0)) > 0)
	    {	    	
		fwrite(server_reply, 1, bytes, file);
		bytes_received += bytes;
	    }
	    fclose(file);
	    close(socket1);
	    return 0;
	}
	// Quit program
	else if ( strcmp( message, "QUIT" ) == 0)
	{
		puts("Closing the program");
		return 0;
	}
}

