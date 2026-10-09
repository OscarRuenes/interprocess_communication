// Title: log_s
// Author: Oscar Ruenes Campos
// Date: 5/6/2024
// Desc: This log server keeps track of messages from client machines into the echo_s server noting their date, message, and ip address
// and stores that into echo.log
#include <stdio.h>
#include <bits/stdc++.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 9000
#define MAXLINE 1000

using namespace std;
// Driver code
int main()
{
	char buffer[100];
	char buff[100];
	char *message = (char *) "Hello Client";
	int logsock;
	unsigned int len;
	struct sockaddr_in servaddr, cliaddr;

	// Create a UDP Socket
	if((logsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("bad socket");
		exit(2);
	}		
	// clean slate
	memset(&servaddr,0,sizeof(servaddr));
	memset(&cliaddr,0,sizeof(cliaddr));
	// set the server attributes
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;

	
	// bind server address to socket descriptor
	if (bind(logsock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		printf("no bind");
		exit(3);
	}
	
	printf("Log server is listening\n");
	for(;;){
		len = sizeof(cliaddr);
		// receive the message from the server
		bzero(buffer,sizeof(buffer));
		int n = recvfrom(logsock,(char*)buffer,MAXLINE,MSG_WAITALL,(struct sockaddr*)&cliaddr,&len);
		buffer[n] = '\0';
		// receive the IP address from the server
		bzero(buff,sizeof(buff));
		int j = recvfrom(logsock,(char*)buff,MAXLINE,MSG_WAITALL,(struct sockaddr*)&cliaddr,&len);
		buff[j+1] = '\0';
		// put required contents into echo.log
		ofstream logs;
		logs.open("echo.log",std::ios_base::app);
		time_t t = time(NULL);
		struct tm date = *localtime(&t);
		logs << (date.tm_year + 1900);
		logs << "-";
		if(date.tm_mon+1 < 10)
			logs << "0";
		logs << (date.tm_mon + 1);
		logs << "-";
		if(date.tm_mday < 10)
			logs << "0";
		logs << date.tm_mday;
		logs << " ";
		if(date.tm_hour < 10)
			logs << "0";
		logs << date.tm_hour;
		logs << ":";
		if(date.tm_min < 10)
			logs << "0";
		logs << date.tm_min;
		logs << ":";
		if(date.tm_sec < 10)
			logs << "0";
		logs << date.tm_sec;
		logs << "\t";
		logs << "\"";
		for(int i = 0; i < sizeof(buffer)/sizeof(buffer[0]); i++){
			if(buffer[i] == 10 || buffer[i] == '\0')
				break;
			logs << buffer[i];
		}
		logs << "\" was received from ";
		for(int i = 0; i < sizeof(buff)/sizeof(buff[0]); i++){
			if(buff[i] == 10 || buff[i] == '\0')
				break;
			logs << buff[i];
		}
		logs << "\n";
		logs.close();
	}
}

