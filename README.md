Lambda
======

Lambda expressions implementation for C++0x and higher, very similar to [Boost Lambda](http://www.boost.org/doc/libs/1_54_0/doc/html/lambda.html).

This implementation uses the following C++0x features:
- rvalue references,
- variadic template arguments,
- return type inference.

Made mainly for fun.

Advantages compared to Boost Lambda:
* generated functors can be called with more than 10 arguments (unlimited arguments supported);
* faster compilation thanks to C++0x features (less overloads to select, less templates to instantiate).

Disadvantages:
* only works on very recent C++ compilers. Works in g++ 4.8.
