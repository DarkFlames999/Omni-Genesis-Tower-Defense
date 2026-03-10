#include <iostream>

#ifndef FOO_H
#define FOO_H
void foo() {
    std::cout << "Hello from foo\n" << std::endl;
    foo();
}

#endif