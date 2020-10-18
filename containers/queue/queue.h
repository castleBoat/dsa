//
// Created by ywt on 2020/10/18.
//

#ifndef CDS_QUEUE_H
#define CDS_QUEUE_H

#include "../deque/deque.h"

template <typename T, typename Container = deque<T>>
class queue {
public:
    queue() {};

    ~queue() {};

    bool empty() {
        return _c.empty();
    };

    size_t size() {
        return _c.size();
    };

    T& front() {
        return _c.front();
    };

    T& back() {
        return _c.back();
    };

    void push(const T& value) {
        _c.push_back(value);
    };

    void pop() {
        _c.pop_front();
    };

private:
    Container _c;
};

#endif //CDS_QUEUE_H
