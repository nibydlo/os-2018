/*#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

#define CHK(eval) if(eval < 0){perror("eval"); exit(-1);}

using namespace std;

const int MAX_EPOLL_EVENTS = 100;

int main() {

	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		perror("error with socket");
		return 1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(3237);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listener, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("error with bind");
		return 2;
	}

	listen(listener, 1);

	int epollfd = epoll_create1(0);
     if (epollfd == -1) {
     	perror("epoll_create1");
          exit(EXIT_FAILURE);
     }

	struct epoll_event events[MAX_EPOLL_EVENTS];
	struct epoll_event ev;
     ev.events = EPOLLIN;
     ev.data.fd = listener;
     if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev) == -1) {
          perror("epoll_ctl: listen_sock");
          exit(EXIT_FAILURE);
     }

	for(;;) {

		int nfds = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
          if (nfds == -1) {
              perror("epoll_wait");
              exit(EXIT_FAILURE);
	    }

		for (int n = 0; n < nfds; ++n) {
              if (events[n].data.fd == listener) {
                  int accepted = accept(listener,
                                     NULL, NULL);
                  if (accepted == -1) {
                      perror("accept");
                      exit(EXIT_FAILURE);
                  }

			   CHK(fcntl(accepted, F_SETFL, fcntl(accepted, F_GETFD, 0)|O_NONBLOCK));

			   ev.events = EPOLLIN | EPOLLET;
			   ev.data.fd = accepted;
			   if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener,
							  &ev) == -1) {
			    	  perror("epoll_ctl: conn_sock");
				  exit(EXIT_FAILURE);
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
          }
	}

	return 0;
}*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#define MAX_EVENTS 10

using namespace std;

int main() {

	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		perror("error with socket");
		return 1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(238);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listener, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("error with bind");
		return 2;
	}

	listen(listener, 1);

	int epollfd = epoll_create(4);
	if (epollfd == -1) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}

	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	ev.events  = EPOLLIN;
	ev.data.fd = listener;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev) == -1) {
		perror("epoll_ctl: listener");
		exit(EXIT_FAILURE);
	}

	for(;;) {

		int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == listener) {

				struct sockaddr_storage addr;
				socklen_t addrlen = sizeof(addr);

				int accepted = accept(listener, (struct sockaddr *) &addr, &addrlen);
				if (accepted == -1) {
					perror("accept");
					exit(EXIT_FAILURE);
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
		}

		/*int accepted = accept(listener, NULL, NULL);
		if (accepted < 0) {
			perror("error with accept");
			return 3;
		}*/


		//close(accepted);
	}

	return 0;
}
