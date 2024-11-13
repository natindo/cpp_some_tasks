
CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wpedantic -fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

all: matrix_test.o matrix.o
	$(CC) $(FLAGS) matrix.o matrix_test.o -o matrix $(FLAGS_TEST)

matrix.o : matrix.cpp
	$(CC) $(FLAGS) -c matrix.cpp

matrix_test.o: matrix_test.cpp
	$(CC) $(FLAGS) -c matrix_test.cpp

test:
	./matrix

clean:
	rm -f *.o matrix