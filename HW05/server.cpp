#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
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
	address.sin_port = htons(303);
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

		for (;;) {
			char buff[1024];

			int received = 0;
			int sizeof_info[1];
			while (received != sizeof(sizeof_info)) {
				int rec_b_count = recv(accepted, sizeof_info + received, sizeof(sizeof_info) - received, 0);
				if(rec_b_count == -1) {
					cout << "problem with receiving message size" << endl;
					break;
				}
				received += rec_b_count;
			}

			received = 0;
			while(received < sizeof_info[0]) {
				int rec_b_count = recv(accepted, buff + received, sizeof_info[0] - received, 0);
			     if(rec_b_count == -1) {
					cout << "problem with receiving message" << endl;
					break;
				}
			     received += rec_b_count;
			}

			//send(accepted, buff, received, 0);
			int sended = 0;

		    	while(sended < sizeof_info[0]) {
		        	int send_b_count = send(accepted, buff+sended, received - sended, 0);
		        	if(send_b_count == -1) {
					cout << "problem with sending message" << endl;
					break;
				}
		        	sended += send_b_count;
		    	}

		}
		close(accepted);
	}

	return 0;
}
