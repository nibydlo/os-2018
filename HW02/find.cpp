#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

const string INDENT = "     ";
const int CONDITIONS_COUNT = 5;
vector<bool> conditions(CONDITIONS_COUNT, false);
struct stat buf;

vector<string> output_vector;
/*
0 - inum
1 - name
2 - size
3 - nlinks
4 - ex
*/

int inum , nlinks;
long long size;
char size_type, * name = "", * path = "";
string slash, ex_path;

bool is_number(string &s)
{
	if (s.length() == 0) {
		return false;
	}
    for (int i = 0; i < s.size(); i++) {
	    if (!isdigit(s[i])) {
		    return false;
	    }
    }
    return true;
}

void help() {
	printf("Function \'find\' takes odd number of arguments:\n");
	printf("First argument is a path to search place\n");
	printf("\'find\' can take several options:\n");
	printf("\t\'-name\' takes a string which describes file name\n");
	printf("\t\'-size\' takes a char followed by a positive integer which describes size of file\n");
	printf("\t\t(+) - greater than size\n");
	printf("\t\t(-) - less than size\n");
	printf("\t\t(=) - equal to size\n");
	printf("\t\'-inum\' takes a positive integer which describes inode of file\n");
	printf("\t\'-nlink\' takes a positive integer which describes number of hardlinks\n");
	printf("\t\'-exec\' takes a string which describes a path to executable file\n");
}

int parser(int argc, char* argv[]) {


	if (argc % 2 == 1) {
		//cout << "run" << endl;
		return -1;
	}

	for (int i = 1; i < argc; ++i) {

		if (strcmp(argv[i], "-inum") == 0)
		{
			if (i == argc - 1) {
				cout << "no number for -inum" << endl;
				return -1; // error because no argument
			}
			inum = atoi(argv[++i]);
			conditions[0] = true;
		}
		else if (strcmp(argv[i], "-name") == 0)
		{
			if (i == argc - 1) {
				cout << "no name for -iname" << endl;
				return -1; // error because no argument
			}
			name = argv[++i];
			conditions[1] = true;
		}
		else if (strcmp(argv[i], "-size") == 0)
		{
			if (i == argc - 1) {
				cout << "no number for -size" << endl;
				return -1; // error because no argument
			}
			string temp = argv[++i], temp2(argv[i]);
			temp2.erase(0, 1);
			if (!is_number(temp2)) {
				cout << "run" << endl;
				return -1;
			}
			size = stoi(temp2);
			conditions[2] = true;
			size_type = argv[i][0];
			if (size_type != '+' && size_type != '-' && size_type != '=') {
				return -1;
			}
		}
		else if (strcmp(argv[i], "-nlinks") == 0)
		{
			if (i == argc - 1) {
				cout << "no number for -nlinks" << endl;
				return -1; // error because no argument
			}
			nlinks = atoi(argv[++i]);
			conditions[3] = true;
		}
		else if (strcmp(argv[i], "-exec") == 0)
		{
			if (i == argc - 1) {
				cout << "no path for -exec" << endl;
				return -1; // error because no argument
			}
			ex_path = argv[++i];
			conditions[4] = true;
		} else if (argv[i][0] != '-') {
			path = argv[i];
		}
	}
	//cout << "path = " << path << endl;
	//cout << "inum = " << inum << "  name = " << name << "  size_type = " << size_type << " size = " << size << " nlinks = " << nlinks << " ex_path = " << ex_path << endl;
	return 0;
}

bool direc_name(string smth)
{
	//cout << "run" << endl;
   	for (int i = 0; i < smth.length(); i++)
   	{
      	if (smth[i] == '.')
       	{
           	return false;
       	}
   	}
	//cout << "run2" << endl;
   	return true;
}

bool filter(const char * path, string cur_path) {

	//checking name
	if (conditions[1] && strcmp(path, name) != 0) {
		//cout << "path = " << path << " name = " << name << endl;
		return false;
	}

	//getting information
	lstat(path, &buf);

	//checking inode
	if (conditions[0] && buf.st_ino != inum) {
		//cout << "cond 0" << endl;
		return false;
	}

	//checking size
	if (conditions[2]) {
		//cout << "cond 2" << endl;
		if (size_type == '+' && buf.st_size <= size) {
			return false;
		}
		if (size_type == '=' && buf.st_size != size) {
			return false;
		}
		if (size_type == '-' && buf.st_size >= size) {
			return false;
		}
	}

	//checking nlinks
	if (conditions[3] && buf.st_nlink != nlinks) {
		//cout << "cond 3" << endl;
		return false;
	}

	//checking if folder
	if (S_ISDIR(buf.st_mode)) {
		//cout << "check if dir" << endl;
		//cout << "buf st mode is " << buf.st_mode << "   S_IFDIR = " << S_IFDIR << endl;
		//cout << path << endl;

		DIR *dir;
		dir = opendir(cur_path.c_str());
		bool flag = false;
		if (dir) {
			flag = true;;
		}
		closedir(dir);
		if (flag) {
			return false;
		}
	}

	//cout << "re true" << endl;
	return true;
}

void outputFiles(string cur_path) {
	DIR *dir;
	struct dirent *entry;
	dir = opendir(cur_path.c_str());

	if (!dir) {
		//perror("diropen");
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{

		int lol = lstat(entry->d_name, &buf);
		string str_d_name(entry->d_name);
		string new_cur_path = cur_path + slash + str_d_name;
		//cout << entry->d_name << endl;
		if (strcmp(entry->d_name, ".") != 00 && strcmp(entry->d_name, "..") != 0 && filter(entry->d_name, new_cur_path)) {
			//cout << entry->d_name << endl;
			output_vector.push_back(new_cur_path);
		}
		if (direc_name(entry->d_name)) {
			outputFiles(new_cur_path);
		}
	}

	closedir(dir);
}

int executor(string s, char *arge[]) {

	istringstream ist(s);
	vector<string> v;
	string buf;

	while (ist >> buf) {
		v.push_back(buf + '\0');
	}

	char* act = new char[v[0].size()];
	char** executor_args = new char*[v.size() + 1];
	strcpy(act, v[0].c_str());

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

int main(int argc, char* argv[], char* arge[])
{
	slash = "/";

	int status = parser(argc, argv);
	if (status == -1)
	{
		help();
	} else {
		outputFiles(path);
		if (!conditions[4]) {


			for (auto elem : output_vector) {
				cout << elem << endl;
			}
		} else {
			string new_arg;
			for (int i = 0; i < output_vector.size(); i++) {
				new_arg = ex_path + " " + output_vector[i] + "\n";
				const pid_t pt = fork();
				if (pt == -1) {
					cout << strerror(errno) << endl;
					return -1;
				} else if (pt == 0) {
					int result = executor(new_arg, arge);
					return result;
				}
			}
		}
	}

	return 0;
}
