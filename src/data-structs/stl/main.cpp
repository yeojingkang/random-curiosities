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

std::ostream& operator<<(std::ostream& os, const C& c)
{
    os << c.x;
    return os;
}

template<typename T>
void testCtor()
{
    std::cout << "ctors:" << std::endl;
    T v(2);
    T v2(3);
    auto mcv = std::move(v);
    auto cv2 = v2;
}

template<typename T>
void testPushBack()
{
    std::cout << "push_back:" << std::endl;
    T pv;
    for (auto i = 0; i < 5; ++i)
    {
        pv.push_back(C{i});
    }
    for (auto i = 0; i < 5; ++i)
        std::cout << pv[i] << "  ";
    std::cout << std::endl;
}

template<typename T>
void testElementAccess()
{
    std::cout << "element access:" << std::endl;
    const T cv(3);
    T v{C{1},C{2},C{3}};
    std::cout << "cv: " << cv.at(0) << "  " << cv[1] << std::endl;
    std::cout << "pv: " << v.at(0) << "  " << v[1] << std::endl;
    //std::cout << "out_of_range: " << v.at(5) << std::endl; // out of range

    std::cout << "front: " << cv.front() << ", " << v.front() << std::endl;
    std::cout << "back: " << cv.back() << ", " << v.back() << std::endl;
    std::cout << "data: " << (&cv[0] == cv.data()) << " " << (&v[0] == v.data()) << std::endl;
}

template<typename T>
void testIterators()
{
    std::cout << "iterators:" << std::endl;

    const T cv(5);
    T v{C{1},C{2},C{3}, C{4}, C{5}};

    std::cout << "cv normal: ";
    for (auto it = std::cbegin(cv); it != std::cend(cv); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;

    std::cout << "v normal: ";
    for (auto it = std::begin(v); it != std::end(v); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;

    std::cout << "cv rev: ";
    for (auto it = std::crbegin(cv); it != std::crend(cv); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;

    std::cout << "v rev: ";
    for (auto it = std::rbegin(v); it != std::rend(v); ++it)
        std::cout << *it << "  ";
    std::cout << std::endl;
}

template<template<typename> typename T>
void testC(const std::string& name)
{
    std::cout << name << ":" << std::endl;

    testCtor<T<C>>();
    std::cout << std::endl;

    testPushBack<T<C>>();
    std::cout << std::endl;

    testElementAccess<T<C>>();
    std::cout << std::endl;

    testIterators<T<C>>();
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
