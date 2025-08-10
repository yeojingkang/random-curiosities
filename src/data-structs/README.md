# Data Structures
The bread and butter of programming.

## Red-black tree
`rb-tree/`

I learned about this data structure in school, but never implemented it. Once I found out that Linux uses it everywhere in its kernel, I knew it's high time I wrote one to understand it better.

## C++ Standard Template Library (STL)
`stl/`

The STL contains a suite of versatile data structures and algorithms (and more), and what better way to understand their interface and inner workings than by implementing them myself? I wrote some of these containers during my undergrad days, but they were basic and only fulfilled a subset of the interface. This time I will attempt to implement the full interface as an exercise on template programming.

### vector

Conceptually simple, just a dynamic array that changes size in runtime.

As the first container to be implemented, I went into it thinking it'd be a quick exercise that'd take me only 1 or 2 days to complete. On the contrary, I often got stuck, and spent time relearning the basics (e.g. copy/move semantics, variadic templates) or reading about new features (e.g. **comparisons in C++20**). This was also the first time I implemented an **AllocatorAware** container, and I wasn't familiar with handling the allocator during copies/moves. I now realise that a simple container can be a lot more complicated than I thought, especially when it needs to be a general purpose solution offering various features.

My implementation is far from perfect and probably contains more mistakes than I'm aware of (I'll need more tests to verify). In fact, there are still functions I have yet to implement which I will eventually revisit when I gain the knowledge required for them. However, working on this has taught me a lot, and I'm sure I'll learn even more as I move on to the other containers.

**Revised knowledge**:
- Lvalues/rvalues
- Iterators
- `constexpr`
- Rule of 0/3/5
- Universal references
- Perfect forwarding
- Templates
- Variadic templates

**New things I've learned**:
- Conditional `noexcept`
- Comparisons in C++20 (3-way comp., Primary/secondary, rewritten/reordered operators)
- AllocatorAware containers
- A few algorithms in `<algorithm>`
- `auto` function return type
- `std::format`

### hive

As of the time I wrote this, hive has yet to be added to the STL. I will implement this data structure as specified by the [original paper](https://isocpp.org/files/papers/P0447R19.html).
