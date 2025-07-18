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
        // TODO: Need to define own iterator class?
        //using iterator = int;
        //using const_iterator = int;
        //using reverse_iterator = std::reverse_iterator<iterator>;
        //using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        vector() : vector(Allocator()) {}
        explicit vector(const Allocator& alloc) :
            _alloc{alloc}, _data{nullptr}, _sz{0}, _cap{0}
        {}

        explicit vector(size_type count, const Allocator& alloc = Allocator()) :
            _alloc{alloc}, _data{alloc_traits::allocate(_alloc, count)}, _sz{count}, _cap{count}
        {
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::construct(_alloc, _data + i);
        }

        ~vector()
        {
            for (size_type i = 0; i < _sz; ++i)
                alloc_traits::destroy(_alloc, _data + i);
            alloc_traits::deallocate(_alloc, _data, _cap);
        }

    private:
        allocator_type _alloc;
        pointer _data;
        size_type _sz, _cap;
    };
}
