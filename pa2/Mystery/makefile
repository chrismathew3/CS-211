COMPILER = gcc
CCFLAGS = -m64 -g -Wall

mystery: mystery.h mystery.c
	$(COMPILER) $(CCFLAGS) -o mystery mystery.c
	$(COMPILER) -m32 -S mystery.c
test: 
	./mystery 0
	./mystery 1
	./mystery -1
	./mystery 5.5
	./mystery 65
clean:
	rm -f mystery 
	rm -f *.o