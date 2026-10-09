//Title: echo_c
//Author: Oscar Ruenes Campos
//Date: 5/6/2024
//Desc: This program is run as a client for either TCP or UDP style communication. Use "-u" as an argument during execution for UDP
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 80
#define MAXLINE 1000
#define PORT 7001
#define SA struct sockaddr
// used for tcp 
void func(int sockfd)
{
	char buff[MAX];
	int n;
	// chat for tcp
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		// send message
		send(sockfd, buff, sizeof(buff),0);
		bzero(buff, sizeof(buff));
		// receive message
		recv(sockfd, buff, sizeof(buff),0);
		printf("From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main(int argc, char **argv)
{
	// check style of communication
	bool flag = false;
	for(int i = 0; i < argc; i++){
		char* x = argv[i];
		if(strcmp(x,"-u") == 0){
			flag = true;
		}
	}
	// TCP
	if(!flag){
		int sockfd, connfd;
		struct sockaddr_in servaddr, cli;

		// socket create and verification
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			printf("socket creation failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));
	
		// assign IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servaddr.sin_port = htons(PORT);
	
		// connect the client socket to server socket
		if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
			printf("connection with the server failed...\n");
			exit(0);
		}
		else
			printf("connected to the server..\n");
	
		// function for chat
		func(sockfd);
	
		// close the socket
		close(sockfd);
	}	
	//UDP
	if(flag){
		char buffer[100];
		char *message = (char *) "Hello Server";
		int sockfd, n;
		unsigned int len;
		struct sockaddr_in servaddr;
		
		// clear servaddr
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servaddr.sin_port = htons(PORT);
		servaddr.sin_family = AF_INET;
		
		// create datagram socket
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		
		// request to send datagram
		// no need to specify server address in sendto
		// connect stores the peers IP and port
		sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
		// waiting for response
		n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &len);
        	buffer[n] = '\0';
		printf("%s\n", buffer);
		close(sockfd);
	}
}

