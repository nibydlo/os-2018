#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <iostream>

using namespace std;

int main() {

	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		perror("error with socket");
		return 1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(201);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listener, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("error with bind");
		return 2;
	}

	listen(listener, 1);

	for(;;) {
		int accepted = accept(listener, NULL, NULL);
		if (accepted < 0) {
			perror("error with accept");
			return 3;
		}


			char buf[1024];

			char * rec_pointer = buf;
			int rec_info = 0, rec_size = 0;

			while((rec_info = recv(accepted, rec_pointer, 1024, 0)) > 0) {
				rec_pointer += rec_info;
				cout << buf << endl;
			}

			

			int info_size = strlen(buf);
			char * info_pointer = buf;

			while (info_size > 0) {
				int sended = send(accepted, info_pointer, info_size, 0);
				if (sended == -1) {
					perror("send");
					exit(EXIT_FAILURE);
				}
				info_pointer += sended;
				info_size -= sended;
			}
			//send(accepted, buff, info, 0);

		close(accepted);
	}

	return 0;
}
