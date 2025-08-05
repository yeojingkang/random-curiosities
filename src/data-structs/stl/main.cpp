#include <vector>
#include "vector.h"

#include <iostream>
#include <tuple>
#include <string_view>
#include <compare>

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

    bool operator==(const C& other) const = default;
    std::strong_ordering operator<=>(const C& other) const = default;

    int x;
};

std::ostream& operator<<(std::ostream& os, const C& c)
{
    os << c.x;
    return os;
}

template<template<typename> typename T>
std::ostream& operator<<(std::ostream& os, const T<C>& v)
{
    os
        << "{ sz " << v.size() << ", "
        << "cap " << v.capacity() << ": ";
    for (const auto &i : v)
        os << i << "  ";
    os << " }";
    return os;
}

namespace VectorTests
{
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

        std::cout << "size " << pv.size() << ": ";
        for (auto i = 0; i < 6; ++i)
            std::cout << pv[i] << "  ";
        std::cout << std::endl;


        std::cout << "inserting values at pos 1 & 3..." << std::endl;
        const auto res1 = pv.insert(std::cbegin(pv) + 1, C{19});
        const C insC{21};
        const auto res2 = pv.insert(std::cbegin(pv) + 3, insC);
        std::cout << *res1 << ", " << *res2 << std::endl
                << pv << std::endl;

        std::cout << "inserting multiple counts of value at pos 5..." << std::endl;
        pv.insert(std::cbegin(pv) + 5, 5, C{42});
        std::cout << pv << std::endl;

        std::cout << "erase:" << std::endl;
        std::cout << "erasing values at pos 1 & 3..." << std::endl;
        auto ers1 = pv.erase(std::cbegin(pv) + 1);
        auto ers2 = pv.erase(std::cbegin(pv) + 3);
        std::cout << *(ers1++) << ", " << *(ers2++) << std::endl;
        std::cout << *ers1 << ", " << *ers2 << std::endl
                << pv << std::endl;

        std::cout << "erasing range 3 to 8..." << std::endl;
        auto rers1 = pv.erase(std::cbegin(pv) + 3, std::cbegin(pv) + 8);
        std::cout << *rers1 << ", " << *(rers1 + 1) << std::endl
                << pv << std::endl;

        std::cout << "erasing range 3 to end..." << std::endl;
        auto rers2 = pv.erase(std::cbegin(pv) + 3, std::cend(pv));
        std::cout << "returned itr is end(): " << (rers2 == std::cend(pv)) << std::endl
                << pv << std::endl;

        std::cout << "pop_back:" << std::endl;
        pv.pop_back();
        std::cout << pv << std::endl;
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
        auto res = c.emplace(std::next(c.begin()), C{1}, 7);
        std::cout << "last emplace: " << res->first << ", " << res->second << std::endl;
        ++res;
        std::cout << "next elem: " << res->first << ", " << res->second << std::endl;
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

        std::cout << v << std::endl;
    }

    template<typename T>
    void testAssignment()
    {
        std::cout << "assignment:" << std::endl;

        T c1{C{1}, C{2}, C{3}};
        T c2{C{4}, C{5}, C{6}};
        T c3{C{111}};
        T c4{C{200}, C{201}, C{202}, C{203}};
        T m1{C{7}, C{8}, C{9}};
        T m2{C{10}, C{11}, C{12}};
        T m3{C{0}};
        T m4{C{30}, C{123}, C{44}, C{998}};

        std::cout << "copying same size..." << std::endl;
        c1 = c2;
        std::cout << "moving same size..." << std::endl;
        m1 = std::move(m2);
        std::cout
            << "c1: " << c1 << std::endl
            << "c2: " << c2 << std::endl
            << "m1: " << m1 << std::endl
            << "m2: " << m2 << std::endl;

        std::cout << "copying smaller..." << std::endl;
        c1 = c3;
        std::cout << "moving smaller..." << std::endl;
        m1 = std::move(m3);
        std::cout
            << "c1: " << c1 << std::endl
            << "c3: " << c3 << std::endl
            << "m1: " << m1 << std::endl
            << "m3: " << m3 << std::endl;

        std::cout << "copying larger..." << std::endl;
        c1 = c4;
        std::cout << "moving larger..." << std::endl;
        m1 = std::move(m4);
        std::cout
            << "c1: " << c1 << std::endl
            << "c4: " << c4 << std::endl
            << "m1: " << m1 << std::endl
            << "m4: " << m4 << std::endl;

        std::cout << "moving from real temp..." << std::endl;
        m1 = T{C{-234}};
        std::cout << "m1: " << m1 << std::endl;
    }

    template<typename T>
    void testResize()
    {
        std::cout << "resize:" << std::endl;

        T v{C{1}, C{2}, C{3}};

        std::cout << "resize same size..." << std::endl;
        v.resize(v.size());
        std::cout << v << std::endl;

        std::cout << "resize smaller..." << std::endl;
        v.resize(1);
        std::cout << v << std::endl;

        std::cout << "resize larger..." << std::endl;
        v.resize(5);
        std::cout << v << std::endl;

        std::cout << "resize smaller w value..." << std::endl;
        v.resize(2, C{9});
        std::cout << v << std::endl;

        std::cout << "resize larger w value..." << std::endl;
        v.resize(8, C{8});
        std::cout << v << std::endl;
    }

    template<typename T>
    void testComparisons()
    {
        std::cout << "comparisons:" << std::endl;

        T v1{C{1}, C{2}, C{3}};
        T v2{C{1}, C{2}, C{3}};
        T v3{C{2}, C{1}, C{3}};
        T v4{C{0}};
        T v5{C{6}, C{7}, C{8}, C{4}, C{5}};

        auto comp = [](const auto& a, const auto& b, std::string_view an, std::string_view bn)
        {
            std::cout
                << an << " == " << bn << ": " << (a == b) << std::endl
                << an << " != " << bn << ": " << (a != b) << std::endl
                << an << "  > " << bn << ": " << (a > b) << std::endl
                << an << " >= " << bn << ": " << (a >= b) << std::endl
                << an << "  < " << bn << ": " << (a < b) << std::endl
                << an << " <= " << bn << ": " << (a <= b) << std::endl
                << std::endl;
        };

        comp(v1, v2, "v1", "v2");
        comp(v1, v3, "v1", "v3");
        comp(v1, v4, "v1", "v4");
        comp(v1, v5, "v1", "v5");
    }

    template<template<typename> typename T>
    void testC(const std::string& name)
    {
        std::cout
            << "-----------------------" << name << "-----------------------"
            << std::endl;

        //testCtor<T<C>>();
        //std::cout << std::endl;

        //testPushBack<T<C>>();
        //std::cout << std::endl;

        //testEmplaceBack<T>();
        //std::cout << std::endl;

        //testElementAccess<T<C>>();
        //std::cout << std::endl;

        //testIterators<T<C>>();
        //std::cout << std::endl;

        //testCapacity<T<C>>();
        //std::cout << std::endl;

        //testAssignment<T<C>>();
        //std::cout << std::endl;

        //testResize<T<C>>();
        //std::cout << std::endl;

        testComparisons<T<C>>();
        std::cout << std::endl;
    }

    void runTests()
    {
        testC<mystl::vector>("mystl vector");
        std::cout << std::endl;
        testC<std::vector>("std vector");
    }
}

int main()
{
    VectorTests::runTests();
}
