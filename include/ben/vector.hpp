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
                size_t current_size = address_of_last - start_of_mem;
                size_t new_capacity = size;

                if (current_size >= new_capacity)
                {
                    std::cout << "reserve size should be bigger than current array size." << std::endl;
                    return;
                }

                T *temp_start_of_mem = static_cast<T*>(operator new(new_capacity * sizeof(T))); 
                T *temp_end_of_mem = temp_start_of_mem + new_capacity; 

                for ( size_t i {}; i < current_size; i++)
                {
                    temp_start_of_mem[i] = std::move(start_of_mem[i]); 
                }

                for ( size_t i = 0; i < vector::size(); ++i)
                {
                    start_of_mem[i].~T(); 
                }
                operator delete(start_of_mem);

                start_of_mem = temp_start_of_mem;
                end_of_mem = temp_end_of_mem;
                address_of_last = start_of_mem + current_size;
            }

            void push_back(const T& var)
            {
                if (end_of_mem == address_of_last) reserve();

                new (address_of_last) T(var);
                address_of_last++;
            }

            void push_back(T&& var)
            {
                if (end_of_mem == address_of_last) reserve();
                if (address_of_last == nullptr) reserve();

                new (address_of_last) T(std::move(var));
                address_of_last++;
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

            void reserve ()
            {
                if (start_of_mem == nullptr) allocate(1);
                size_t current_capacity = capacity() ;
                size_t new_capacity = (current_capacity == 0) ? 1 : current_capacity * 2;
                size_t current_size = size() ;

                T* new_block = static_cast<T*>(operator new(new_capacity * sizeof(T))); 

                for ( size_t i {}; i < current_size; i++)
                {
                    new (new_block + i) T(std::move_if_noexcept(start_of_mem[i]));
                    start_of_mem[i].~T(); 
                }

                operator delete(start_of_mem);

                start_of_mem = new_block;
                address_of_last = start_of_mem + current_size;
                end_of_mem = start_of_mem + new_capacity;
            }
            

    };
}
