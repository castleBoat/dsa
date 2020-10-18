# vector

stl 中声明形式是：

```c++
template<
    class T,
    class Allocator = std::allocator<T>
> class vector;
```

vector 是封装了动态大小数组的顺序容器，容器内元素在内存中顺序存放，并提供随机位置读写能力。

size 表示容器内存放的元素个数，capacity 表示容器最多能够存放的元素个数，一般 capacity >= size。

当添加的元素个数大于容器 capacity 时容器需要扩容，重新开辟一段连续空间并把所有元素拷贝到新位置，

所以 vector 扩容是很费时的操作。

时间复杂度:

- 随机访问：O(1)
- 插入或删除尾部元素：O(1)
- 插入或删除其他位置：需要移动插入或删除位置到尾部的线性距离，O(n)

# 底层实现

基类 `_Vector_base`，基类中定义的内部结构 `_Vector_impl`，并有这个结构的成员 `_M_impl`

```c++
struct _Vector_impl {
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;
};
```

![vector](https://github.com/steveLauwh/SGI-STL/raw/master/The%20Annotated%20STL%20Sources%20V3.3/Other/vector.PNG)

应该注意的点：

1. 开始的 vector capacity 为 0，没有分配容量。
2. 扩容的计算方法是 `__len = size() + std::max(size(), __n);`，如果增加的小于当前 size，则按 2 倍扩容；否则按新增量增加
3. 扩容时应该调用拷贝构造（move更好）移动到新位置，并且原位置中元素需要调用析构函数析构掉。
4. 扩容移动元素时调用的是 `__uninitialized_move_if_noexcept_a`，待确认实现，是否是使用 move ？
5. 扩容时原位置元素析构是调用 `std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());`，待确认？
