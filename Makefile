F = gcc -lssl -lcrypto -ggdb -Wall
G = -fsanitize=address


all: main

main: main.o encryption.o rsa.o protected.o signature.o key.o cellKey.o cellProtected.o hashCell.o block.o cellTree.o vote.o
	$(F) -lm -o main bin/main.o bin/encryption.o bin/rsa.o bin/protected.o bin/signature.o bin/key.o bin/cellKey.o bin/cellProtected.o bin/hashCell.o bin/block.o bin/cellTree.o bin/vote.o

main.o:  src/main.c src/lib/headers/block.h src/lib/headers/encryption.h src/lib/headers/cellTree.h src/lib/headers/vote.h
	$(F) -c src/main.c -o bin/main.o

encryption.o: src/lib/encryption.c src/lib/headers/rsa.h
	$(F) -c src/lib/encryption.c -o bin/encryption.o

rsa.o: src/lib/rsa.c
	$(F) -c src/lib/rsa.c -o bin/rsa.o

protected.o: src/lib/protected.c src/lib/headers/signature.h
	$(F) -c src/lib/protected.c -o bin/protected.o

signature.o: src/lib/signature.c src/lib/headers/key.h
	$(F) -c src/lib/signature.c -o bin/signature.o

key.o: src/lib/key.c
	$(F) -c src/lib/key.c -o bin/key.o

cellKey.o: src/lib/cellKey.c
	$(F) -c src/lib/cellKey.c -o bin/cellKey.o

cellProtected.o: src/lib/cellProtected.c
	$(F) -c src/lib/cellProtected.c -o bin/cellProtected.o

hashCell.o: src/lib/hashCell.c
	$(F) -c src/lib/hashCell.c -o bin/hashCell.o

block.o: src/lib/block.c
	$(F) -c src/lib/block.c -o bin/block.o

vote.o: src/lib/vote.c
	$(F) -c src/lib/vote.c -o bin/vote.o

cellTree.o: src/lib/cellTree.c
	$(F) -c src/lib/cellTree.c -o bin/cellTree.o

clean :
	rm bin/*.o
	rm main
