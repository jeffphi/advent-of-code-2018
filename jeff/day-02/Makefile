#
# Taken from https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
# 

CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
 
CFLAGS := -g  -Wall
# LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
INC := -I include

all: part-1 part-2

part-1: part-1.cpp
	g++ $(CFLAGS) -o part-1 part-1.cpp ../common/lib/example.a

part-2: part-2.cpp
	g++ $(CFLAGS) -o part-2 part-2.cpp ../common/lib/example.a

clean:
	rm -rfv *.o *.dSYM part-1 part-2

.PHONY: clean
