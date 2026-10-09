//Title: echo_s
//Author: Oscar Ruenes Campos
//Date: 5/6/2024
//Class: CS 3377.007
//Desc: This is a server program that accepts three ports as arguments and can handle both TCP and UDP style of communication on them.
//A port is required when executing the program, and the server echos the messages from a client back to the client.
//The server also sends the message and ip address of clients to log_s server for tracking.

#include <stdio.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX 80
#define MAXLINE 1000
#define PORT 9000
#define SA struct sockaddr
using namespace std;
char buff[MAX];
//for tcp communication between client
void func(int connfd, char* ip){
	int n, logfd;
	struct sockaddr_in logaddr;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);
		// read the message from client and copy it in buffer
		recv(connfd, buff, sizeof(buff),0);
		// and send that buffer to client
		send(connfd, buff, sizeof(buff),0);
		// also send to log_s
		printf("Sending to log: %s\n", buff);
		memset(&logaddr,0,sizeof(logaddr));
		// log server attributes
		logaddr.sin_addr.s_addr = INADDR_ANY;
		logaddr.sin_port = htons(PORT);
		logaddr.sin_family = AF_INET;

		if(logfd = socket(AF_INET,SOCK_DGRAM,0) < 0){
			printf("socket failed");
			exit(5);
		}
		// send client message
		sendto(logfd,buff,sizeof(buff),MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
		// sent ip address of client
		sendto(logfd,ip,sizeof(ip)+1,MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
		close(logfd);
		printf("Done sending!\n");
		// if msg contains "Exit" then server exit and chat ended.
		
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main(int argc, char **argv)
{
	// get and set ports
	if(argc < 2 || argc > 4){
		printf("Invalid number of ports (1-3 only)");
		exit(1);
	}
	int port1, port2, port3;
	if((port1 = atoi(argv[1])) == NULL){
		printf("Invalid port number(s)");
		exit(2);
	}
	if(argc > 2){
		if((port2 = atoi(argv[2])) == NULL){
			printf("Invalid port number(s)");
			exit(2);
	}
	
	}
	if(argc>3){
		if((port3 = atoi(argv[3])) == NULL){
			printf("Invalid port number(s)");
			exit(2);
	}
	
	}
	int sockfd, listenfd, sockfd2, listenfd2, sockfd3, listenfd3, logfd, connfd, maxfd, maxfd2, maxfd3, isready, childpid;
	char buffer[MAX];
	char buffer2[MAX];
	char *message = "Hello htere";
	unsigned int len, lenu, loglen;
	struct sockaddr_in servaddr,servaddru, cli, cli2, cli3, cliaddr, logaddr;
	char* ip;
	fd_set rset, rset2, rset3;
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port1);
	
	// udp socket
	listenfd = socket(AF_INET, SOCK_DGRAM,0);
	if (listenfd == -1){
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddru, sizeof(servaddru));
	// udp ports, IP
	servaddru.sin_family = AF_INET;
	servaddru.sin_port = htons(port1);
	servaddru.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");
	// bind udp
	bind(listenfd, (struct sockaddr*)&servaddru, sizeof(servaddru));

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	if(argc > 2){
		sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd2 == -1) {
			printf("socket creation failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));

		// assign IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(port2);
	
		// udp socket
		listenfd2 = socket(AF_INET, SOCK_DGRAM,0);
		if (listenfd2 == -1){
			printf("socket creation failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully created..\n");
		bzero(&servaddru, sizeof(servaddru));
	
		servaddru.sin_family = AF_INET;
		servaddru.sin_port = htons(port2);
		servaddru.sin_addr.s_addr = htonl(INADDR_ANY);
	
		// Binding newly created socket to given IP and verification
		if ((bind(sockfd2, (SA*)&servaddr, sizeof(servaddr))) != 0) {
			printf("socket bind failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully binded..\n");
		// bind udp
		bind(listenfd2, (struct sockaddr*)&servaddru, sizeof(servaddru));
	
		// Now server is ready to listen and verification
		if ((listen(sockfd2, 5)) != 0) {
			printf("Listen failed...\n");
			exit(0);
		}
		else
			printf("Server listening..\n");
		if (sockfd2 > listenfd2)
			maxfd2 = sockfd2;
		else
			maxfd2 = listenfd2;
	}
	if(argc > 3){
		sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd3 == -1) {
			printf("socket creation failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));

		// assign IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(port3);
	
		// udp socket
		listenfd3 = socket(AF_INET, SOCK_DGRAM,0);
		if (listenfd3 == -1){
			printf("socket creation failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully created..\n");
		bzero(&servaddru, sizeof(servaddru));
	
		servaddru.sin_family = AF_INET;
		servaddru.sin_port = htons(port1);
		servaddru.sin_addr.s_addr = htonl(INADDR_ANY);
	
		// Binding newly created socket to given IP and verification
		if ((bind(sockfd3, (SA*)&servaddr, sizeof(servaddr))) != 0) {
			printf("socket bind failed...\n");
			exit(0);
		}
		else
			printf("Socket successfully binded..\n");
		// bind udp
		bind(listenfd3, (struct sockaddr*)&servaddru, sizeof(servaddru));
	
		// Now server is ready to listen and verification
		if ((listen(sockfd3, 5)) != 0) {
			printf("Listen failed...\n");
			exit(0);
		}
		else
			printf("Server listening..\n");
		if (sockfd3 > listenfd3)
			maxfd3 = sockfd3;
		else
			maxfd3 = listenfd3;
	}
	// can be both udp or tcp, must check
	FD_ZERO(&rset);
	FD_ZERO(&rset2);
	FD_ZERO(&rset3);
	if(listenfd > sockfd)
		maxfd = listenfd;
	else
		maxfd = sockfd;
	for(;;){
		FD_SET(sockfd, &rset);
		FD_SET(listenfd,&rset);
		// bloking call
		isready = select(maxfd+1, &rset, NULL, NULL, NULL);
		//udp
		if(FD_ISSET(listenfd,&rset)){
			lenu = sizeof(cliaddr);
			bzero(buffer,sizeof(buffer));
			printf("\nMessage from client: ");
			int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr,&lenu);
			buffer[n] = '\0';
			printf("%s\n", buffer);
			// send message received back to client for echo
			sendto(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
			// send message and IP to log as well
			printf("Sending to log: %s\n", buffer);
			memset(&logaddr,0,sizeof(logaddr));
			// log server
			logaddr.sin_addr.s_addr = INADDR_ANY;
			logaddr.sin_port = htons(PORT);
			logaddr.sin_family = AF_INET;

			//udp socket
			logfd = socket(AF_INET,SOCK_DGRAM,0);
			ip = inet_ntoa(cliaddr.sin_addr);
			// message
			sendto(logfd,buffer,sizeof(buffer),MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
			// ip
			sendto(logfd,ip,sizeof(ip)+1,MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
			close(logfd);
			printf("Done sending!\n");
		}
		else if(FD_ISSET(sockfd,&rset)){
			len = sizeof(cli);
			connfd = accept(sockfd,(struct sockaddr*)&cli,&len);
			ip = inet_ntoa(cli.sin_addr);
			// allow subprocess to handle looped communication
			if((childpid = fork()) == 0){
				func(connfd,ip);
				exit(0);
			}
			close(sockfd);
		}
		if(argc > 2){
			FD_SET(sockfd2, &rset2);
			FD_SET(listenfd2,&rset2);
			isready = select(maxfd2+1, &rset2, NULL, NULL, NULL);
			//udp
			if(FD_ISSET(listenfd2,&rset2)){
				lenu = sizeof(cliaddr);
				bzero(buffer,sizeof(buffer));
				printf("\nMessage from client: ");
				int n = recvfrom(listenfd2, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr,&lenu);
				buffer[n] = '\0';
				printf("%s\n", buffer);
				//puts(buffer);
				sendto(listenfd2, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
				printf("Sending to log: %s\n", buffer);
				memset(&logaddr,0,sizeof(logaddr));
				logaddr.sin_addr.s_addr = INADDR_ANY;
				logaddr.sin_port = htons(PORT);
				logaddr.sin_family = AF_INET;
	
				logfd = socket(AF_INET,SOCK_DGRAM,0);
				//bind(logfd, (struct sockaddr*)&logaddr,sizeof(logaddr));
				ip = inet_ntoa(cliaddr.sin_addr);
				sendto(logfd,buffer,sizeof(buffer),MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
				sendto(logfd,ip,sizeof(ip)+1,MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
				close(logfd);
				printf("Done sending!\n");
			}
			else if(FD_ISSET(sockfd2,&rset2)){
				printf("Going to connect\n");
				len = sizeof(cli2);
				ip = inet_ntoa(cli2.sin_addr);
				connfd = accept(sockfd2,(struct sockaddr*)&cli2,&len);
				if((childpid = fork()) == 0){
					func(connfd,ip);
					exit(0);
				}
				close(sockfd2);
			}
		}
		if(argc > 3){
			FD_SET(sockfd3, &rset3);
			FD_SET(listenfd3,&rset3);
			isready = select(maxfd3+1, &rset3, NULL, NULL, NULL);
			//udp
			if(FD_ISSET(listenfd3,&rset3)){
				lenu = sizeof(cliaddr);
				bzero(buffer,sizeof(buffer));
				printf("\nMessage from client: ");
				int n = recvfrom(listenfd3, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr,&lenu);
				buffer[n] = '\0';
				printf("%s\n", buffer);
				//puts(buffer);
				sendto(listenfd3, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
				printf("Sending to log: %s\n", buffer);
				memset(&logaddr,0,sizeof(logaddr));
				logaddr.sin_addr.s_addr = INADDR_ANY;
				logaddr.sin_port = htons(PORT);
				logaddr.sin_family = AF_INET;
	
				logfd = socket(AF_INET,SOCK_DGRAM,0);
				//bind(logfd, (struct sockaddr*)&logaddr,sizeof(logaddr));
				ip = inet_ntoa(cliaddr.sin_addr);
				sendto(logfd,buffer,sizeof(buffer),MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
				sendto(logfd,ip,sizeof(ip)+1,MSG_CONFIRM,(struct sockaddr*)&logaddr,sizeof(logaddr));
				close(logfd);
				printf("Done sending!\n");
			}
			else if(FD_ISSET(sockfd3,&rset3)){
				len = sizeof(cli3);
				ip = inet_ntoa(cli3.sin_addr);
				connfd = accept(sockfd3,(struct sockaddr*)&cli3,&len);
				if((childpid = fork()) == 0){
					func(connfd,ip);
					exit(0);
				}
				close(sockfd3);
			}
		}
	}
}
