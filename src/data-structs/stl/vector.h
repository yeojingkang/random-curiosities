#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <iomanip>

namespace CS170
{
    template<typename T>
    class Vector
    {
    public:
        Vector() :
            _data{nullptr},
            _size{0},
            _capacity{0},
            _alloc_count{0}
        {}

        Vector(const T* a, std::size_t sz) :
            _data{nullptr},
            _size{sz},
            _capacity{0},
            _alloc_count{0}
        {
            maybe_expand();
            std::memcpy(_data, a, sz * sizeof(T)); // Note: didn't call ctor for elements :/
        }

        Vector(const Vector& r) :
            _data{new T[r._capacity]},
            _size{r._size},
            _capacity{r._capacity},
            _alloc_count{1}
        {
            std::memcpy(_data, r._data, r._size * sizeof(T)); // Note: same here...
        }

        ~Vector() { delete[] _data; }

        Vector& operator=(const Vector& r)
        {
            if (this == &r)
                return *this;

            clear();

            _size = r._size;
            _capacity = r._capacity;
            _alloc_count = r._alloc_count;

            delete[] _data;
            _data = new T[_capacity];
            std::memcpy(_data, r._data, r._size * sizeof(T));

            return *this;
        }

        void push_back(const T& v)
        {
            maybe_expand();
            _data[_size++] = v;
        }

        void push_front(const T& v)
        {
            insert(v, 0);
        }

        void pop_back() { _data[--_size].~T(); }
        void pop_front() { erase(0); }

        void remove(const T& val)
        {
            for (std::size_t i = 0; i < _size; ++i)
                if (_data[i] == val)
                {
                    erase(i);
                    return;
                }
        }

        void insert(const T& v, std::size_t index)
        {
            maybe_expand();
            for (auto i = _size; i > index; --i)
                _data[i] = _data[i - 1];
            _data[index] = v;
            ++_size;
        }

        void clear()
        {
            delete[] _data;
            _data = nullptr;
            _capacity = _size = 0;
        }

        Vector& operator+=(const Vector& r)
        {
            const auto oldSize = _size;
            _size += r._size;
            maybe_expand();

            std::memcpy(_data + oldSize, r._data, r._size * sizeof(T));

            return *this;
        }

        bool empty() const { return _size == 0; }
        std::size_t inline size() const { return _size; }
        std::size_t inline capacity() const { return _capacity; }
        std::size_t inline allocations() const { return _alloc_count; }

        T& operator[] (int i) { return _data[i]; }
        const T& operator[] (int i) const { return _data[i]; }

    private:
        T* _data;
        std::size_t _size, _capacity, _alloc_count;

        void maybe_expand()
        {
            if (_size < _capacity)
                return;

            _capacity = _capacity == 0 ? 1 : (_capacity << 1);
            while (_capacity < _size)
                _capacity <<= 1;

            auto *newData = new T[_capacity];

            if (_data != nullptr)
                std::memcpy(newData, _data, _size * sizeof(T));

            delete[] _data;
            _data = newData;

            ++_alloc_count;
        }

        void erase(std::size_t index)
        {
            _data[index].~T(); // Make sure to destroy the removed element first
            --_size;
            for (std::size_t i = index; i < _size; ++i)
                _data[i] = _data[i + 1];
        }
    };

    template<typename T>
    Vector<T> operator+(const Vector<T>& l, const Vector<T>& r)
    {
        auto ret{l};
        ret += r;
        return ret;
    }

    template<typename T>
    void Print(const Vector<T> &v)
    {
        for (std::size_t i = 0; i < v.size(); ++i)
            std::cout << v[i] << "  ";

        std::cout
            << "("
            << "size=" << v.size() << ", "
            << "capacity=" << v.capacity() << ", "
            << "allocs=" << v.allocations() << ")" << std::endl;
    }

    template<>
    void Print(const Vector<float> &v)
    {
        for (std::size_t i = 0; i < v.size(); ++i)
            std::cout << std::setprecision(3) << std::setw(5) << v[i] << "  ";

        std::cout
            << "("
            << "size=" << v.size() << ", "
            << "capacity=" << v.capacity() << ", "
            << "allocs=" << v.allocations() << ")" << std::endl;
    }

    template<>
    void Print(const Vector<unsigned char> &v)
    {
        for (std::size_t i = 0; i < v.size(); ++i)
            std::cout << static_cast<unsigned>(v[i]) << "  ";

        std::cout
            << "("
            << "size=" << v.size() << ", "
            << "capacity=" << v.capacity() << ", "
            << "allocs=" << v.allocations() << ")" << std::endl;
    }

    template<>
    void Print(const Vector<double> &v)
    {
        for (std::size_t i = 0; i < v.size(); ++i)
            std::cout
                << std::left << std::setprecision(5) << std::setw(7)
                << v[i] << "  ";

        std::cout
            << "("
            << "size=" << v.size() << ", "
            << "capacity=" << v.capacity() << ", "
            << "allocs=" << v.allocations() << ")" << std::endl;
    }
}
