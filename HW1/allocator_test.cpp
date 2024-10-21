#include "allocator.h"
#include <gtest/gtest.h>

TEST(allocator_test, simpleAllocationTest)
{
  Allocator allocator;
  allocator.makeAllocator(10);

  char* ptr1 = allocator.alloc(3);
  EXPECT_NE(nullptr, ptr1);

  char* ptr2 = allocator.alloc(5);
  EXPECT_NE(nullptr, ptr2);

  char* ptr3 = allocator.alloc(1);
  EXPECT_NE(nullptr, ptr3);
}

TEST(allocator_test, allocationTestAllocBeyondSize)
{
  Allocator allocator;
  allocator.makeAllocator(10);

  char* ptr1 = allocator.alloc(2);
  EXPECT_NE(nullptr, ptr1);

  char* ptr2 = allocator.alloc(7);
  EXPECT_NE(nullptr, ptr2);

  char* ptr3 = allocator.alloc(3);
  EXPECT_EQ(nullptr, ptr3);
}

TEST(allocator_test, allocationTestReset)
{
  Allocator allocator;
  allocator.makeAllocator(10);

  char* ptr1 = allocator.alloc(2);
  EXPECT_NE(nullptr, ptr1);

  allocator.reset();

  char* ptr2 = allocator.alloc(10);
  EXPECT_NE(nullptr, ptr2);
}

TEST(allocator_test, allocationTestMultimplyMakeAllocator)
{
  Allocator allocator;
  allocator.makeAllocator(10);

  char* ptr1 = allocator.alloc(2);
  EXPECT_NE(nullptr, ptr1);

  allocator.makeAllocator(50);

  char* ptr2 = allocator.alloc(40);
  EXPECT_NE(nullptr, ptr2);
}

TEST(allocator_test, allocationTestAllocationZeroSize)
{
  Allocator allocator;
  allocator.makeAllocator(0);

  char* ptr1 = allocator.alloc(2);
  EXPECT_EQ(nullptr, ptr1);
}

TEST(allocator_test, allocationTestAllocZeroSize)
{
  Allocator allocator;
  allocator.makeAllocator(10);

  char* ptr1 = allocator.alloc(0);
  EXPECT_EQ(nullptr, ptr1);
}

TEST(allocator_test, allocationTestAllocWirhoutMakeAllocator)
{
  Allocator allocator;

  char* ptr1 = allocator.alloc(0);
  EXPECT_EQ(nullptr, ptr1);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}