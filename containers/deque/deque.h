//
// Created by ywt on 2020/10/14.
//

#ifndef CDS_DEQUE_H
#define CDS_DEQUE_H

#include <cassert>

#define DEQUE_NODE_LEN 512
size_t node_size(size_t ele_size) {
    return ele_size > DEQUE_NODE_LEN ? ele_size : DEQUE_NODE_LEN;
}

size_t node_ele_size(size_t ele_size) {
    return ele_size > DEQUE_NODE_LEN ? 1: DEQUE_NODE_LEN / ele_size;
}

/*
 * deque contains point to fix-size element array
 * [node._first, node._last) is element array range
 * so node._cur is in [node._first, node._last)
 *
 * front_node and back_node point to array which has front or back element
 * front._cur point to saved element,
 * back._cur point to next to saved element
 * [front._first, front._last) is elements in node range
 * [front._cur, front._last) is validate elements in front node
 * so front._cur is in [front._first, front._last)
 *
 * [back._first, back._last) is elements in node range
 * [back._first, back._cur) is validate elements in back node
 * so back._cur is in (back.first, back._last]
 *
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
class deque {
public:
    deque();

    ~deque();

    bool empty();

    size_t size();

    T& front();

    void push_front(const T& value);

    void pop_front();

    T& back();

    void push_back(const T& value);
    // void push_back(T&& value);

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
deque<T>::deque() : _nodes(nullptr), _node_size(0) {
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
deque<T>::~deque() {
//    for (size_t i = 0; i < _node_size; ++i) {
//        for (size_t j = 0; j < node_ele_size(sizeof(T)); ++j) {
//            _nodes[i][j].~T();
//        }
//        free(_nodes[i]);
//    }
    if (_front_node._cur == _front_node._last) {
        set_iter_pos(&_front_node, _front_node._node_pos+1);
        _front_node._cur = _front_node._first;
    }

    // free not save element array
    for (size_t pos = 0; pos < _front_node._node_pos; ++pos) {
        free(_nodes[pos]);
    }

    // free save element array, call dtor
    while (_front_node._node_pos < _back_node._node_pos) {
        while (_front_node._cur < _front_node._last) {
            _front_node._cur->~T();
            ++_front_node._cur;
        }
        free(_nodes[_front_node._node_pos]);

        set_iter_pos(&_front_node, _front_node._node_pos+1);
        _front_node._cur = _front_node._first;
    }
    while (_front_node._cur < _back_node._cur) {
        _front_node._cur->~T();
        ++_front_node._cur;
    }

    for (size_t pos = _front_node._node_pos; pos < _node_size; ++pos) {
        free(_nodes[pos]);
    }
    free(_nodes);
}

template <typename T>
void deque<T>::set_iter_pos(DequeIterator<T> *iter, size_t pos) {
    iter->_node_pos =  pos;
    iter->_first = _nodes[pos];
    iter->_last = iter->_first + node_ele_size(sizeof(T));
}

template <typename T>
bool deque<T>::front_back_same_node() {
    return _front_node._node_pos == _back_node._node_pos;
}

template <typename T>
size_t deque<T>::front_back_same_node_size() {
    // assert(front_back_same_node());
    return _back_node._cur - _front_node._cur;
}

template <typename T>
bool deque<T>::empty() {
    return size() == 0;
}

template <typename T>
size_t deque<T>::size() {
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
T& deque<T>::front() {
    // assert(!empty());
    if (_front_node._cur == _front_node._last) {
        set_iter_pos(&_front_node, _front_node._node_pos+1);
        _front_node._cur = _front_node._first;
    }
    return *_front_node._cur;
}

template <typename T>
void deque<T>::push_front(const T& value) {
    if (_front_node._cur == _front_node._first) {
        if (_front_node._node_pos == 0) {
            add_nodes(1, false);
            _front_node._node_pos += 1;
            _back_node._node_pos += 1;
        }
        set_iter_pos(&_front_node, _front_node._node_pos-1);
        _front_node._cur = _front_node._last;
    }
    --_front_node._cur;
    new(_front_node._cur) T(value);
}

template <typename T>
void deque<T>::pop_front() {
    if (_front_node._cur == _front_node._last) {
        assert(_front_node._node_pos < _node_size);
        set_iter_pos(&_front_node, _front_node._node_pos+1);
        _front_node._cur = _front_node._first;
    }
    ++_front_node._cur;
}

template <typename T>
T& deque<T>::back() {
    if (_back_node._cur == _back_node._first) {
        set_iter_pos(&_back_node, _back_node._node_pos-1);
        _back_node._cur = _back_node._last;
    }
    return *(_back_node._cur-1);
}

template <typename T>
void deque<T>::push_back(const T &value) {
    if (_back_node._cur == _back_node._last) {
        if (_back_node._node_pos == _node_size-1) {
            add_nodes(1, true);
        }
        set_iter_pos(&_back_node, _back_node._node_pos+1);
        _back_node._cur = _back_node._first;
    }
    new(_back_node._cur) T(value);
    ++_back_node._cur;
}

template <typename T>
void deque<T>::pop_back() {
    if (_back_node._cur == _back_node._first) {
        assert(_back_node._node_pos > 0);
        set_iter_pos(&_back_node, _back_node._node_pos-1);
        _back_node._cur = _back_node._last;
    }
    --_back_node._cur;
}

template <typename T>
void deque<T>::add_nodes(size_t num, bool back) {
    T** new_nodes = static_cast<T**>(malloc((_node_size + num) * sizeof(T*)));
    size_t offset = back ? 0 : num;
    for (size_t i = 0; i < _node_size; ++i) {
        new_nodes[i + offset] = _nodes[i];
    }
    offset = back ? _node_size : 0;
    for (size_t i = 0; i < num; ++i) {
        new_nodes[i + offset] = static_cast<T*>(malloc(node_size(sizeof(T))));
    }
    _node_size += num;
    free(_nodes);
    _nodes = new_nodes;
}


#endif //CDS_DEQUE_H
