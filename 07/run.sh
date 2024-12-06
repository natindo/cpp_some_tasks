make
make test
make clean
make valgrind
valgrind --leak-check=full ./format
make clean
