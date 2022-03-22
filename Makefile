all: main

main: main.o encryption.o rsa.o protected.o signature.o key.o cellKey.o cellProtected.o
	gcc -Wall -ggdb -lm -o main bin/main.o bin/encryption.o bin/rsa.o bin/protected.o bin/signature.o bin/key.o bin/cellKey.o bin/cellProtected.o

main.o:  src/main.c src/lib/headers/rsa.h src/lib/headers/encryption.h 
	gcc -Wall -c src/main.c -o bin/main.o

encryption.o: src/lib/encryption.c src/lib/headers/rsa.h
	gcc -ggdb -Wall -c src/lib/encryption.c -o bin/encryption.o

rsa.o: src/lib/rsa.c
	gcc -ggdb -Wall -c src/lib/rsa.c -o bin/rsa.o

protected.o: src/lib/protected.c src/lib/headers/signature.h
	gcc -ggdb -Wall -c src/lib/protected.c -o bin/protected.o

signature.o: src/lib/signature.c src/lib/headers/key.h
	gcc -ggdb -Wall -c src/lib/signature.c -o bin/signature.o

key.o: src/lib/key.c
	gcc -ggdb -Wall -c src/lib/key.c -o bin/key.o

cellKey.o: src/lib/cellKey.c
	gcc -ggdb -Wall -c src/lib/cellKey.c -o bin/cellKey.o

cellProtected.o: src/lib/cellProtected.c
	gcc -ggdb -Wall -c src/lib/cellProtected.c -o bin/cellProtected.o

clean :
	rm bin/*.o
	rm main
