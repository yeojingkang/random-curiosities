#pragma once

#include <cstddef>
#include <memory>
#include <iterator>
#include <format>

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
        constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}

        constexpr explicit vector(const Allocator& alloc) :
            _alloc{alloc}, _sz{0}, _cap{0}, _data{nullptr}
        {}

        explicit vector(size_type count, const Allocator& alloc = Allocator()) :
            _alloc{alloc}, _sz{count}, _cap{count}, _data{alloc_traits::allocate(_alloc, count)}
        {
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::construct(_alloc, _data + i);
        }

        constexpr vector(size_type count, const T& value, const Allocator& alloc = Allocator()) :
            _alloc{alloc}, _sz{count}, _cap{count}, _data{alloc_traits::allocate(_alloc, count)}
        {
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::construct(_alloc, _data + i, value);
        }

        template<typename InputIt>
        constexpr vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()) :
            _alloc{alloc}, _sz{static_cast<size_type>(std::distance(first, last))}, _cap{_sz}, _data{alloc_traits::allocate(_alloc, _cap)}
        {
            std::uninitialized_copy(first, last, _data);
        }

        constexpr vector(const vector& other) :
            _alloc{alloc_traits::select_on_container_copy_construction(other.get_allocator())},
            _sz{other._sz},
            _cap{other._cap},
            _data{alloc_traits::allocate(_alloc, _cap)}
        {
            std::uninitialized_copy_n(other._data, other._sz, _data);
        }

        constexpr vector(vector&& other) noexcept :
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

        constexpr ~vector()
        {
            if (_data == nullptr)
                return;

            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::destroy(_alloc, _data + i);
            alloc_traits::deallocate(_alloc, _data, _cap);
        }

        // Assignments
        /**********************************************************************/
        constexpr vector& operator=(const vector& other)
        {
            if (this == &other)
                return *this;

            // TODO: Implement

            return * this;
        }

        constexpr vector& operator=(vector&& other)
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

        // Element access
        /**********************************************************************/
        constexpr reference operator[](size_type pos) { return _data[pos]; }
        constexpr const_reference operator[](size_type pos) const { return _data[pos]; }

        constexpr reference at(size_type pos)
        {
            check_bounds(pos);
            return (*this)[pos];
        }
        constexpr const_reference at(size_type pos) const
        {
            check_bounds(pos);
            return (*this)[pos];
        }

        constexpr reference front() { return _data[0]; }
        constexpr const_reference front() const { return _data[0]; }
        constexpr reference back() { return _data[_sz - 1]; }
        constexpr const_reference back() const { return _data[_sz - 1]; }
        constexpr pointer data() noexcept { return _data; }
        constexpr const_pointer data() const noexcept{ return _data; }
        /**********************************************************************/

        // Iterators
        /**********************************************************************/
        constexpr iterator begin() noexcept { return _data; }
        constexpr const_iterator begin() const noexcept { return _data; }
        constexpr const_iterator cbegin() const noexcept { return _data; }
        constexpr iterator end() noexcept { return _data + _sz; }
        constexpr const_iterator end() const noexcept { return _data + _sz; }
        constexpr const_iterator cend() const noexcept { return _data + _sz; }
        constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
        constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
        /**********************************************************************/

        // Modifiers
        /**********************************************************************/
        constexpr void push_back(const T& value)
        {
            maybe_expand();
            alloc_traits::construct(_alloc, _data + _sz++, value);
        }
        constexpr void push_back(T&& value)
        {
            maybe_expand();
            alloc_traits::construct(_alloc, _data + _sz++, std::move(value));
        }
        /**********************************************************************/

        // Capacity
        /**********************************************************************/
        constexpr bool empty() const noexcept { return _sz == 0; }
        constexpr size_type size() const noexcept { return _sz; }
        constexpr size_type capacity() const noexcept { return _cap; }
        /**********************************************************************/

        constexpr allocator_type get_allocator() const noexcept { return _alloc; }

        constexpr void swap(vector& other) noexcept
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

        constexpr void maybe_expand()
        {
            if (_sz < _cap)
                return;

            auto newCap = _cap == 0 ? 1 : _cap;
            while (newCap <= _sz) newCap <<= 1; // Keep doubling until large enough
            auto newData = alloc_traits::allocate(_alloc, newCap);
            std::uninitialized_copy_n(_data, _sz, newData);

            std::swap(_cap, newCap);
            std::swap(_data, newData);
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::destroy(_alloc, newData + i);
            alloc_traits::deallocate(_alloc, newData, newCap);
        }

        inline constexpr void check_bounds(size_type pos) const
        {
            if (pos >= _sz)
                throw std::out_of_range{std::format("vector: index {} out of range of size {}", pos, _sz)};
        }
    };
}
