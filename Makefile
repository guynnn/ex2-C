GCC_FLAGS = gcc -Wall -Wextra -Wvla -DNDEBUG -std=c99

all: ListExample
	./ListExample

CheckDependency: CheckDependency.c
	$(GCC_FLAGS) -o CheckDependency CheckDependency.c

ListExample: ListExample.c MyLinkedList.o
	$(GCC_FLAGS) -o ListExample.o -c ListExample.c
	$(GCC_FLAGS) -o ListExample ListExample.o MyLinkedList.o

MyLinkedList.o: MyLinkedList.c MyLinkedList.h
	$(GCC_FLAGS) -o MyLinkedList.o -c MyLinkedList.c

CheckDependency.o: CheckDependency.c
	$(GCC_FLAGS) -o CheckDependency.o CheckDependency.c

clean:
	rm -f CheckDependency ListExample.o ListExample MyLinkedList.o CheckDependency.o

submit: valgrind
	tar -cvf ex2.tar CheckDependency.c MyLinkedList.c README Makefile valdbg_check_dependecy.out valdbg_list_example.out

