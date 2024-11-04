#include <iostream>
#include "test.h"
#include <fmt/core.h>

void printHello() {
    std::cout << "Hello from test.cpp!" << std::endl;
}

int main() {
    printHello();
    fmt::print("Hello from fmt!\n");
    return 0;
}