Lambda
======

Lambda expressions implementation for C++0x and higher, very similar to [Boost Lambda](http://www.boost.org/doc/libs/1_54_0/doc/html/lambda.html).

This implementation uses the following C++0x features:
- rvalue references,
- variadic template arguments,
- return type inference.

Made mainly for fun.

Comparison
----------

Advantages compared to Boost Lambda:
* generated functors can be called with more than 10 arguments (unlimited arguments supported);
* faster compilation thanks to C++0x features (less overloads to select, less templates to instantiate).

Disadvantages:
* only works on very recent C++ compilers. Works in g++ 4.8.

FAQ
---

**Why make another lambda expressions implementation when C++0x supports native lambdas?**

Because they suck, you have to either manually specify captured variables or use the verbose `[=]` or `[&]` capture clause. Not to talk about `decltype`.

Also, I don't agree with C++0x natively supporting lambda expressions because there were _already_ lambda expressions in the language, through manual implementations like Boost Lambda and this one. I would have pretty much loved if the standard committee concentrated their efforts in improving the necessary language features to implement more effective lambdas. For example, I would love the direct assignment `=` operator to be like other operators and not need to be a class member so that we didn't have to write:

```C++
var(x) = _1;
```

and we could directly write:

```C++
x = _1;
```

**But native lambdas are just fine, who cares what _you_ think?**

I do, I made this for fun.

**You said `decltype` sucks, but you did use it in your code.**

I only approve return type inference but I still think `decltype` sucks. In other words, I only allow myself to use `decltype` along with `auto` return type and only using the same expression returned by the function body.
