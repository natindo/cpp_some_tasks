
CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wpedantic -fsanitize=address
FLAGS_TEST=-lgtest_main -lgtest -lpthread

all: parser_test.o parser.o
	$(CC) $(FLAGS) parser.o parser_test.o -o parser $(FLAGS_TEST)

allocator.o : parser.cpp
	$(CC) $(FLAGS) -c parser.cpp

parser_test.o: parser_test.cpp
	$(CC) $(FLAGS) -c parser_test.cpp

test:
	./parser

clean:
	rm -f *.o parser