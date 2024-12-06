#include <gtest/gtest.h>
#include <sstream>
#include "serializer.h"
#include "deserializer.h"
#include "data.h"

TEST(SerializerTest, SimpleSerializationDeserialization) {
    Data x{1, true, 2};
    std::stringstream stream;

    Serializer serializer(stream);
    EXPECT_EQ(serializer.save(x), Error::NoError);

    Data y{0, false, 0};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::NoError);

    EXPECT_EQ(x.a, y.a);
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.c, y.c);
}

TEST(SerializerTest, MaxUintSerializationDeserialization) {
    Data x{UINT64_MAX, false, UINT64_MAX};
    std::stringstream stream;

    Serializer serializer(stream);
    EXPECT_EQ(serializer.save(x), Error::NoError);

    Data y{0, true, 0};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::NoError);

    EXPECT_EQ(x.a, y.a);
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.c, y.c);
}

TEST(SerializerTest, MinUintSerializationDeserialization) {
    Data x{1, true, 2};
    std::stringstream stream;

    Serializer serializer(stream);
    EXPECT_EQ(serializer.save(x), Error::NoError);

    Data y{UINT64_MAX, false, UINT64_MAX};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::NoError);

    EXPECT_EQ(x.a, y.a);
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.c, y.c);
}

TEST(SerializerTest, UncorrectDeserialization) {
    std::stringstream stream("11 123");

    Data y{0, false, 0};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::CorruptedArchive);
}

TEST(SerializerTest, ExtraDataDeserialization) {
    std::stringstream stream("11 true 123 123123123");

    Data y{0, false, 0};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::NoError);

    EXPECT_EQ(y.a, 11);
    EXPECT_EQ(y.b, true);
    EXPECT_EQ(y.c, 123);
}

TEST(SerializerTest, EmptyDeserialization) {
    std::stringstream stream("");

    Data y{0, false, 0};
    Deserializer deserializer(stream);
    EXPECT_EQ(deserializer.load(y), Error::CorruptedArchive);
}

TEST(SerializerTest, FormatOutput) {
    std::stringstream stream;

    Data x{123, false, 1234};
    Serializer serializer(stream);
    EXPECT_EQ(serializer.save(x), Error::NoError);

    EXPECT_EQ(stream.str(), "123 false 1234");
}


int main(int argc, char** argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}