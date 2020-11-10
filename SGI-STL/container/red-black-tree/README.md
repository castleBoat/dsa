# red-black-tree

# 内部结构

- `_Rb_tree_node_base`: 基础机构，定义了 `_M_color`, `_M_parent`, `_M_left`, `_M_right` 指针。

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

```bash
```

header._M_parent 指向 root 节点
header._M_left 指向最左的节点（最小节点）
header._M_right 指向最右的节点（最大节点）
root._M_parent 指向 header 节点

## 迭代器

- `_Rb_tree_iterator<_Tp>`: 迭代器，内部保存指向 `_Rb_tree_node_base` 的指针，访问元素时转换为 `_Rb_tree_node<_Tp>`

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

- `_Rb_tree<>`: 模板变量有 `_Key`, `_Val`, `_KeyOfValue`, `_Compare`，其中 `_KeyOfValue` 作用？
- `_Rb_tree::_Rb_tree_impl`:

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
