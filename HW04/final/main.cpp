#include <iostream>
#include <dlfcn.h>
#include "staticlib.h"
#include "dynamiclib1.h"

using namespace std;

int main() {
	print_hello_static();
	cout << endl;
	cout << "the sum of 1 and 2 is " << sum(1, 2) << endl;
	cout << "the sub from 2 1 is " << sub(2, 1) << endl;

 	void * dyn = dlopen("./dynamiclib2.so", RTLD_LAZY);
	if (!dyn) {
		cout << "Problem with opening library dynamiclib2.so " << dlerror() << endl;
		dlclose(dyn);
		return 1;
	}

	typedef int (*mult)(int, int);
	mult fun = (mult)dlsym(dyn, "mult");
	if (!fun) {
		cout << "Problem with opening function " << dlerror() << endl;
		return 2;
	}
	cout << "4 * 3 = " << (*fun)(4, 3) << endl;
	dlclose(dyn);
	return 0;
}
