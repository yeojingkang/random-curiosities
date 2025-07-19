#include <vector>
#include "vector.h"

#include <iostream>

class C
{
public:
    C() : x(0) { std::cout << "C default ctor" << std::endl; }
    C(const C& c) : x(c.x) {  std::cout << "C copy ctor" << std::endl; }
    C(C&& c) : x(std::move(c.x)) {  std::cout << "C move ctor" << std::endl; }
    ~C() { std::cout << "C dtor" << std::endl; }

    explicit C(int i) : x(i) { std::cout << "value ctor: " << i << std::endl; }

    C& operator=(const C& c)
    {
        if (&c == this)
            return *this;

        x = c.x;
        std::cout << "C copy assignment" << std::endl;
        return *this;
    }

    C& operator=(C&& c)
    {
        x = std::move(c.x);
        std::cout << "C move assignment" << std::endl;
        return *this;
    }

    int x;
};

template<template<typename> typename T>
void testC(const std::string& name)
{
    std::cout << name << ":" << std::endl;

    T<C> cv(2);
    T<C> cv2(3);
    auto mcv = std::move(cv);
    auto ccv2 = cv2;

    std::cout << "push_back:" << std::endl;
    T<C> pv;
    for (auto i = 0; i < 5; ++i)
        pv.push_back(C{i});
    for (auto i = 0; i < 5; ++i)
        std::cout << pv[i].x << "  ";
    std::cout << std::endl;
}

int main()
{
    mystl::vector<int> a(10);
    mystl::vector<int> b;


    testC<mystl::vector>("mystl vector");

    std::cout << std::endl;

    testC<std::vector>("std vector");
}
