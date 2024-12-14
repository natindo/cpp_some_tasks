
CC=g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -Wpedantic
FLAGS_ASAN=-fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

TARGET_TEST=format_string_test.o

all: $(TARGET_TEST)
	$(CC) $(CFLAGS) $(FLAGS_ASAN) $(TARGET_TEST) -o format_string $(FLAGS_TEST)

valgrind: $(TARGET_TEST)
	$(CC) $(CFLAGS) $(TARGET_TEST) -o format_string $(FLAGS_TEST)

$(TARGET_TEST): format_string_test.cpp
	$(CC) $(CFLAGS) -c format_string_test.cpp

test: all
	./format_string

clean:
	rm -f *.o format_string