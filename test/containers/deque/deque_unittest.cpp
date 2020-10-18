//
// Created by ywt on 2020/10/18.
//

#include "deque.h"
#include <gtest/gtest.h>

namespace {

TEST(dequeTest, PushFront) {
    deque<int> q;
    q.push_front(1);
    q.push_front(2);
    q.push_front(3);
    EXPECT_EQ(3, q.front());
    q.pop_front();
    EXPECT_EQ(2, q.front());
    q.pop_front();
    EXPECT_EQ(1, q.front());
    q.pop_front();
    EXPECT_EQ(true, q.empty());
}

TEST(dequeTest, PushBack) {
    deque<int> q;
    q.push_back(1);
    q.push_back(2);
    EXPECT_EQ(2, q.back());
    q.pop_back();
    EXPECT_EQ(1, q.back());
    q.pop_back();
    EXPECT_EQ(true, q.empty());
}

TEST(dequeTest, PushFrontAndPushBack) {
    deque<int> q;
    q.push_back(1);
    q.push_back(2);
    EXPECT_EQ(1, q.front());
    EXPECT_EQ(2, q.back());
    q.pop_back();
    EXPECT_EQ(1, q.front());
    EXPECT_EQ(1, q.back());
    q.pop_back();
    EXPECT_EQ(true, q.empty());

    q.push_front(1);
    q.push_back(2);
    EXPECT_EQ(1, q.front());
    EXPECT_EQ(2, q.back());
    EXPECT_EQ(2, q.size());
    q.pop_front();
    EXPECT_EQ(2, q.front());
    EXPECT_EQ(2, q.back());
    q.pop_back();
    EXPECT_EQ(true, q.empty());

}

TEST(dequeTest, BigNumPush) {
    deque<int> q;

    int nums = 512;
    for (int i = 0; i < nums; ++i) {
        if (i % 2 == 0) {
            q.push_front(i);
        } else {
            q.push_back(i);
        }
    }
    for (int i = nums - 1; i >= 0; --i) {
        if (i % 2 == 0) {
            EXPECT_EQ(i, q.front());
            q.pop_front();
        } else {
            EXPECT_EQ(i, q.back());
            q.pop_back();
        }
    }
    EXPECT_EQ(true, q.empty());
}

class S {
public:
    explicit S(int v) {
        val = new int(v);
        ++count;
        std::cout << "S()" << std::endl;
    }

    ~S() {
        free(val);
        --count;
        std::cout << "~S()" << std::endl;
    }

    S(const S &rhs) {
        val = new int(*rhs.val);
        ++count;
        std::cout << "S(const S&)" << std::endl;
    }

    S(S &&rhs) {
        free(val);
        val = rhs.val;
        rhs.val = nullptr;
        ++count;
        std::cout << "S(S&&)" << std::endl;
    }

public:
    int *val;
    static size_t count;
};

size_t S::count = 0;

TEST(dequeTest, CallCtorAndDtor) {
    deque<S> q;
    S s(1);
    q.push_back(s);
    q.push_back(s);
    EXPECT_EQ(3, S::count);
}

}  // namespace
