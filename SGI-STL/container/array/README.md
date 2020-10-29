
# array

array 存放固定大小连续元素的容器。使用上是取代 C 语言的数组使用。

# 构造函数

在文件中没有找到构造函数的实现。查看 cppreference 中构造函数

> initializes the array following the rules of aggregate initialization (note that default initialization may result in indeterminate values for non-class T) 

[aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) 的定义比较像是基础类型或者 pod 类型，所以在 array 中建议使用简单的类型。

# 访问接口

1. at(size_t): 提供 at 接口访问
2. std::get<size_t>(array): get 接口访问，模板变量是下标

## std::move

注意通过

```c++
Foo f = std::get<1>(std::move(a));

// 底层实现
template<std::size_t _Int, typename _Tp, std::size_t _Nm>
constexpr _Tp&&
get(array<_Tp, _Nm>&& __arr) noexcept
{
  static_assert(_Int < _Nm, "array index is within bounds");
  return std::move(_GLIBCXX_STD_C::get<_Int>(__arr));
}
```

可以发现移动的是 `a[1]` 这个元素，array 本身或者其他元素仍是有效的。
