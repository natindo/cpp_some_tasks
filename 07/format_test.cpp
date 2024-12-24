#include <gtest/gtest.h>
#include <string>
#include "format.h"

TEST(AVL_Test, EmptyAndSize) {
    avl<int, std::string> tree;

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0u);

    tree.insert({10, "Ten"});
    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(tree.size(), 1u);

    tree.clear();
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0u);
}

TEST(AVL_Test, InsertAndContains) {
    avl<int, std::string> tree;
    EXPECT_FALSE(tree.contains(10));
    
    auto ret1 = tree.insert({10, "Ten"});
    EXPECT_TRUE(ret1.second);
    EXPECT_TRUE(tree.contains(10));

    auto ret2 = tree.insert({10, "TenAgain"});
    EXPECT_FALSE(ret2.second);
    EXPECT_EQ(tree.at(10), "Ten");
    
    tree.insert({20, "Twenty"});
    tree.insert({3,  "Three"});
    tree.insert({5,  "Five"});

    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(3));
    EXPECT_TRUE(tree.contains(5));
    EXPECT_FALSE(tree.contains(999));
}

TEST(AVL_Test, OperatorIndex) {
    avl<int, std::string> tree;
    std::string val = tree[123];
    EXPECT_EQ(val, "");

    EXPECT_TRUE(tree.contains(123));
    EXPECT_EQ(tree.at(123), "");

    tree[123] = "Hello";
    EXPECT_EQ(tree.at(123), "Hello");

    tree[10] = "Ten";
    tree[20] = "Twenty";
    EXPECT_EQ(tree.size(), 3u);

    tree[10] = "TenAgain";
    EXPECT_EQ(tree.at(10), "TenAgain");
}

TEST(AVL_Test, AtExceptions) {
    avl<int, std::string> tree;
    EXPECT_THROW(tree.at(999), std::out_of_range);

    tree[10] = "Ten";
    EXPECT_NO_THROW({
        auto val = tree.at(10);
        EXPECT_EQ(val, "Ten");
    });
}

TEST(AVL_Test, FindMethod) {
    avl<int, std::string> tree;
    tree.insert({10, "Ten"});
    tree.insert({20, "Twenty"});
    tree.insert({5,  "Five"});

    auto it1 = tree.find(10);
    EXPECT_NE(it1, tree.end());
    EXPECT_EQ(it1->first, 10);
    EXPECT_EQ(it1->second, "Ten");

    auto it2 = tree.find(999);
    EXPECT_EQ(it2, tree.end());
}

TEST(AVL_Test, ClearMethod) {
    avl<int, std::string> tree;
    tree.insert({10, "Ten"});
    tree.insert({20, "Twenty"});
    tree.insert({5,  "Five"});
    EXPECT_EQ(tree.size(), 3u);

    tree.clear();
    EXPECT_EQ(tree.size(), 0u);
    EXPECT_TRUE(tree.empty());

    tree.clear();
    EXPECT_EQ(tree.size(), 0u);
    EXPECT_TRUE(tree.empty());
}

TEST(AVL_Test, IterationCheck) {
    avl<int, std::string> tree;
    tree[10] = "Ten";
    tree[20] = "Twenty";
    tree[5]  = "Five";
    tree[6]  = "Six";
    tree[3]  = "Three";
    std::vector<int> keys;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        keys.push_back(it->first);
    }
    EXPECT_EQ(keys.size(), 5u);
    EXPECT_EQ(keys[0], 3);
    EXPECT_EQ(keys[1], 5);
    EXPECT_EQ(keys[2], 6);
    EXPECT_EQ(keys[3], 10);
    EXPECT_EQ(keys[4], 20);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
