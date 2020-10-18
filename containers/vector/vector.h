//
// Created by ywt on 2020/10/18.
//

#ifndef CDS_VECTOR_H
#define CDS_VECTOR_H

#include <cstdlib>
#include <cstring>
#include <cassert>

template <typename T>
class vector {
public:
    vector();
    ~vector();

    T& front();
    T& back();
    T& operator[](size_t pos);

    bool empty();
    size_t size();
    // void reserve(size_t size);
    size_t capacity();

    void push_back(const T& value);
    void pop_back();
    void insert(size_t pos, const T& value);
    void erase(size_t pos);

private:
    void renew_buf(size_t size);


private:
    static const size_t INIT_BUF_SIZE = 8;

    T* _buf;
    size_t _size;
    size_t _capacity;
};

template <typename T>
vector<T>::vector() : _size(0), _capacity(INIT_BUF_SIZE) {
    _buf = static_cast<T*>(malloc(sizeof(T) * _capacity));
}

template <typename T>
vector<T>::~vector() {
    for (size_t i = 0; i < _size; ++i) {
        _buf[i].~T();
    }
    free(_buf);
}

template <typename T>
T& vector<T>::front() {
    return _buf[0];
}

template <typename T>
T& vector<T>::back() {
    return _buf[_size-1];
}

template <typename T>
T& vector<T>::operator[](size_t pos) {
    return _buf[pos];
}

template <typename T>
bool vector<T>::empty() {
    return _size == 0;
}

template <typename T>
size_t vector<T>::size() {
    return _size;
}

template <typename T>
size_t vector<T>::capacity() {
    return _capacity;
}

template <typename T>
void vector<T>::push_back(const T &value) {
    if (_size >= _capacity) {
        renew_buf(2 * _capacity);
    }
    new(&_buf[_size++]) T(value);
}

// 这里 pop 时需要调用析构函数，stl 中通过 `_Alloc_traits::destroy` 析构
template <typename T>
void vector<T>::pop_back() {
    _buf[--_size].~T();
}

template <typename T>
void vector<T>::insert(size_t pos, const T &value) {
    if (_size >= _capacity) {
        renew_buf(2 * _capacity);
    }
//    for (size_t i = _size-1; i >= pos; --i) {
//        _buf[i+1] = _buf[i];  // 使用 memmove 可以代替不调用析构吗？
//    }
    memmove(&_buf[pos+1], &_buf[pos], (_size-pos)*sizeof(T));
    _buf[pos].~T();
    new(&_buf[pos]) T(value);
    ++_size;
}

template <typename T>
void vector<T>::erase(size_t pos) {
    _buf[pos].~T();
    memmove(&_buf[pos], &_buf[pos+1], (_size-pos-1)*sizeof(T));
    --_size;
}

// TODO 这里扩容移动旧元素时应该使用 move 或者拷贝构造。直接使用 realloc 是不行的
// 并且原来位置的元素也需要调用析构函数
template <typename T>
void vector<T>::renew_buf(size_t size) {
    _buf = static_cast<T*>(realloc(_buf, sizeof(T) * size));
    assert(_buf != nullptr);
    _capacity = size;
}

#endif //CDS_VECTOR_H
