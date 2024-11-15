make clean
make valgrind
valgrind --leak-check=full ./bigint
make clean
