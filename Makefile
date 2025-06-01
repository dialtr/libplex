AR=ar
CXX=g++
CXXFLAGS=-Wall -Werror -g -std=c++17
RANLIB=ranlib

.PHONY:
all: libplex.a demo

.PHONY:
clean:
	-rm -f demo *.a *.o

demo: demo.o libplex.a
	$(CXX) $(CXXFLAGS) -o demo $^

.PHONY:
format:
	clang-format -style=Google -i *.cc *.h

.PHONY:
lint:
	cpplint *.cc *.h

libplex.a: gdm.o
	$(AR) rcs libplex.a $^
	$(RANLIB) libplex.a

.PHONY:
valgrind: demo
	valgrind --leak-check=full --track-fds=yes ./demo

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

