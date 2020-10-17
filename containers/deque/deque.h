//
// Created by ywt on 2020/10/14.
//

#ifndef CDS_DEQUE_H
#define CDS_DEQUE_H

#define DEQUE_NODE_LEN 512
size_t node_size(size_t ele_size) {
    return ele_size > DEQUE_NODE_LEN ? ele_size : DEQUE_NODE_LEN;
}

size_t node_ele_size(size_t ele_size) {
    return ele_size > DEQUE_NODE_LEN ? DEQUE_NODE_LEN / ele_size : 1;
}

/*
 * [first.first, first.last) is validate elements in node range
 * [first.cur, first.last) is validate elements in first node
 * [last.first, last.cur) is validate elements in last node
 * [first, last) range
 * _front_cur point to saved element,
 * _back_cur point to next to saved element
 */
template<typename T>
struct DequeIterator {
    DequeIterator() : _node_pos(0), _first(nullptr), _last(nullptr),
        _front_cur(nullptr), _back_cur(nullptr) {}

    size_t _node_pos;
    T* _first;
    T* _last;
    T* _front_cur;
    T* _back_cur;

    size_t front_size() {
        return _front_cur - _first + 1;
    }

    size_t back_size() {
        return _last - _back_cur;
    }
};

template <typename T>
class Deque {
public:
    Deque();

    bool empty();

    size_t size();

    T& front();

    void push_front(const T& value);

    void pop_front();

    T& back();

    void push_back(const T& value);

    void pop_back();

private:
    bool front_back_same_node();
    size_t same_node_size();

    void set_first_node(size_t node_pos);

private:
    T** _nodes;
    size_t _node_size;
    size_t _node_ele_size;

    DequeIterator<T> _first_node;
    DequeIterator<T> _last_node;
};

template <typename T>
Deque<T>::Deque() : _nodes(nullptr), _node_size(0), _node_ele_size(0) {
}

template <typename T>
bool Deque<T>::front_back_same_node() {
    return _first_node._node_pos == _last_node._node_pos;
}

template <typename T>
size_t Deque<T>::same_node_size() {
    // assert(front_back_same_node());
    return _first_node._back_cur - _first_node._front_cur;
}

template <typename T>
bool Deque<T>::empty() {
    return _nodes == nullptr || _node_size == 0
            || (front_back_same_node() && same_node_size() == 0);
}

template <typename T>
size_t Deque<T>::size() {
    size_t res = 0;
    if (empty()) {
        return 0;
    }
    if (front_back_same_node()) {
        res = same_node_size();
    } else {
        res += _first_node.front_size();
        res += _last_node.back_size();
        if (_last_node._node_pos - _first_node._node_pos > 1) {
            res += (_last_node._node_pos - _first_node._node_pos - 1) * _node_ele_size;
        }
    }
    return res;
}

template <typename T>
T& Deque<T>::front() {
    // assert(!empty());
    return *_first_node._front_cur;
}

template <typename T>
void Deque<T>::push_front(const T& value) {
    if (_nodes == nullptr || _node_size == 0) {
        _nodes = new T*;
        _node_size = 1;
        *_nodes = malloc(node_size(sizeof(T)));

        // TODO 这里实现有问题
        // 第一次分配的 cur 位置，怎么判断 front 空满，或者 back 空满。
        set_first_node(0);
        *_first_node._front_cur = value;
        _first_node._front_cur++;
    }
}

template <typename T>
void Deque<T>::set_first_node(size_t node_pos) {
    _first_node._node_pos = node_pos;
    _first_node._first = _nodes[node_pos];
    _first_node._last = _nodes[node_pos] + node_ele_size(sizeof(T));
    _first_node._front_cur = nullptr;
    _first_node._back_cur = nullptr;
}

#endif //CDS_DEQUE_H
