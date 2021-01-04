
# array

array 存放固定大小连续元素的容器。使用上是取代 C 语言的数组使用。

# 构造函数

在文件中没有找到构造函数的实现。在实现中找到一句注释的话

> // No explicit construct/copy/destroy for aggregate type

array 做为 aggregate 类型，没有提供构造函数。

查看 cppreference 中构造函数

> initializes the array following the rules of aggregate initialization (note that default initialization may result in indeterminate values for non-class T) 

[aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) 的定义比较像是基础类型或者 pod 类型，所以在 array 中建议使用简单的类型。

# __array_traits

是 array 的 traits 类型，定义了实际保存元素数组的类型，和存取访问数组中元素的接口。

```c++
template<typename _Tp, std::size_t _Nm>
  struct __array_traits
  {
    typedef _Tp _Type[_Nm];  // 数组类型 _Type
    typedef __is_swappable<_Tp> _Is_swappable;
    typedef __is_nothrow_swappable<_Tp> _Is_nothrow_swappable;

    static constexpr _Tp&
    _S_ref(const _Type& __t, std::size_t __n) noexcept  // 访问元素的接口
    { return const_cast<_Tp&>(__t[__n]); }

    static constexpr _Tp*
    _S_ptr(const _Type& __t) noexcept
    { return const_cast<_Tp*>(__t); }
  }
```

在 array 类中定义了 traits::_Type 类型的数组，也就是实际保存元素的数组，并通过 traits 的接口访问数组中的元素。

可以看见实际数据是分配在栈上的。

```c++
  template<typename _Tp, std::size_t _Nm>
    struct array
    {
      // Support for zero-sized arrays mandatory.
      typedef _GLIBCXX_STD_C::__array_traits<_Tp, _Nm> _AT_Type;
      typename _AT_Type::_Type                         _M_elems;  // 实际保存元素的数组

      _GLIBCXX17_CONSTEXPR iterator begin() noexcept
      { return iterator(data()); }

      _GLIBCXX17_CONSTEXPR iterator end() noexcept
      { return iterator(data() + _Nm); }

      _GLIBCXX17_CONSTEXPR pointer data() noexcept
      { return _AT_Type::_S_ptr(_M_elems); }

      _GLIBCXX17_CONSTEXPR reference at(size_type __n)
      {
	if (__n >= _Nm)
	  std::__throw_out_of_range_fmt(__N("array::at: __n (which is %zu) "
					    ">= _Nm (which is %zu)"),
					__n, _Nm);
	return _AT_Type::_S_ref(_M_elems, __n);
      }
    }
```

# 访问接口

1. at(size_t): 提供 at 接口访问
2. std::get<size_t>(array): get 接口访问，模板变量是下标
3. std::get<size_t>(std::move(array)): get 接口访问，模板变量是下标, 移动单个元素

## std::move

注意通过 `std::move` 访问元素实际移动的是数组中的元素，而不是整个数组。

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
