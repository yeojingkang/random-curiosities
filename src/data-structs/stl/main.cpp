#include <vector>
#include "vector.h"

#include <iostream>
#include <tuple>

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
    const C pushC{7};
    for (auto i = 0; i < 5; ++i)
        pv.push_back(C{i});
    std::cout << "push_back lvalue..." << std::endl;
    pv.push_back(pushC);

    for (auto i = 0; i < 6; ++i)
        std::cout << pv[i] << "  ";
    std::cout << std::endl;


    std::cout << "inserting values at pos 1 & 3..." << std::endl;
    const auto res1 = pv.insert(std::cbegin(pv) + 1, C{19});
    const C insC{21};
    const auto res2 = pv.insert(std::cbegin(pv) + 3, insC);
    std::cout << *res1 << ", " << *res2 << std::endl;
    for (const auto &i : pv)
        std::cout << i << "  ";
    std::cout << std::endl;

    std::cout << "inserting multiple counts of value at pos 5..." << std::endl;
    pv.insert(std::cbegin(pv) + 5, 5, C{42});
    for (const auto &i : pv)
        std::cout << i << "  ";
    std::cout << std::endl;
}

template<template<typename> typename T>
void testEmplaceBack()
{
    std::cout << "emplace_back:" << std::endl;
    T<std::tuple<int, const char*, double>> v;
    const auto& [i1, c1, d1] = v.emplace_back(1, "Hello", 3.14);
    std::cout << "1st emplace: " << i1 << ", " << c1 << ", " << d1 << std::endl;
    const auto& [i2, c2, d2] = v.emplace_back(2, "World", 2.71);
    std::cout << "2nd emplace: " << i2 << ", " << c2 << ", " << d2 << std::endl;

    for (const auto& [i, c, d] : v)
        std::cout << i << ", " << c << ", " << d << std::endl;

    std::cout << "emplacing at positions..." << std::endl;
    T<std::pair<C, int>> c;
    c.emplace(c.begin(), C{4}, 1);
    c.emplace(c.begin(), C{8}, 3);
    c.emplace(c.end(), C{9}, 5);
    const auto res = c.emplace(std::next(c.begin()), C{1}, 7);
    std::cout << "last emplace: " << res->first << ", " << res->second << std::endl;
    std::cout << "elems: " << std::endl;
    for (const auto &[f, s] : c)
        std::cout << "\t" << f << ", " << s << std::endl;
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

template<typename T>
void testCapacity()
{
    std::cout << "capacity:" << std::endl;

    T v;
    constexpr auto cap = 5;

    std::cout << v.capacity() << " ";
    v.reserve(cap);
    std::cout << v.capacity() << " ";
    v.reserve(cap - 3);
    std::cout << v.capacity() << std::endl;

    for (auto i = 0; i < cap; ++i)
    {
        v.push_back(C{i});
        v.push_back(C{i << 1});
        v.push_back(C{i << 2});
    }

    std::cout << "cap after push_backs: " << v.capacity() << std::endl;

    for (const auto &c : v)
        std::cout << c.x << "  ";
    std::cout << std::endl;
}

template<template<typename> typename T>
void testC(const std::string& name)
{
    std::cout
        << "-----------------------" << name << "-----------------------"
        << std::endl;

    testCtor<T<C>>();
    std::cout << std::endl;

    testPushBack<T<C>>();
    std::cout << std::endl;

    testEmplaceBack<T>();
    std::cout << std::endl;

    testElementAccess<T<C>>();
    std::cout << std::endl;

    testIterators<T<C>>();
    std::cout << std::endl;

    testCapacity<T<C>>();
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
