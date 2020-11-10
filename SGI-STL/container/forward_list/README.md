
# forward_list

是**单向链表**，提供常数时间插入和删除操作，不提供随机访问能力。

# 实现

实现上内部只保留了 `_M_head` 头节点，dump 节点，它的 `_M_next` 指向链表的第一个节点。

主要几个类之间的关系为：

- `_Fwd_list_node_base`: 包含指向下一个节点的 `_M_next` 指针
- `_Fwd_list_node`: 继承 `_Fwd_list_node_base`，并存储实际类型成员 `_M_storage`
- `_Fwd_list_base::_Fwd_list_impl`: 内部类，继承 `_Fwd_list_node`, 并包含链表的头节点 `_M_head`
- `_Fwd_list_base`: 基础类，包含一个 `_Fwd_list_impl` 的成员
- `forward_list`: 继承 `_Fwd_list_base`

![forward_list类关系图](https://github.com/castleBoat/dsa/raw/main/SGI-STL/img/forward_list.png)


# 使用

由于链表只保存了头节点，所有没有 `push_back` 这类接口。
