#pragma once
#include <cassert>
#include <cstddef>
#include <utility>
#include <iostream>

namespace ben 
{
    template <typename T>
    class vector 
    {
        public:
            vector () = default;
            

            explicit vector (size_t size) 
            {
                if (size > 0) allocate(size);
            }

            vector(const vector& other)
            {
                size_t other_size = other.address_of_last - other.start_of_mem;
                if (other_size > 0) 
                {
                    allocate(other_size);
                    std::copy(other.start_of_mem, other.address_of_last, start_of_mem);
                    address_of_last = start_of_mem + other_size;
                }
            }

            vector (vector&& other) noexcept : start_of_mem(other.start_of_mem), end_of_mem(other.end_of_mem), address_of_last(other.address_of_last) {
                other.start_of_mem = nullptr;
                other.end_of_mem = nullptr;
                other.address_of_last = nullptr;
            }

            vector& operator=(vector&& other) noexcept 
            {
                if (this != &other) 
                {
                    clear(); 
                    operator delete(start_of_mem);

                    start_of_mem = other.start_of_mem;
                    address_of_last = other.address_of_last;
                    end_of_mem = other.end_of_mem;

                    other.start_of_mem = nullptr;
                    other.address_of_last = nullptr;
                    other.end_of_mem = nullptr;
                }
                return *this;
            }

            vector& operator=( const vector& other )
            {
                if ( this != &other )
                {
                    if ( capacity() < other.size() )
                    {
                        clear();
                        operator delete( start_of_mem );

                        allocate( other.size() );
                    }
                    else
                    {
                        clear();
                    }

                    for ( size_t i = 0; i < other.size(); ++i )
                    {
                        new ( address_of_last ) T( other[ i ] );
                        address_of_last++;
                    }
                }
                return *this;
            }


            ~vector ( ) 
            {
                for (size_t i = 0; i < size(); ++i)
                {
                    start_of_mem[i].~T(); 
                }
                operator delete(start_of_mem);
                start_of_mem = nullptr;
                end_of_mem = nullptr;
                address_of_last = nullptr;
            }

            T& operator[](size_t index) 
            {
                return start_of_mem[index];
            }

            const T& operator[](size_t index) const 
            {
                return start_of_mem[index];
            }

            size_t size() const
            {
                return address_of_last - start_of_mem;
            }

            size_t capacity() const
            {
                return end_of_mem - start_of_mem;
            }

            void clear() 
            {
                for (size_t i = 0; i < size(); ++i)
                {
                    start_of_mem[i].~T(); 
                }
                address_of_last = start_of_mem;
            }

            void reserve (size_t size)
            {
                size_t current_size = vector::size();
                size_t new_capacity = size;

                if (current_size >= new_capacity) return; 

                T* new_block = static_cast<T*>(operator new(new_capacity * sizeof(T))); 

                for ( size_t i = 0; i < current_size; i++)
                {
                    new (new_block + i) T(std::move_if_noexcept(start_of_mem[i]));
                    start_of_mem[i].~T(); 
                }

                operator delete(start_of_mem);

                start_of_mem = new_block;
                end_of_mem = start_of_mem + new_capacity; 
                address_of_last = start_of_mem + current_size;
            }

            T* push_back(const T& var)
            {
                if (end_of_mem == address_of_last) reserve();
                new (address_of_last) T(var);
                T* result = address_of_last;
                address_of_last++;
                return result;
            }

            T* push_back(T&& var)
            {
                if (end_of_mem == address_of_last) reserve();

                new (address_of_last) T(std::move(var));
                T* result = address_of_last;
                address_of_last++;
                return result;
            }

            template <typename... Args>
            T* emplace_back(Args&&... args)
            {
                if (address_of_last == end_of_mem) reserve();

                new (address_of_last) T(std::forward<Args>(args)...);
                T* result = address_of_last;
                address_of_last++;
                return result;
            }

        private:
            T *start_of_mem = nullptr;
            T *address_of_last = nullptr;
            T *end_of_mem = nullptr;

            void allocate (size_t size)
            {
                start_of_mem = static_cast<T*>(operator new(size * sizeof(T))); 
                end_of_mem = start_of_mem + size;
                address_of_last = start_of_mem;
            }

            void reserve()
            {
                size_t current_cap = capacity();
                size_t new_cap = (current_cap == 0) ? 1 : current_cap * 2;
                size_t current_s = size();

                T* new_block = static_cast<T*>(operator new(new_cap * sizeof(T)));

                for (size_t i = 0; i < current_s; i++)
                {
                    new (new_block + i) T(std::move_if_noexcept(start_of_mem[i]));
                    start_of_mem[i].~T();
                }

                operator delete(start_of_mem);

                start_of_mem = new_block;
                address_of_last = start_of_mem + current_s;
                end_of_mem = start_of_mem + new_cap;
            }
            

    };
}
