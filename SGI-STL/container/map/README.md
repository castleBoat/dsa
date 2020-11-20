# map

map 提供排序的 (key, value) pair 关联容器，其中 key 是唯一性。提供在 log 时间内根据 key 查找、插入、删除能力。

并提供双向遍历能力。

# 内部

声明类型：

```c++
template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class map {
    typedef _Key					key_type;
    typedef _Tp					mapped_type;
    typedef std::pair<const _Key, _Tp>		value_type;
    typedef _Compare					key_compare;
    typedef _Alloc					allocator_type

    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>,
       key_compare, _Pair_alloc_type> _Rep_type;

    /// The actual tree structure.
    _Rep_type _M_t
};
```

可以看见 map 包含了 `_Rb_tree` 的成员 `_M_t`，使用红黑树做为底层数据结构。

模板类型：

- `_Key`: map 中主键类型
- `_Tp`: mapped_type 映射的类型，
- `_Compare`: 比较 key_type 的 functor, key_compare

加上内部定义的类型：

- `value_type`: 把 key_type 和 mapped_type 放在一起的 pair，做为红黑树的 value 类型

# 接口

map 提供了 at, 下标访问 `[]`, insert, earse, find, lower_bound, upper_bound 接口，都是直接使用红黑树的对应接口。

# multimap

和 map 一样的关联容器，不同之处在能够存储相同的 key。声明形式：

```c++
template <typename _Key, typename _Tp,
    typename _Compare = std::less<_Key>,
    typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class multimap;
```

模板参数定义和 map 一致。

和 map 一样底层使用相同的红黑树结构，区别在插入时 map 使用红黑树的 `_M_insert_unique`，multimap 使用 `_M_insert_equal`。

红黑树的相关操作可以见[这篇内容](https://github.com/castleBoat/dsa/blob/main/SGI-STL/container/red-black-tree/README.md)。

