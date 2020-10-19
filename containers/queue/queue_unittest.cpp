//
// Created by ywt on 2020/10/18.
//

#include "queue.h"
#include "gtest/gtest.h"

namespace {

TEST(queueTest, empty) {
    queue<int> q;
    EXPECT_EQ(true, q.empty());
    EXPECT_EQ(0, q.size());
}

TEST(queueTest, PushPop) {
    queue<int> q;
    q.push(1);
    EXPECT_EQ(1, q.front());
    EXPECT_EQ(1, q.back());
    q.push(2);
    EXPECT_EQ(1, q.front());
    EXPECT_EQ(2, q.back());
    q.pop();
    EXPECT_EQ(2, q.front());
    EXPECT_EQ(2, q.back());
    q.pop();
    EXPECT_EQ(true, q.empty());
    EXPECT_EQ(0, q.size());
}

}

