
CC=g++
FLAGS=-std=c++20 -Wall -Werror -Wextra -Wpedantic
FLAGS_ASAN=-fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

TARGET_TEST=serializer_test.o

all: $(TARGET_TEST)
	$(CC) $(FLAGS) $(FLAGS_ASAN) $(TARGET_TEST) -o serializer $(FLAGS_TEST)

valgrind: $(TARGET_TEST)
	$(CC) $(FLAGS) $(TARGET_TEST) -o serializer $(FLAGS_TEST)

$(TARGET_TEST): serializer_test.cpp
	$(CC) $(FLAGS) -c serializer_test.cpp

test: all
	./serializer

clean:
	rm -f *.o serializer