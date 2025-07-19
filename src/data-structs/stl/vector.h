#pragma once

#include <cstddef>
#include <memory>
#include <iterator>

namespace mystl
{
    template<
        typename T,
        typename Allocator = std::allocator<T>
    > class vector
    {
    private:
        using alloc_traits = std::allocator_traits<Allocator>;

    public:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = alloc_traits::pointer;
        using const_pointer = alloc_traits::const_pointer;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // Constructors
        /**********************************************************************/
        vector() : vector(Allocator()) {}
        explicit vector(const Allocator& alloc) :
            _alloc{alloc}, _sz{0}, _cap{0}, _data{nullptr}
        {}

        explicit vector(size_type count, const Allocator& alloc = Allocator()) :
            _alloc{alloc}, _sz{count}, _cap{count}, _data{alloc_traits::allocate(_alloc, count)}
        {
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::construct(_alloc, _data + i);
        }

        vector(size_type count, const T& value, const Allocator& alloc = Allocator()) :
            _alloc{alloc}, _sz{count}, _cap{count}, _data{alloc_traits::allocate(_alloc, count)}
        {
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::construct(_alloc, _data + i, value);
        }

        template<typename InputIt>
        vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()) :
            _alloc{alloc}, _sz{std::distance(first, last)}, _cap{_sz}, _data{alloc_traits::allocate(_alloc, _cap)}
        {
            std::uninitialized_copy(first, last, _data);
        }

        vector(const vector& other) :
            _alloc{alloc_traits::select_on_container_copy_construction(other.get_allocator())},
            _sz{other._sz},
            _cap{other._cap},
            _data{alloc_traits::allocate(_alloc, _cap)}
        {
            std::uninitialized_copy_n(other._data, other._sz, _data);
        }

        vector(vector&& other) :
            _alloc{std::move(other._alloc)},
            _sz{std::move(other._sz)},
            _cap{std::move(other._cap)},
            _data{std::move(other._data)}
        {
            other._sz = 0;
            other._data = nullptr;
        }

        vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
            vector(init.begin(), init.end(), alloc)
        {}
        /**********************************************************************/

        ~vector()
        {
            if (_data == nullptr)
                return;

            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::destroy(_alloc, _data + i);
            alloc_traits::deallocate(_alloc, _data, _cap);
        }

        // Assignments
        /**********************************************************************/
        vector& operator=(const vector& other)
        {
            if (this == &other)
                return *this;

            // TODO: Implement

            return * this;
        }

        vector& operator=(vector&& other)
        {
            if (this == &other)
                return *this;

            _alloc = std::move(other._alloc);
            _sz = std::move(other._sz);
            _cap = std::move(other._cap);
            _data = std::move(other._data);
            return *this;
        }
        /**********************************************************************/

        reference operator[](size_type pos) { return _data[pos]; }
        const_reference operator[](size_type pos) const { return _data[pos]; }

        void push_back(const T& value)
        {
            maybe_expand();
            alloc_traits::construct(_alloc, _data + _sz++, value);
        }
        void push_back(T&& value)
        {
            maybe_expand();
            alloc_traits::construct(_alloc, _data + _sz++, std::move(value));
        }

        bool empty() const { return _sz == 0; }
        size_type size() const { return _sz; }
        allocator_type get_allocator() const { return _alloc; }

        void swap(vector& other)
        {
            if constexpr (alloc_traits::propagate_on_container_swap::value)
                std::swap(_alloc, other._alloc);

            std::swap(_sz, other._sz);
            std::swap(_cap, other._cap);
            std::swap(_data, other._data);
        }

    private:
        allocator_type _alloc;
        size_type _sz, _cap;
        pointer _data;

        void maybe_expand()
        {
            if (_sz < _cap)
                return;

            auto newCap = _cap == 0 ? 1 : _cap;
            while (newCap < _sz) newCap <<= 1; // Keep doubling until large enough
            auto newData = alloc_traits::allocate(_alloc, newCap);
            std::uninitialized_move_n(_data, _sz, newData);

            std::swap(_cap, newCap);
            std::swap(_data, newData);
            // Shouldn't need to destroy old array elements
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::destroy(_alloc, newData + i);
            alloc_traits::deallocate(_alloc, newData, newCap);
        }
    };
}
