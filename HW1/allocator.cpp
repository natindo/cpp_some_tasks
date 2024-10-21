#include "allocator.h"

Allocator::Allocator()
{
    userSize = 0;
    offset = 0;
    startPtr = nullptr;
}

void Allocator::makeAllocator(size_t maxSize)
{
    if (startPtr != nullptr) {
        delete[] startPtr;
    }
    if (maxSize == 0) {
        startPtr = nullptr;
        userSize = 0;
        offset = 0;
        return;
    }
    startPtr = new char[maxSize];
    userSize = maxSize;
    offset = 0;
}

char* Allocator::alloc(size_t size)
{
    if (startPtr == nullptr || offset + size > userSize || size == 0) {
        return nullptr;
    }
    char* ret = startPtr + offset;
    offset += size;
    return ret;
}

void Allocator::reset()
{
    offset = 0;
}

Allocator::~Allocator()
{
    delete[] startPtr;
}
