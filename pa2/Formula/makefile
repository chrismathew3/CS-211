COMPILER = gcc
CCFLAGS = -ansi -pedantic -Wall
all: formula

formula: formula.h formula.c nCr.h nCr.s
	 $(COMPILER) $(CCFLAGS) -o formula formula.c	nCr.s
test:
	./formula 3
	./formula 9
	./formula 100
	./formula 1000
clean:
	rm -f formula
	rm -f *.o