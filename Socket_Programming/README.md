I created a file share application that has a server and a client. The application is written in C and uses the socket interface. 
The server code does not have multi-client feature. With this application you can get any text file that you want,from the server’s directory. 
The client creates a copy of the file with the same file name in the client’s directory. My application is only working in local. 
Client and server should be in different directories. 
I have used “The Definitive Guide to Linux Network Programming” book by Keir Davis, John W. Turner and Nathan Yocom to implement the connection between client and server.

Steps to run the code:
	Server:
		gcc server.c -o server
		./server           (server should be running before client)
	Client:
		gcc client.c -o client
		./client
		SEND_FILE
		file.txt
