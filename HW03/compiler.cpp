#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char* argv[]) {

	//code of inc function
	unsigned char mash_code[] = {0x55, 0x48, 0x89, 0xe5, 0x89, 0x7d, 0xfc, 0x8b, 0x45, 0xfc, 0x83, 0xc0, 0x01, 0x5d, 0xc3};

	if (argc != 3) {
		std::cout << "exprected <run> <int number> <+/-> (+ for inc, - for dec)" << std::endl;
		return 0;
	}

	if (argv[2][0] == '-') {
		mash_code[11] = 0xe8;
	}
	void * new_memory = mmap(NULL, sizeof (mash_code), PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	memcpy(new_memory, mash_code, sizeof(mash_code));
	int c = ((int(*)(int))new_memory) (atoi(argv[1]));
	int check_error = munmap(new_memory, sizeof(mash_code));
	if (check_error == -1) {
		 std::cout << "error in mmap" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "result of oper: " << c << std::endl;
}
