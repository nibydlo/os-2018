#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

int main() {

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("error with sockets");
		return 1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(303);
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("error with connection");
		return 2;
	}

	char info[] = "lolkek";
	int sizeof_info[1] = { sizeof(info) };
	int sended = 0;
	while (sended != sizeof(sizeof_info)) {
		int send_b_count = send(sock, (char *) sizeof_info + sended, sizeof(sizeof_info) - sended, 0);
		if(send_b_count == -1) {
			cout << "problem with sending message" << endl;
			break;
		}
        	sended += send_b_count;
	}



	//sended = 0;
	cout << "run" << endl;
	char * info_pointer = info;
	int d_size = strlen(info);
    	while(0 < d_size) {
        	int sended = send(sock, info_pointer, d_size, 0);
        	if(sended == -1) {
			cout << "problem with sending message" << endl;
			break;
		}
		info_pointer += sended;
		d_size -= sended;
        	//sended += send_b_count;
    	}
	shutdown(sock, SHUT_WR);
	char buf[sizeof(info)];
	int received = 0;

	//int send_num = send(sock, info, sizeof(info), 0);
	while(received < sizeof(info)) {
		int rec_b_count = recv(sock, buf+received, sizeof(info) - received, 0);
	     if(rec_b_count == -1) {
			cout << "problem with receiving message" << endl;
			break;
		}
	     received += rec_b_count;
	}

	cout << buf << endl;
	close(sock);

	return 0;
}
