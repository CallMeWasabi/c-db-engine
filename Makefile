SHELL := bash
CC = gcc
LIBS = -lhashmap -lvector

# main
run_main: build_main
	./build/bin/main

build_main: build_hashmap_lib
	$(CC) src/main.c -o build/bin/main -Lbuild/libs $(LIBS) -Iinclude

# test
run_test: build_test
	./build/bin/test

build_test: build_hashmap_lib build_vector_lib
	$(CC) test/test.c -o build/bin/test -Lbuild/libs $(LIBS) -Iinclude


# library
build_hashmap_lib:
	$(CC) -c src/hashmap.c -o build/object/hashmap.o
	ar rcs build/libs/libhashmap.a build/object/hashmap.o

build_vector_lib:
	$(CC) -c src/vector.c -o build/object/vector.o
	ar rcs build/libs/libvector.a build/object/vector.o
