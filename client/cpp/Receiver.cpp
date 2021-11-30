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
#define CHECK_BUF_SIZE 4

int tcp_connect(char *ip, int host);
void errquit(char* mesg) { perror(mesg); exit(0); }

std::string buf2str(char* checker, int bufflen);
void validationStep(int listen_sock, std::string fileNm);
void fileDownloadStep(int listen_sock, std::string pathname);

int main(int argc, char* argv[])
{
	int listen_sock;
	
	std::string fileNm(argv[3]);
	std::string pathNm(argv[4]);
	
	if (argc != 5) {
		printf("사용법 : %s [server_ip] [port] [filename] [pathname]\n", argv[0]);
		exit(0);
	}

	listen_sock = tcp_connect(argv[1], atoi(argv[2]));
	
	if (listen_sock == -1) errquit("tcp_connect fail");

	validationStep(listen_sock, fileNm);

	fileDownloadStep(listen_sock, pathNm);

	std::cout << "[SUCCESS] \"" << fileNm << "\" Download Finished.\n";

	return 0;
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

void validationStep(int listen_sock, std::string fileNm)
{
	std::string errStr("00000000");
	char checker[4];

	// STEP 1 :: send filename to node server (filename size, max 511)
	send(listen_sock, fileNm.c_str(), fileNm.size(), 0);

	// STEP 2 :: receive check Result as Buffer from server (4 bytes);
	int nbyte = recv(listen_sock, checker, 4, 0);
	
	// STEP 3 :: Using buf2str function, get checkerString
	std::string checkerString = buf2str(checker, CHECK_BUF_SIZE);
	
	// STEP 4 :: Compare checkerString with errorString.
	// If same, this process will be shut down.
	if(checkerString == errStr) 
	{
		std::cout << "[ERROR] No File Exists.\n";
		exit(-1);
	}
}

void fileDownloadStep(int listen_sock, std::string pathname)
{
	FILE* file;
	char buffer[MAX_SIZE];

	// If file exists, start downloading file.
	// First, get File pointer with "wb" option and pathname(argv[4])
	file = fopen(pathname.c_str(), "wb");

	int nbyte = MAX_SIZE;

	while (nbyte) {
		nbyte = recv(listen_sock, buffer, MAX_SIZE, 0);
		fwrite(buffer, sizeof(char), nbyte, file);
	}

	fclose(file);
}

std::string buf2str(char* checker, int bufflen)
{
    std::string tempStr; 

	for(int i = 0; i < bufflen; ++i) {
		char buf[bufflen];

		snprintf(buf, bufflen, "%02x", checker[i]);

		tempStr += buf;
	}

	return tempStr;
}