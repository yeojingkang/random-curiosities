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
        constexpr void clear() noexcept
        {
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::destroy(_alloc, _data + i);
            _sz = 0;
        }

        template<typename TVal>
        constexpr void push_back(TVal&& value)
        {
            maybe_expand();
            alloc_traits::construct(_alloc, _data + _sz++, std::forward<TVal>(value));
        }
        template<typename... Args>
        constexpr reference emplace_back(Args&&... args)
        {
            maybe_expand();
            alloc_traits::construct(_alloc, _data + _sz, std::forward<Args>(args)...);
            return _data[_sz++];
        }

        template<typename TVal>
        constexpr iterator insert(const_iterator pos, TVal&& value)
        {
            const auto index = pos - cbegin();
            maybe_expand();
            std::uninitialized_move_n(rbegin(), _sz - index, std::prev(rbegin()));
            const auto ins = begin() + index;
            alloc_traits::construct(_alloc, ins, std::forward<TVal>(value));
            ++_sz;
            return ins;
        }
        constexpr iterator insert(const_iterator pos, size_type count, const T& value)
        {
            // TODO: Possible to eliminate moves when expansion is required (Perform action on new mem, then copy old elems to correct new pos)
            const auto index = pos - cbegin();
            maybe_expand(count);

            auto d_it = rbegin();
            std::advance(d_it, -count);
            std::uninitialized_move_n(rbegin(), _sz - index, d_it);
            const auto ins = begin() + index;
            std::uninitialized_fill_n(ins, count, value);
            _sz += count;
            return ins;
        }
        template<typename InputIt>
        constexpr iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            const auto index = pos - cbegin();
            maybe_expand();

            const auto cnt = static_cast<size_type>(std::distance(first, last));
            auto d_it = rbegin();
            std::advance(d_it, -cnt);
            std::uninitialized_move_n(rbegin(), _sz - index, d_it);
            const auto ins = begin() + index;
            std::uninitialized_copy(first, last, ins);
            _sz += cnt;
            return ins;
        }
        constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist)
        {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template<typename... Args>
        constexpr iterator emplace(const_iterator pos, Args&&... args)
        {
            // TODO: Possible to eliminate moves when expansion is required (Perform action on new mem, then copy old elems to correct new pos)
            const auto index = pos - cbegin();
            maybe_expand();

            const auto ins = begin() + index;
            if (ins == end())
                alloc_traits::construct(_alloc, ins, std::forward<Args>(args)...);
            else
            {
                T new_elem(std::forward<Args>(args)...);
                std::uninitialized_move_n(rbegin(), _sz - index, std::prev(rbegin()));
                *ins = std::move(new_elem);
            }

            ++_sz;
            return ins;
        }

        constexpr iterator erase(const_iterator pos)
        {
            alloc_traits::destroy(_alloc, pos);
            const auto ers = const_cast<iterator>(pos); // Forgive me
            for (auto it = ers; it != end() - 1; ++it)
                *it = std::move(*(it + 1));
            --_sz;
            return ers;
        }
        constexpr iterator erase(const_iterator first, const_iterator last)
        {
            if (first == last)
                return const_cast<iterator>(last); // Please forgive me

            const auto cnt = last - first;
            for (auto it = first; it != last; ++it)
                alloc_traits::destroy(_alloc, it);

            auto fr = const_cast<iterator>(last);
            auto to = const_cast<iterator>(first);
            while (fr != end())
                *to++ = std::move(*fr++);
            _sz -= cnt;
            return const_cast<iterator>(first);
        }

        constexpr void pop_back()
        {
            if (!empty())
                alloc_traits::destroy(_alloc, _data + --_sz);
        }
        /**********************************************************************/

        // Capacity
        /**********************************************************************/
        constexpr bool empty() const noexcept { return _sz == 0; }
        constexpr size_type size() const noexcept { return _sz; }
        constexpr size_type capacity() const noexcept { return _cap; }
        constexpr void reserve(size_type new_cap)
        {
            if (new_cap <= _cap)
                return;

            auto new_data = alloc_traits::allocate(_alloc, new_cap);

            /*
             * This is an observation from gcc's implementation, which copies the
             * old data instead of moving them, and it surprised me because moving
             * the data would be a more efficient approach. However, it dawned on
             * me that doing so will violate the requirement of leaving the container
             * in a valid state (i.e. before expansion) if an exception is thrown
             * (e.g. whilst moving an element).
             * 
             * This reveals a possible reason to implement a more efficient custom
             * vector-like container if one can guarantee that the move operation
             * on their elements will never throw exceptions.
             */
            std::uninitialized_copy_n(_data, _sz, new_data);

            std::swap(_cap, new_cap);
            std::swap(_data, new_data);
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::destroy(_alloc, new_data + i);
            alloc_traits::deallocate(_alloc, new_data, new_cap);
        }
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

        constexpr void maybe_expand(size_type expand_by = 1)
        {
            const auto expand_to = _sz + expand_by;
            if (expand_to <= _cap)
                return;

            auto newCap = _cap == 0 ? 1 : _cap;
            while (newCap <= expand_to) newCap <<= 1; // Keep doubling until large enough
            reserve(newCap);
        }

        inline constexpr void check_bounds(size_type pos) const
        {
            if (pos >= _sz)
                throw std::out_of_range{std::format("vector: index {} out of range of size {}", pos, _sz)};
        }
    };
}
