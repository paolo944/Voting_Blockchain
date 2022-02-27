all : main

main : main.o
	gcc -Wall -ggdb -lm -o main main.o

main.o : main.c
	gcc -Wall -c main.c

clean :
	rm main
	rm *.o