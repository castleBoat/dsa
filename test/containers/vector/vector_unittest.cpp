//
// Created by ywt on 2020/10/18.
//

#include "vector.h"
#include "gtest/gtest.h"

TEST(vectorTest, Empty) {
    vector<int> v;
    EXPECT_EQ(true, v.empty());
    EXPECT_EQ(0, v.size());
    v.push_back(1);
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(1, v.back());
    v.pop_back();
    EXPECT_EQ(true, v.empty());
}

TEST(vectorTest, Push) {
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(4, v.back());

    for (size_t i = 5; i < 128; ++i) {
        v.push_back(i);
    }
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(127, v.back());
    EXPECT_EQ(127, v.size());
    EXPECT_EQ(128, v.capacity());
}

TEST(vectorTest, Pop) {
    vector<int> v;
    size_t nums = 16;

    for (size_t i = 0; i <= nums; ++i) {
        v.push_back(i);
    }
    for (size_t i = nums; i >= nums; --i) {
        EXPECT_EQ(i, v.back());
        v.pop_back();
    }
}


TEST(vectorTest, InsertErase) {
    vector<int> v;
    v.push_back(1);
    // v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.insert(1, 2);
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(4, v.back());
    EXPECT_EQ(4, v.size());

    v.erase(2);
    EXPECT_EQ(1, v.front());
    EXPECT_EQ(4, v.back());
    EXPECT_EQ(3, v.size());
}

TEST(vectorTest, Operator) {
    vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    EXPECT_EQ(0, v[0]);
    EXPECT_EQ(1, v[1]);
    EXPECT_EQ(5, v[5]);
    v.erase(2);
    EXPECT_EQ(3, v[2]);
}

