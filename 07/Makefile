
CC=g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -Wpedantic
FLAGS_ASAN=-fsanitize=address -fsanitize=leak
FLAGS_TEST=-lgtest_main -lgtest -lpthread

TARGET=format.o
TARGET_TEST=format_test.o

all: $(TARGET) $(TARGET_TEST)
	$(CC) $(CFLAGS) $(FLAGS_ASAN) $(TARGET) $(TARGET_TEST) -o format $(FLAGS_TEST)

valgrind: $(TARGET) $(TARGET_TEST)
	$(CC) $(CFLAGS) $(TARGET) $(TARGET_TEST) -o format $(FLAGS_TEST)

$(TARGET) : format.cpp
	$(CC) $(CFLAGS) -c format.cpp

$(TARGET_TEST): format_test.cpp
	$(CC) $(CFLAGS) -c format_test.cpp

test:
	./format

clean:
	rm -f *.o format