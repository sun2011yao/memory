#ifndef FOONATHAN_MEMORY_DETAILL_FREE_LIST_HPP_INCLUDED
#define FOONATHAN_MEMORY_DETAILL_FREE_LIST_HPP_INCLUDED

#include <cstddef>

namespace foonathan { namespace memory
{
    /// \cond impl
    namespace detail
    {
        // manages free memory blocks for a pool
        // they are stored in a list and returned for subsequent allocations
        // there are two versions for the functions: ordered and non ordered
        // non ordered is faster, because it does not keep the ist sorted
        // ordered allows arrays because multiple free blocks are stored after each other
        class free_memory_list
        {
        public:
            // minimum element size
            static constexpr auto min_element_size = sizeof(char*);
            
            //=== constructor ===//
            free_memory_list(std::size_t el_size) noexcept;
            
            // does not own memory!
            free_memory_list(std::size_t el_size,
                             void *mem, std::size_t size) noexcept;

            //=== insert/allocation/deallocation ===//
            // inserts a new memory block, by splitting it up and setting the links
            // does not own memory!
            void insert(void *mem, std::size_t size) noexcept;
            void insert_ordered(void *mem, std::size_t size) noexcept;

            // returns single block from the list
            // pre: !empty()
            void* allocate() noexcept;
            
            // returns the start to multiple blocks after each other
            // pre: !empty()
            // can return nullptr if no block found
            // won't necessarily work if non-ordered functions are called
            void* allocate(std::size_t n) noexcept;

            void deallocate(void *ptr) noexcept;
            void deallocate(void *ptr, std::size_t n) noexcept
            {
                insert(ptr, n * element_size());
            }

            void deallocate_ordered(void *ptr) noexcept;
            void deallocate_ordered(void *ptr, std::size_t n) noexcept
            {
                insert_ordered(ptr, n * element_size());
            }

            //=== getter ===//
            std::size_t element_size() const noexcept
            {
                return el_size_;
            }

            bool empty() const noexcept
            {
                return !first_;
            }

        private:
            void insert_between(void *pre, void *after,
                                void *mem, std::size_t size) noexcept;

            char *first_;
            std::size_t el_size_;
        };
    } // namespace detail
    /// \endcond
}} // namespace foonathan::memory

#endif // FOONATHAN_MEMORY_DETAILL_FREE_LIST_HPP_INCLUDED
