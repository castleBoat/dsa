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
 * front._cur point to saved element,
 * back._cur point to next to saved element
 * [front._first, front._last) is elements in node range
 * [front._cur, front._last) is validate elements in front node
 * [back._first, back._last) is elements in node range
 * [back._first, back._cur) is validate elements in back node
 * [front, back] range
 * [front._cur, back._cur) is validate elements if front and back point to same node
 */
template<typename T>
struct DequeIterator {
    DequeIterator() : _node_pos(0), _first(nullptr), _last(nullptr),
        _cur(nullptr) {}

    size_t _node_pos;
    T* _first;
    T* _last;
    T* _cur;

    size_t front_size() {
        return _last - _cur;
    }

    size_t back_size() {
        return _cur - _first;
    }
};

template <typename T>
class Deque {
public:
    Deque();

    ~Deque();

    bool empty();

    size_t size();

    T& front();

    void push_front(const T& value);

    void pop_front();

    T& back();

    void push_back(const T& value);

    void pop_back();

private:
    // void malloc_nodes(size_t num);

    void set_iter_pos(DequeIterator<T>* iter, size_t pos);

    bool front_back_same_node();

    size_t front_back_same_node_size();

    void add_nodes(size_t num, bool back);

private:
    T** _nodes;
    size_t _node_size;

    DequeIterator<T> _front_node;
    DequeIterator<T> _back_node;
};

template <typename T>
Deque<T>::Deque() : _nodes(nullptr), _node_size(0) {
    _node_size = 2;
    _nodes = static_cast<T**>(malloc(sizeof(T*) * _node_size));
    for (size_t i = 0; i < _node_size; ++i) {
        _nodes[i] = static_cast<T*>(malloc(node_size(sizeof(T))));
    }
    set_iter_pos(&_front_node, 0);
    _front_node._cur = _front_node._last;

    set_iter_pos(&_back_node, 1);
    _back_node._cur = _back_node._first;
}

template <typename T>
Deque<T>::~Deque() {
    for (size_t i = 0; i < _node_size; ++i) {
        for (size_t j = 0; j < node_ele_size(sizeof(T)); ++j) {
            _nodes[i][j].~T();
        }
        free(_nodes[i]);
    }
    free(_nodes);
}

template <typename T>
void Deque<T>::set_iter_pos(DequeIterator<T> *iter, size_t pos) {
    iter->_node_pos =  pos;
    iter->_first = _nodes[pos];
    iter->_last = iter->_first + node_ele_size(sizeof(T));
}

template <typename T>
bool Deque<T>::front_back_same_node() {
    return _front_node._node_pos == _back_node._node_pos;
}

template <typename T>
size_t Deque<T>::front_back_same_node_size() {
    // assert(front_back_same_node());
    return _back_node._cur - _front_node._cur;
}

template <typename T>
bool Deque<T>::empty() {
    return size() == 0;
}

template <typename T>
size_t Deque<T>::size() {
    size_t res = 0;
    if (front_back_same_node()) {
        res = front_back_same_node_size();
    } else {
        res += _front_node.front_size();
        res += _back_node.back_size();
        if (_back_node._node_pos > _front_node._node_pos) {
            res += (_back_node._node_pos - _front_node._node_pos - 1) * node_ele_size(sizeof(T));
        }
    }
    return res;
}

template <typename T>
T& Deque<T>::front() {
    // assert(!empty());
    return *_front_node._front_cur;
}

template <typename T>
void Deque<T>::push_front(const T& value) {
    if (_front_node._cur == _front_node._first) {
        if (_front_node._node_pos > 0) {
            set_iter_pos(&_front_node, _front_node._node_pos - 1);
            _front_node._cur = _front_node._last;
        } else {
            add_nodes(1, false);
            set_iter_pos(&_front_node, 0);
            _back_node._node_pos += 1;
        }
    }
    --_front_node._cur;
    *_front_node._cur = value;
}

template <typename T>
void Deque<T>::add_nodes(size_t num, bool back) {
    T** new_nodes = static_cast<T**>(malloc((_node_size + num) * sizeof(T*)));
    size_t offset = back ? 0 : num;
    for (size_t i = 0; i < _node_size; ++i) {
        new_nodes[i + offset] = _nodes[i];
    }
    // TODO malloc added node
    for (size_t i = 0; i < num; ++i) {
    }
    _node_size += num;
    free(_nodes);
    _nodes = new_nodes;
}


#endif //CDS_DEQUE_H
