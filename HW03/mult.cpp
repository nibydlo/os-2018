#include <stdlib.h>
using namespace std;

int inc(int a) {
	return a + 1;
}

int dec(int a) {
	return a - 1;
}

int main(int argc, char* argv[]) {
	return inc(atoi(argv[1]));
}
