#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int status_p;

int execute(std::string& s) {
    std::istringstream ist(s);
    std::vector<std::string> r;
    std::string word;

    while(ist >> word) {
        r.push_back(word + '\0');
    }
    char* command = new char[r[0].size()];
    char **arguments = new char*[r.size() + 1];
    char *envp[] = {NULL};
    strcpy(command, r[0].c_str());

    for(size_t i = 0; i < r.size(); i++) {
        arguments[i] = new char[r[i].size()];
        for(size_t j = 0; j < r[i].size(); j++) {
            arguments[i][j] = r[i][j];
        }
    }

    arguments[r.size()] = 0;

    return execve(command, arguments,envp);
}

int main(int argv, char * args[]) {

    while(true) {
        std::string input;
        getline(std::cin, input);
        if(std::cin.eof()) {
            std::cout<<"EOF\n";
            return 0;
        }
        const pid_t p = fork();
        if(p == -1) {
            std::cout << "can't fork\n";
            return -1;
        } else if(p == 0) {
            int key = execute(input);
            return key;
        }
        if(wait(&status_p) < 0) {
            return -1;
        }

        std::cout << "child process return " << status_p << "\n";

        if(status_p < 0) {
            return -1;
        }
    }
    return 0;
}
