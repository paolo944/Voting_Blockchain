all : main

main : main.o encryption.o rsa.o
	gcc -Wall -ggdb -lm -o main main.o encryption.o rsa.o

main.o : main.c rsa.h encryption.h
	gcc -Wall -c main.c

encryption.o : encryption.c rsa.h
	gcc -ggdb -Wall -c encryption.c

rsa.o : rsa.c
	gcc -ggdb -Wall -c rsa.c

clean :
	rm main
	rm *.o