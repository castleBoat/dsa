# deque

deque 双端队列，支持队首、队尾插入和删除操作，操作都是 `O(1)` 复杂度。

# 底层实现

使用多个固定大小的元素数组保存实际元素；和指向每个数组的指针数组作为二次索引。

每个数组在内存中地址可以不连续。 当前数组元素存放满之后，添加元素时只需要新申请一个数组，不用像 vector 那样拷贝移动现有元素。


![deque](https://github.com/steveLauwh/SGI-STL/raw/master/The%20Annotated%20STL%20Sources%20V3.3/Other/deque%20DS.PNG)

所以底层的迭代器更复杂，有 first, last, cur, node 四个元素。

SGI STL 中使用 `_map` 指向二次索引数组，每个成员指向一个元素数组(node)

- first: 指向数组起始元素位置
- last: 指向数组末尾元素，数组范围是 [first, last)
- cur: 指向当前元素。在 deque 中有指向队首和 队尾数组的两种迭代器 start, finish。它们 cur 表示不同的意思。 
  - start: [cur, last) 表示队首已经入队的元素，所有 cur 指向有效元素。
  - finish: [first, cur) 表示队尾已经入队的元素，所有 cur 指向队尾元素的下一个位置。
- node: 指向二次索引数组中位置。
