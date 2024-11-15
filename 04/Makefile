
CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wpedantic
FLAGS_ASAN=-fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

all: bigint_test.o bigint.o
	$(CC) $(FLAGS) $(FLAGS_ASAN) bigint.o bigint_test.o -o bigint $(FLAGS_TEST)

valgrind: bigint_test.o bigint.o
	$(CC) $(FLAGS) bigint.o bigint_test.o -o bigint $(FLAGS_TEST)

bigint.o : bigint.cpp
	$(CC) $(FLAGS) -c bigint.cpp

bigint_test.o: bigint_test.cpp
	$(CC) $(FLAGS) -c bigint_test.cpp

test:
	./bigint

clean:
	rm -f *.o bigint