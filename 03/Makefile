
CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wpedantic -fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

TARGET=matrix.o
TARGET_TEST=matrix_test.o

all: $(TARGET) $(TARGET_TEST)
	$(CC) $(FLAGS) $(TARGET) $(TARGET_TEST) -o matrix $(FLAGS_TEST)

$(TARGET): matrix.cpp
	$(CC) $(FLAGS) -c matrix.cpp

$(TARGET_TEST): matrix_test.cpp
	$(CC) $(FLAGS) -c matrix_test.cpp

test: all
	./matrix

clean:
	rm -f *.o matrix