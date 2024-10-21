#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>

class Allocator {
public:
    Allocator();
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    // Это деструктор!
    ~Allocator();

private:
    size_t userSize;
    size_t offset;
    char* startPtr;
};

#endif //ALLOCATOR_H
