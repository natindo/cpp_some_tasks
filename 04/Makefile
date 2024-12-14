
CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wpedantic
FLAGS_ASAN=-fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

TARGET=bigint.o
TARGET_TEST=bigint_test.o

all: $(TARGET) $(TARGET_TEST)
	$(CC) $(FLAGS) $(FLAGS_ASAN) $(TARGET) $(TARGET_TEST) -o bigint $(FLAGS_TEST)

valgrind: $(TARGET) $(TARGET_TEST)
	$(CC) $(FLAGS) $(TARGET) $(TARGET_TEST) -o bigint $(FLAGS_TEST)

$(TARGET) : bigint.cpp
	$(CC) $(FLAGS) -c bigint.cpp

$(TARGET_TEST): bigint_test.cpp
	$(CC) $(FLAGS) -c bigint_test.cpp

test: all
	./bigint

clean:
	rm -f *.o bigint