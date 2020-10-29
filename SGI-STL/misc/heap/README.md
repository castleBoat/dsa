
# heap

提供了 is_heap, make_heap, push_heap, pop_heap, sort_heap 接口。

堆中使用连续空间地址，默认使用 `less` 做为排序规则，返回 false 的元素放在前面，所以最大元素在最前面。为最大堆。


- is_heap: 判断 [first, last) 是否是堆
- make_heap: 把 [first, last) 区间构造成堆
- sort_heap: 输入 [first, last) 区间堆时，堆上做排序
- push_heap: [first, last) 插入 last-1 元素到堆 [first, last-1) 中。完成了 [first, last)
- pop_heap: 从堆中弹出元素。first 是堆顶，首先与 last-1 元素交换，然后 [first, last-1) 调整为堆。

## is_heap

双指针，分别指向父节点和子节点。从前到后一次判断父节点和子节点是否满足堆要求。

实际使用单指针，指向父节点，由父节点位置求出子节点位置也行。

## push_heap

尾部加入元素到堆，

1. 加入元素与父节点比较大小，如果大于则上浮。
2. 不断进行这个过程，直到加入元素**上升**到合适位置或者到达堆顶。

## __adjust_heap

堆元素调整元素，声明格式是：

```c++
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
      _Distance __len, _Tp __value, _Compare __comp)
```

调整以空洞位置 (__holeIndex) 为顶点的堆，并且把 __value 元素插入堆。具体做法是：

1. 堆顶做为空洞位置，较大子节点赋值到空洞位置
2. 不断进行这个过程，直到空洞位置下沉到最下一层
3. 调用 push_heap 加入的元素从空洞位置添加到堆

空洞位置下沉需要遍历一次堆，push_heap 也需要遍历一次堆。总共需要遍历两次堆。

如果 __value 就在 空洞位置，实际可以直接把 __value 元素下沉到合适位置。只需要遍历一次堆。

1. 堆顶元素与子节点中较大元素比较，如果小于则与子孩子交换。
2. 不断进行这个过程，直到元素**下沉**到合适位置或者到达最下一层。

## pop_heap

弹出堆顶元素。堆顶与 last-1 位置交换，然后堆顶位置元素调整到合适位置。

堆顶做为空洞位置然后调用 __adjust_heap 把 __value 值从空洞位置调整到合适位置。

## make_heap

1. 从最后一个非叶子节点调用 __adjust_heap 调整堆。
2. 一直调整到堆顶节点

另一种做法：

1. 从下标 1 开始 push_heap 元素加入堆
2. 直到最后一个元素加入堆

## sort_heap

不断的弹出堆顶元素, 直到堆为空。
