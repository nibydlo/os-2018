#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace std;

int main() {

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("error with sockets");
		return 1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(201);
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("error with connection");
		return 2;
	}

	char info[] = "lolkek";
	int info_size = strlen(info);
	char * info_pointer = info;

	while (info_size > 0) {
		int sended = send(sock, info_pointer, info_size, 0);
		if (sended == -1) {
			perror("send");
			exit(EXIT_FAILURE);
		}
		info_pointer += sended;
		info_size -= sended;
	}

	shutdown(sock, SHUT_WR);

	char buf[1024];
	char * rec_pointer = buf;
	int rec_info = 0, rec_size = 0;

	while((rec_info = recv(sock, rec_pointer, 1024, 0)) > 0) {
		rec_pointer += rec_info;
		cout << buf << endl;
	}

	if (rec_info == -1) {
		perror(strerror(errno));
	}

	if (rec_info == 0) {
		perror(strerror(errno));
	}
	close(sock);

	return 0;
}
