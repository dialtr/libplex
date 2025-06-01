AR=ar
CXX=g++
CXXFLAGS=-Wall -Werror -g -std=c++17
RANLIB=ranlib

.PHONY:
all: plexgdm.a demo

.PHONY:
clean:
	-rm -f plexgdm.a demo *.o

demo: demo.o plexgdm.a
	$(CXX) $(CXXFLAGS) -o demo $^

plexgdm.a: gdm.o
	$(AR) rcs plexgdm.a $^
	$(RANLIB) plexgdm.a

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

