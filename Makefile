SRCS = $(wildcard *.cc)
CPP = g++
CPPFLAGS = -std=c++1z 

osecpu: $(SRCS) Makefile
	$(CPP) $(CPPFLAGS) -o osecpu $(SRCS)
