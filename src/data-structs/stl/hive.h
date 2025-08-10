#pragma once

#include <memory>

namespace mystl
{
    struct hive_limits
    {
        std::size_t min, max;
        hive_limits(std::size_t minimum, std::size_t maximum) noexcept :
            min{minimum}, max{maximum}
        {}
    };

    template<
        typename T,
        typename Allocator = std::allocator<T>
    >
    class hive
    {
    private:
        using alloc_traits = std::allocator_traits<Allocator>;

        struct block
        {
            pointer data;
            skipfield_pointer skip;
            size_type cap, sz;
            block_pointer prev, next;
            block_pointer next_w_space; // Next block with free space

            block(allocator_type alloc, skipfield_allocator_type skipfield_alloc, size_type capacity, block_pointer prev = nullptr) :
                data{alloc_traits::allocate(alloc, capacity)},
                skip{skipfield_alloc_traits::allocate(skipfield_alloc, capacity)},
                cap{capacity}, sz{0},
                prev{prev}, next{nullptr},
                next_w_space{nullptr}
            {}
        };

    public:
        using value_type = T;
        using allocator_type = Allocator;
        using pointer = alloc_traits::pointer;
        using const_pointer = alloc_traits::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        // TODO: Define the iterator-related types
        //using difference_type = // This is the diff type of iterator & const_iterator
        //using iterator = 
        //using const_iterator = 
        //using reverse_iterator = 
        //using const_reverse_iterator = 

        hive() noexcept(noexcept(Allocator)) : hive(Allocator()) {}
        explicit hive(const Allocator& alloc) :
            _alloc{alloc}, _limits{MIN_LIMIT, MAX_LIMIT},
            _blocks{nullptr}, _blocks_w_space{nullptr}, _nx_block_sz{MIN_LIMIT}
        {}

        ~hive()
        {
            // TODO: Destroy elements & deallocate blocks (and data inside)
        }

        template<typename... Args>
        iterator emplace(Args&&... args)
        {
            // TODO: Implement
        }
        iterator insert(const T& value)
        {
            // TODO: Implement
        }
        iterator insert(T&& value)
        {
            // TODO: Implement
        }

    private:
        using block_allocator_type = typename alloc_traits::template rebind_alloc<block>;
        using skipfield_allocator_type = typename alloc_traits::template rebind_alloc<size_type>;

        using block_alloc_traits = std::allocator_traits<block_allocator_type>;
        using skipfield_alloc_traits = std::allocator_traits<skipfield_allocator_type>;

        using block_pointer = block_alloc_traits::pointer;
        using skipfield_pointer = skipfield_alloc_traits::pointer;

        constexpr size_type GROWTH_FACTOR = 2;
        constexpr size_type MIN_LIMIT = 8, MAX_LIMIT = std::numeric_limits<size_type>::max();

        allocator_type _alloc;
        block_allocator_type _block_alloc;
        skipfield_allocator_type _skipfield_alloc;
        hive_limits _limits;
        block* _blocks, _blocks_w_space;
        size_type _nx_block_sz;

        // Gets the next free element in the mem. blocks. Allocs a new block if every block is full
        iterator get_free_elem()
        {
            if (_blocks_w_space == nullptr)
            {
                // No free block
                auto new_block = block_alloc_traits::allocate(_block_alloc, 1);
                block_alloc_traits::construct(_block_alloc, new_block, _alloc, _skipfield_alloc, _nx_block_sz)

                if (_blocks == nullptr)
                    _blocks = new_block; // 1st allocated block

                _nx_block_sz *= GROWTH_FACTOR; // TODO: Bound to limits
            }
        }
    };
}
