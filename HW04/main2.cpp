#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include "staticlib.h"
#include "dynamiclib1.h"
using namespace std;
int main() {

	void * dyn = dlopen("dynamiclib2.so", RTLD_LAZY);
	if (!dyn) {
		cout << "Problem with opening library dynamiclib2.so" << endl;
		return 1;
	}

	typedef int (*mult)(int, int);
	mult fun = (mult)dlsym(dyn, "mult");
	cout << "4 * 3 = " << (*fun)(4, 3) << endl;
	return 0;
}
