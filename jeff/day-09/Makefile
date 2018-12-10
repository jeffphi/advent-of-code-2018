#
# Taken from https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
# 

CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
 
CFLAGS := -g  -Wall -std=c++11
# LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
INC := -I ../common/include

all: part-1 part-2

part-1: part-1.cpp
	g++ $(CFLAGS) $(INC) -o part-1 part-1.cpp ../common/lib/jefflib.a

part-2: part-2.cpp
	g++ $(CFLAGS) $(INC) -o part-2 part-2.cpp ../common/lib/jefflib.a

clean:
	rm -rfv *.o *.dSYM part-1 part-2

.PHONY: clean
