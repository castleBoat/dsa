# functor

functor 是实现了 `operator()` 接口的对象。它们虽然是类对象，但是支持当做函数使用，所以称为 functor。

functor 在 algorithm template 中广泛使用，比如：

```c++
std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::plus<int>());
```

其中 `std::plus` 就是构造了 functor 对象，它接受两个 int 类型的参数，返回 int 类型参数。

`stl_function.h` 头文件定义了常用的 functor。 

# base functor

定义了两种基础类型 `unary_function`, `binary_function`，前者接受一个类型参数，返回一个类型参数；后者接受两个类型参数，返回一个类型参数。

后续的 functor 都是继承这两种中的一个。

# arithmetic

算术相关的 functor:

- plus: binary_function，相加
- mius: binary_function，相减
- multiplies: binary_function，相乘
- modulus: binary_function，取余
- negate: unary_function，取负数

算术相关的 functor 参数要求是同类型，返回值类型也是参数类型。

# comparisons

比较相关的 functor，参数类型要求是同类型，返回值是 bool：

- euqal_to: binary_function, 等于判断
- not_equal_to: binary_function, 不等于判断
- greater: binary_function, 大于判断
- less: binary_function, 小于判断
- greater_equal: binary_function, 大于或等于判断
- less_equal: binary_function, 小于或等于判断

less 在 map 中作用默认的比较 functor.

# logical operations

逻辑相关的 functor, 参数类型要求是同类型，返回值是 bool:

- logical_and: binary_function, 逻辑与
- logical_or: binary_function, 逻辑或
- logical_not: unary_function, 逻辑取反

按位操作相关 functor, 参数要求是同类型，返回值类型也是参数类型。

- bit_and: binary_function, 按位与
- bit_or: binary_function, 按位或
- bit_xor: binary_function, 按位异或
- bit_nor: unary_function, 按位取反

# adaptor pointers function 

封装函数指针的 functor。把函数指封装成 functor 的适配器。

比如上面的 transform 需要使用自己实现操作函数，但是它接受的参数是 functor，这时就可以使用 adaptor。

这里的函数指针除一般的函数指针，也可以封装类成员函数的做为 functor。

- pointer_to_unary_function: 单个参数的函数 adaptor functor
- pointer_to_binary_function: 两个参数的函数 adaptor functor
- ptr_fun: 返回上面的 functor

- mem_fun_t: 类成员函数参数位空的 adaptor functor
- mem_fun1_t: 类成员函数有一个参数的 adaptor functor
- mem_fun: 返回 mem_func_t, mem_fun1_t

```c++
void foo2(int) {
    std::cout << "foo2(int)" << std::endl;
}
std::pointer_to_unary_function<int, void> pf = std::ptr_fun(&foo2);
// auto pf = std::ptr_fun(&foo2);
pf(1);

// ----
class Base {
public:
    void foo() {
        std::cout << "Base::foo()" << std::endl;
    }
};

Base b;
std::mem_fun_t<void, Base> mf1 = std::mem_fun(&Base::foo);
// auto Base> mf1 = std::mem_fun(&Base::foo);
mf1(&b);
```

# misc

- `_Select1st`: unary_function, 接受 pair，返回 pair.first 的 functor
- `_Select2nd`: unary_function, 接受 pair，返回 pair.second 的 functor

map 中保存的是 `pair<key, value` 结构，使用 `_Select1st` 判断两个 pair 的 key 大小。

