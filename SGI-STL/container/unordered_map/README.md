# unordered_map

`unordered_map` 是 key-value 结构的关联式容器，key 保持唯一，插入，查找，删除等操作是常数级复杂度。它不提供任何的顺序。

有几个特点：

1. 底层按 key 的哈希值分桶
2. 所有的元素按单链表形式相连。同一个桶里的元素是单链表
3. 同一个桶里的链表尾部下一个元素可能指向另一个桶的元素，目的是迭代器可以通过 `++` 访问所有的元素。

第一个特点会产生两个问题：

- 不同类型的 key 如何计算哈希值
- 哈希值计算后元素放入哪个桶

## functional_hash

定义了实现哈希函数的 functor，接收把任意类型的指针（值）哈希成 std::size_t 类型。

对于基础整形类型，比如 char, short, int, long 等

- 如果参数是指针则哈希值就是指针地址。
- 如果参数是值则哈希值是强转成 std::size_t 的值。

对于其他类型（如 float, double, std::string 等）做哈希过程是：

1. 取对象地址和对象占用空间这段连续空间
2. 在这段内存内容上每单位长度（比如8字节）上计算哈希值

定义有 Murmur hash (默认 `_Hash_impl`)和 FNV-1a (`_Fnv_hash_impl`)两种哈希实现。 

对于 `std::string` 这种类型的哈希值，它在 `basic_string.h` 文件中单独特化了 `hash<std::string>` 结构，并实际调用默认的 `_Hash_impl` 计算哈希值。

```c++
template<>
struct hash<string>
: public __hash_base<size_t, string>
{
  size_t
  operator()(const string& __s) const noexcept
  { return std::_Hash_impl::hash(__s.data(), __s.length()); }
};

template<>
struct __is_fast_hash<hash<string>> : std::false_type
{ };
```

容器中使用 h1, h1 两个哈希函数。h1 是负责 key 类型计算得到 size_t 整形哈希值，h2 是负责范围哈希，计算哈希值落入 n 个桶的哪个。

容器默认的 h1 就是上面的函数；h2 是取余。整体上计算过程是：


```c++
bucket_idx = h2(h1(key), n)  // n is buckets count
```

## _Hashtable_traits

哈希表 traits，表示不同的标识。

```c++
template<bool _Cache_hash_code, bool _Constant_iterators, bool _Unique_keys>
struct _Hashtable_traits
{
  using __hash_cached = __bool_constant<_Cache_hash_code>;
  using __constant_iterators = __bool_constant<_Constant_iterators>;
  using __unique_keys = __bool_constant<_Unique_keys>;
};
```

- `_Cache_hash_code`: 是否保存哈希值，在查找时减少计算量
- `_Constrant_iterators`: 迭代器是否都是 const 类型。对于 unordered_set, unordered_multiset 为 true；unordered_map, unordered_multimap 为 false
- `_Unique_keys`: 哈希表中是否只能存放唯一值，unordered_set, unordered_map 为 true；unordered_multiset, unordered_multimap 为 false

## _Hashtable_ebo_helper

这里使用 `EBO(empty base optimization)` 空基类优化。EBO 的场景是有一个空类 A，不是类中完全为空，而是定义了一些 enum, non-virtual 方法等，没有 non-static 变量。

另一个类 B 需要使用 A 中的内容，有两种方法：1. 关联, B 保存 A 类型的成员变量。2. B 继承 A。

对比两种方法，由于 A 是空类，`但是 sizeof(A) == 1`，如果使用关联与其他变量需要对齐，浪费字节。而使用继承时不会有这个问题。

更多 EBO 可以查看[这篇文章](https://www.bookstack.cn/read/CPlusPlusThings/ca7ffa122ab5f274.md)。

哈希使用的 EBO 提供模板，判断传入的模板类型是 `!_is_final(_Tp) && _is_empty(_Tp)` 时使用 EBO；否则类中直接保留 `_Tp` 的成员对象。

对外暴露转换接口。

```c++
  /**
   *  Primary class template _Hashtable_ebo_helper.
   *
   *  Helper class using EBO when it is not forbidden (the type is not
   *  final) and when it is worth it (the type is empty.)
   */
  template<int _Nm, typename _Tp,
	   bool __use_ebo = !__is_final(_Tp) && __is_empty(_Tp)>
    struct _Hashtable_ebo_helper;

  /// Specialization using EBO.
  template<int _Nm, typename _Tp>
    struct _Hashtable_ebo_helper<_Nm, _Tp, true>
    : private _Tp
    {
      _Hashtable_ebo_helper() = default;

      template<typename _OtherTp>
	_Hashtable_ebo_helper(_OtherTp&& __tp)
	  : _Tp(std::forward<_OtherTp>(__tp))
	{ }

      static const _Tp&
      _S_cget(const _Hashtable_ebo_helper& __eboh)
      { return static_cast<const _Tp&>(__eboh); }

      static _Tp&
      _S_get(_Hashtable_ebo_helper& __eboh)
      { return static_cast<_Tp&>(__eboh); }
    };

  /// Specialization not using EBO.
  template<int _Nm, typename _Tp>
    struct _Hashtable_ebo_helper<_Nm, _Tp, false>
    {
      _Hashtable_ebo_helper() = default;

      template<typename _OtherTp>
	_Hashtable_ebo_helper(_OtherTp&& __tp)
	  : _M_tp(std::forward<_OtherTp>(__tp))
	{ }

      static const _Tp&
      _S_cget(const _Hashtable_ebo_helper& __eboh)
      { return __eboh._M_tp; }

      static _Tp&
      _S_get(_Hashtable_ebo_helper& __eboh)
      { return __eboh._M_tp; }

    private:
      _Tp _M_tp;
    };
```

## 迭代器

迭代器(`_Node_iterator`)内部保留了指向容器中单链表节点(`_Hash_node`)的指针，通过这个指针找到迭代器指向的实际节点。

同时迭代器的 `++` 操作是通过节点的 `_M_next` 指针移动。

```c++
  /// Base class for node iterators.
  template<typename _Value, bool _Cache_hash_code>
    struct _Node_iterator_base
    {
      using __node_type = _Hash_node<_Value, _Cache_hash_code>;

      __node_type*  _M_cur;

      void _M_incr() noexcept
      { _M_cur = _M_cur->_M_next(); }
    };

  template<typename _Value, bool __constant_iterators, bool __cache>
    struct _Node_iterator
    : public _Node_iterator_base<_Value, __cache> {
          _Node_iterator&
        operator++() noexcept {
          this->_M_incr();
          return *this;
        }
    };
```

## hashtable

保存了几个成员变量:

- `_M_buckets`: 桶数组，元素类型是链表节点指针。
- `_M_bucket_count`: 桶个数
- `_M_before_begin`: 链表的 dummy header 节点
- `_M_element_count`: 容器中元素个数
- `_M_rehash_policy`: 重新哈希的策略


`_M_before_begin` header 节点作用使容器 `begin()` 快速接口，由于迭代器是通过链表 next 节点移动，所有 `end()` 实际是空指针封装的。

```c++
  iterator begin() noexcept
  { return iterator(_M_begin());

  iterator end() noexcept
  { return iterator(nullptr); }
```

待确定两个问题：

1. 重新哈希的时机
2. 重新哈希的方法

### _Prime_rehash_policy

默认使用素数重新哈希策略，即桶个数是大于需要元素个数的最小素数。

```c++
  /// Default value for rehash policy.  Bucket size is (usually) the
  /// smallest prime that keeps the load factor small enough.
  struct _Prime_rehash_policy
  {
     _Prime_rehash_policy(float __z = 1.0) noexcept
        : _M_max_load_factor(__z), _M_next_resize(0) { }

     static const std::size_t _S_growth_factor = 2;

     float		_M_max_load_factor;
     mutable std::size_t	_M_next_resize;
  };
```

定义的 `_M_max_load_factor` 表示元素个数和桶个数比例，默认为 1. 定义了 `_S_growth_factor` 每次桶增加的因子。

所以每次桶增加的大小是：

```c++
// __n_elt 当前元素个数；__n_ins 插入元素个数; __n_bkt 桶的个数
long double __min_bkts = (__n_elt + __n_ins)
                 / (long double)_M_max_load_factor;
new_bkt_count = _M_next_bkt(std::max<std::size_t>(__builtin_floor(__min_bkts) + 1,
					      __n_bkt * _S_growth_factor));
```

如果每次插入元素，那么桶个数增长的序列是 `1, 3, 7, 17, 37...`。

待解决重新哈希是，每个元素如果排列的问题？
