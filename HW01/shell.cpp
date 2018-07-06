#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int executor(string& s, char *arge[]) {

	istringstream ist(s);
	vector<string> v;
	string buf;

	while (ist >> buf) {
		v.push_back(buf + '\0');
	}

	char* act = new char[v[0].size()];
	char** executor_args = new char*[v.size() + 1];
	strcpy(act, v[0].c_str());
	cout << "size = " << v.size() + 1 << endl;

	for(size_t i = 0; i < v.size(); i++) {
		executor_args[i] = new char[v[i].size()];
		for (size_t j = 0; j < v[i].size(); j++) {
			executor_args[i][j] = v[i][j];
		}
	}
	
	executor_args[v.size()] = 0;

	int result = execve(act, executor_args, arge);
	delete[] act;
	for (int i = 0; i < v.size(); i++) {
		delete[] executor_args[i];
	}
	delete[] executor_args;
	return result;
}

int main(int argv, char * args[], char* arge[]) {

 	cout << "shell is started" << endl;
	cout << "type exit to exit" << endl;
 	for(;;) {

		int stat;

   		string buf;
   		getline(cin, buf);
   		if (cin.eof() || buf == "exit") {
     		cout << "the end." << endl;
     		return 0;
		}

		const pid_t pt = fork();
		if (pt == -1) {
			cout << strerror(errno) << endl;
			return -1;
		} else if (pt == 0) {
			int result = executor(buf, arge);
			return result;
		}

		if (wait(&stat) < 0) {
			cout << strerror(errno) << endl;
			return -1;
		}

		cout << "executor return: " << stat << endl;

		if (stat < 0) {
			cout << strerror(errno) << endl;
			return -1;
		}
 	}

 	return 0;
}
