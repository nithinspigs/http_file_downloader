#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// connect socket to website (ip address) specified in argument
// use send() function to send GET message
// use recv() function to receive the response
// wait no use fgets() to get all of the header lines
// then when you run into '\r\n', use freads() to read the actual file
// oh right we don't need recv() cuz we're using fdopen() to wrapper the bish

static void die(const char *message) {
	perror(message);
	exit(1);
}

int main(int argc, char **argv) {

	if(argc != 4) {
		fprintf(stderr, "%s\n", "usage: http-client <host> <port_number> <file_path>");
		exit(1);
	}

	/* 
	 * Connect client to host server through socket
	 */

	struct hostent *he;
	char *serverName = argv[1];
	// get server ip from server name
	if((he = gethostbyname(serverName)) == NULL) {
		die("gethostbyname failed");
	}
	char *serverIP = inet_ntoa(*(struct in_addr *)he->h_addr);
	
	unsigned short port = atoi(argv[2]);

	// Create a socket for TCP connection

	int sock; // socket descriptor
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("socket failed");

	// Construct a server address structure

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr)); // must zero out the structure
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(serverIP);
	servaddr.sin_port        = htons(port); // must be in network byte order

	// Establish a TCP connection to the server

	if(connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		die("connect failed");

	FILE *input = fdopen(sock, "r");
	if(input == NULL)
		die("client socket could not be opened as a file");

	char *filename = strrchr(argv[3], '/');
	// printf("%s\n", filename + 1);

	// make get_msg
	char get_msg[8192] = "GET ";
	int n = snprintf(get_msg, sizeof(get_msg), "GET %s HTTP/1.0\r\nHost: %s:%s\r\n\r\n", argv[3], argv[1], argv[2]);
	if(n < 0)
		die("could not make get request message");

	/*
	strcat(get_msg, argv[3]);
	strcat(get_msg, " HTTP/1.0\r\n");
	strcat(get_msg, "Host: ");
	strcat(get_msg, argv[1]);
	strcat(get_msg, ":");
	strcat(get_msg, argv[2]);
	strcat(get_msg, "\r\n\0");
	*/

	// printf("%s", get_msg);

	if(send(sock, get_msg, strlen(get_msg), 0) != strlen(get_msg))
		die("send failed");
	

	char status_line[4096];
	if(fgets(status_line, sizeof(status_line), input) == NULL) 
		die("failed to read status line");
	// printf("%s", status_line);

	if(!strstr(status_line, "200") || !(strstr(status_line, "HTTP/1.0") || strstr(status_line, "HTTP/1.1")))
		die(status_line);
	
	FILE *output = fopen(filename + 1, "wb");
	char header_line[4096];
	while(strcmp(fgets(header_line, sizeof(header_line), input), "\r\n") != 0) { continue; } // skip through headers

	// buf may be a char array, but it is NOT a string
	// char array just means it holds 4096 elements, 1 byte each
	// why is my output file always 0 < x < 4096 bytes shorter than the output file from jae's executable?
	char buf[4096];
	int num_bytes_read;
	while((num_bytes_read = fread(buf, 1, sizeof(buf), input)))
		fwrite(buf, 1, num_bytes_read, output);
	fwrite(buf, 1, num_bytes_read, output);

	fclose(input);
	fclose(output);	

}
