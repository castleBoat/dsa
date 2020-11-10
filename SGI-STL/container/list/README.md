# list

list 是**双向链表**，提供线性时间遍历，和常数时间插入、删除。

# 内部

- `_List_node_base`: 保留前向和后项指针。
- `_List_node_header`: 继承 `_List_node_base`，是 dump 的 header 节点，不保存实际内容。有 `_M_size` 保存节点个数
- `_List_node<_Tp>`: 实际的链表节点。继承 `_List_node_base`，实际内容保存在 `_M_storage`
- `_List_iterator<_Tp>`: 定义的迭代器。保留了指向 `_List_node_base` 的指针成员做为迭代器指向的元素，使用迭代器访问元素时把指向的元素转换为 `_List_node` 类型，获取它的元素。通过 base 的前后项指针完成迭代器的移动。
- `_List_const_iterator<_Tp>`: list::const_iterator
- `_List_base<_Tp>`: 保留 `_List_impl _M_impl` 成员，它保存实际的链表内容。
- `_List_base<_Tp>::_List_impl`: 继承了以 `_List_node` 为粒度的 allocator。有 `_List_node_header _M_node` 保存 header。 
- `list<_Tp>::_List_base`: 

可以看见实现的结构和 `forward_list` 相同。都是封装节点的基本前后向指针结构，header 结构，node 结构, 迭代器。

## 迭代器

链表概念上是一个链表，但实际存储的是一个环，其中 dump header 节点 

```c++
_M_node <--> A <--> B <--> C
 ^                         ^
 |                         |
  -------------------------
```

- `_M_node._M_next`: 指向首节点
- `_M_node._M_prev`: 指向尾节点

所有返回首尾节点时 

- `front()` 返回的节点就是 `_M_node._M_next`；
- `back()` 返回的节点就是 `_M_node` 自己，因为尾节点的 next 指向 header。

判断链表是否为空，就是通过 front 是否等于 back，也就是 header 的 next 是否指向自己。


