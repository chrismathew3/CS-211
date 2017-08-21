all: y86emul

y86emul: y86emul.c y86emul.h
	gcc -o y86emul y86emul.c 

test:
	./y86emul prog1.y86.txt
	./y86emul prog2.y86.txt

clean: 
	rm -f y86emul
	rm -f *.o