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

            ~vector ( ) 
            {
                delete[] start_of_mem;
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

            void reserve (size_t size)
            {
                size_t current_size = address_of_last - start_of_mem;
                size_t new_capacity = size;

                if (current_size >= new_capacity)
                {
                    std::cout << "reserve size should be bigger than current array size." << std::endl;
                    return;
                }

                T *temp_start_of_mem = new T[new_capacity]; 
                T *temp_end_of_mem = temp_start_of_mem + new_capacity; 

                for ( size_t i {}; i < current_size; i++)
                {
                    temp_start_of_mem[i] = std::move(start_of_mem[i]); 
                }
                delete[] start_of_mem;

                start_of_mem = temp_start_of_mem;
                end_of_mem = temp_end_of_mem;
                address_of_last = start_of_mem + current_size;
            }

            void push_back(T var)
            {
                if (start_of_mem == nullptr)
                {
                    allocate(1);
                }
                if (end_of_mem == address_of_last)
                {
                    reserve();
                }

                *address_of_last = var;
                address_of_last++;
            }

        private:
            T *start_of_mem = nullptr;
            T *address_of_last = nullptr;
            T *end_of_mem = nullptr;

            void allocate (size_t size)
            {
                start_of_mem = new T[size]; 
                end_of_mem = start_of_mem + size;
                address_of_last = start_of_mem;
            }

            void reserve ()
            {
                size_t current_size = address_of_last - start_of_mem;
                size_t current_capacity = capacity() ;
                size_t new_capacity = (current_capacity == 0) ? 1 : current_capacity * 2;

                T *temp_start_of_mem = new T[new_capacity]; 
                T *temp_end_of_mem = temp_start_of_mem + new_capacity; 

                for ( size_t i {}; i < current_size; i++)
                {
                    temp_start_of_mem[i] = std::move(start_of_mem[i]); 
                }
                delete[] start_of_mem;

                start_of_mem = temp_start_of_mem;
                end_of_mem = temp_end_of_mem;
                address_of_last = start_of_mem + current_size;
            }
            

    };
}
