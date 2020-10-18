# queue

c++ 中 queue 的声明是:

```c++
template<
    class T,
    class Container = std::deque<T>
> class queue;
```

可以看见 queue 使用了 `deque` 作为底层容器提供先进先出规则。使用的 `deque` 的 push_back, pop_front 实现先进先出。

这种使用其他底层容器的容器叫做 container adapter，类似 stack 也是这样。

queue 还可以使用 list 作为底层容器。

# priority_queue

C++ 中 priority_queue 的声明是：

```c++
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

priority_queue 是带有权重的 queue，默认每次弹出的都是最大值（序列的最后一个元素）。

最后一个元素是最大值由 Compare 参数控制，默认参数 std::less 是一个函数对象，判断两个数的大小，声明是：

```c++
bool operator()( const T& lhs, const T& rhs ) const;
// 函数返回 true 如果 lhs < rhs
```

在 priority_queue 实现中 Compare 返回 true 时第一个参数排在第二个参数钱前面，所以最后一个元素是最大的元素。

如果需要自定义排序和弹出元素需要记住两点：

1. Compare 返回 true 时第一个参数在第二个前面
2. 每次弹出的序列的最后一个元素

Container 默认参数说明 priority_queue 使用 vector 作为基础容器，并且对 vector 做 heap 的处理规则，保证了每次弹出最大元素。

使用上一般把 priority_queue 作为最大堆使用。