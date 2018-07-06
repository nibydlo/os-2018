all: hello

hello: main.o sum.o hello.o sub.o mult.o
	g++ -shared -fPIC -o dynamiclib1.so sub.o
	g++ -shared -fPIC -o dynamiclib2.so mult.o
	g++ main.o sum.o hello.o sub.o -o HW04 -ldl
	ar crv myStaticLib.a sum.o hello.o
	g++ -o HW04 main.o myStaticLib.a ./dynamiclib1.so -ldl

main.o: main.cpp
	g++ -c main.cpp

sum.o: sum.cpp
	g++ -c sum.cpp

hello.o: hello.cpp
	g++ -c hello.cpp

sub.o: sub.cpp
	g++ -c sub.cpp

mult.o: mult.cpp
	g++ -c mult.cpp

clean:
	rm -rf *.o hello
