# red-black-tree

# 内部结构

- `_Rb_tree_node_base`: 树的基础节点结构，定义了 `_M_color`, `_M_parent`, `_M_left`, `_M_right` 指针。

```c++
struct _Rb_tree_node_base
{
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

    _Rb_tree_color	_M_color;
    _Base_ptr		_M_parent;
    _Base_ptr		_M_left;
    _Base_ptr		_M_right;
};
```

- `_Rb_tree_header`: 内部包含一个 `_rb_tree_node_base` 节点做为 dump 头节点，和树的节点个数。

```c++
struct _Rb_tree_header
{
    _Rb_tree_node_base	_M_header;
    size_t		_M_node_count; // Keeps track of size of tree.
};
```

- `_Rb_tree_node<_Val>`: 继承基础节点 `_Rb_tree_node_base`，并内部保存用户类型的存储 `_M_storage`。

```c++
template<typename _Val>
struct _Rb_tree_node : public _Rb_tree_node_base
{
    typedef _Rb_tree_node<_Val>* _Link_type;
    __gnu_cxx::__aligned_membuf<_Val> _M_storage;
};
```

实现上 header 作用如下：

![rbt_header](https://github.com/castleBoat/dsa/raw/main/SGI-STL/img/rbt_header.png)

  - header._M_parent 指向 root 节点
  - header._M_left 指向最左的节点（最小节点）
  - header._M_right 指向最右的节点（最大节点）
  - root._M_parent 指向 header 节点

## 红黑树结构

`_Rb_tree` 结构体。

```c++
template<typename _Key, typename _Val, typename _KeyOfValue,
   typename _Compare, typename _Alloc = allocator<_Val> >
class _Rb_tree
{
    template<typename _Key_compare,
        bool /* _Is_pod_comparator */ = __is_pod(_Key_compare)
    struct _Rb_tree_impl
    : public _Node_allocator
    , public _Rb_tree_key_compare<_Key_compare>
    , public _Rb_tree_header
    {
    };

    _Rb_tree_impl<_Compare> _M_impl;
};
```

模板参数的意义：

- `_Key`: key 数据类型
- `_Val`: value 数据类型。map 中的这个类型为 `std::pair<key, value>`
- `_KeyOfValue`: 通过 value 找到 key 的 functor。map 中就是通过 pair 找到 pair.first
- `_Compare`: 比较 key 大小的 functor。map 中默认是 std::less。

内部定义 `_Rb_tree_impl` 结构体，继承 `_Rb_tree_header`，并包含成员对象 `_M_impl` 做为实际结构。

## 迭代器

`_Rb_tree_iterator<_Tp>` 迭代器，内部保存指向 `_Rb_tree_node_base` 的指针，访问元素时转换为 `_Rb_tree_node<_Tp>`

```c++
template<typename _Tp>
struct _Rb_tree_iterator
{
    _Base_ptr _M_node;
};
```

迭代器支持 `++`, `--` 操作，分别是找到红黑树中大于当前值的最小值，小于当前值的最大值。具体做法是：

- ++: 先找右孩子的最左节点。如果没有右孩子，如果当前节点是右孩子，则当前节点指向父节点，再向上寻找。直到当前节点不是右孩子的父节点。
- --: 先找左孩子的最右节点。如果没有左孩子，如果当前节点是左孩子，则当前节点指向父节点，再向上寻找。直到当前节点不是左孩子的父节点。

## 接口

### 查找

- lower_bound(k): 查找**大于或等于** k 的最小值，如果没找到返回 end()
- upper_bound(k): 查找**大于** k 的最小值，如果没找到返回 end()
- equal_range():

这两个函数在范围查找时很有用

- `find`:
- `count`:

### 插入

- `_M_insert_unique`: 插入元素到树中，不允许重复元素。插入成功返回 pair(插入元素迭代器, true)；失败返回 pair(指向相同元素的迭代器, false)
- `_M_insert_equal`: 插入元素到树中，允许重复元素。插入成功返回 pair(插入元素迭代器, true)；失败返回 pair(指向相同元素的迭代器, false)
- `_M_emplace_unique`: 插入元素，利用传入的参数在节点中构造 pair<key, value>。省去了 pair 的 copy 或者 move 的操作。这里参数可以是构造 pair 中对象需要的参数、也可以直接是 pair（节点中就是使用 move 操作构造新的对象）。
- `_M_emplace_equal`: 同上一样，但是支持保存相同的 key。

先查找元素应该插入的叶子节点位置，然后调用上面的内部插入节点，并调整红黑树到平衡。

查找定位接口：

- `_M_get_insert_unique_pos(k)`: 在红黑树中查找元素应插入位置。如果存在相同元素，则返回 pair(指向相同元素的迭代器, nullptr)；如果不存在则返回 pair(nullptr, 指向应插入元素位置叶子节点的迭代器)
- `_M_get_insert_equal_pos(k)`: 在红黑树中查找元素应插入位置，可以插入相同元素。返回 pair(nullptr, 指向应插入元素位置叶子节点的迭代器)

这两个函数在插入元素时首先查找位置时使用

插入到叶子节点函数：

- `_M_insert(__p, __v)`: 内部函数，插入 v 元素到叶子节点 p 下。

插入元素内部的函数，插入元素到叶子节点的孩子节点，并调整红黑树到平衡。

### 删除

- `erase`: 删除给定位置的元素，返回下一个位置上的迭代器，如果没有则返回 end();

使用 `_M_erase_aux` 接口删除给定位置元素，并调整保证树的平衡。

## 平衡

红黑树如何保证插入、删除之后的平衡
