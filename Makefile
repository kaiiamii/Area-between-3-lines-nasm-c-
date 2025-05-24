all: program

program: task6.o functions.o
	gcc -m32 -no-pie task6.o functions.o -o program -lm

task6.o: task6.c
	gcc -m32 -g -Wall -c task6.c -o task6.o

functions.o: functions.asm
	nasm -f elf32 functions.asm -o functions.o

clean:
	rm -f *.o program