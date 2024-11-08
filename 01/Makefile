
CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wpedantic -fsanitize=address
FLAGS_TEST=-lgtest_main -lgtest -lpthread

all: allocator_test.o allocator.o
	$(CC) $(FLAGS) allocator_test.o allocator.o -o allocator $(FLAGS_TEST)

allocator.o : allocator.cpp
	$(CC) $(FLAGS) -c allocator.cpp

allocator_test.o: allocator_test.cpp
	$(CC) $(FLAGS) -c allocator_test.cpp

test:
	./allocator

clean:
	rm -f *.o allocator