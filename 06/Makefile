
CC=g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -Wpedantic
FLAGS_ASAN=-fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

TARGET=format_string.o
TARGET_TEST=format_string_test.o

all: $(TARGET) $(TARGET_TEST)
	$(CC) $(CFLAGS) $(FLAGS_ASAN) $(TARGET) $(TARGET_TEST) -o format_string $(FLAGS_TEST)

valgrind: $(TARGET) $(TARGET_TEST)
	$(CC) $(CFLAGS) $(TARGET) $(TARGET_TEST) -o format_string $(FLAGS_TEST)

$(TARGET) : format_string.cpp
	$(CC) $(CFLAGS) -c format_string.cpp

$(TARGET_TEST): format_string_test.cpp
	$(CC) $(CFLAGS) -c format_string_test.cpp

test:
	./format_string

clean:
	rm -f *.o format_string