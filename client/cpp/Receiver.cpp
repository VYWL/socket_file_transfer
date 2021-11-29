#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>

#define MAX_LINE 511
#define MAX_SIZE 1024

int tcp_connect(char *ip, int host);
void errquit(char* mesg) { perror(mesg); exit(0); }

int main(int argc, char* argv[])
{
	int sum = 0;

	int listen_sock;
	FILE* file;
	size_t fsize;
	std::string fileNm = argv[3];
	char buffer[MAX_SIZE];
	char filename[MAX_LINE], temp[5];
	
	if (argc != 5) {
		printf("사용법 : %s [server_ip] [port] [filename] [savepath]\n", argv[0]);
		exit(0);
	}

	listen_sock = tcp_connect(argv[1], atoi(argv[2]));
	
	if (listen_sock == -1)
		errquit("tcp_connect fail");

	send(listen_sock, fileNm.c_str(), fileNm.size(), 0);

	file = fopen(argv[4], "wb");

	int nbyte = MAX_SIZE;

	while (nbyte) {
		nbyte = recv(listen_sock, buffer, MAX_SIZE, 0);
		fwrite(buffer, sizeof(char), nbyte, file);
	}

	printf("Finished.\n");
	fclose(file);
}

int tcp_connect(char* ip, int port) {
	int sd;
	struct sockaddr_in servaddr;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		errquit("socket fail");
	}

	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);

	if (connect(sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		errquit("connect fail");
	}

	return sd;
}