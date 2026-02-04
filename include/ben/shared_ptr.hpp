#include <cstddef>

//Control block is just in memory
//We create shared_ptr object init the block in memory.
//set shared_ptr to 1
//Copy contructor means that we shoud be able to copy, but we add to shared_ptr.
// Destructor is difficult we need to make sure that if 
namespace ben 
{
    template <typename T>
    class weak_ptr;

    struct Control_block
    {
        size_t shared_ptr = 0;
        size_t weak_ptr = 0;
    };

    template <typename T>
    class shared_ptr {
        public:
            shared_ptr() = default;

            shared_ptr (T* ptr) : ptr(ptr)
            {
                cb = new Control_block();
                cb->shared_ptr = 1;
            }

            shared_ptr (const shared_ptr& other)
            {
                ptr = other.ptr;
                cb = other.cb;
                if (cb) cb->shared_ptr++;
            }

            shared_ptr& operator= (shared_ptr& other)
            {
                if (this == &other)
                {
                    return *this;
                }

                if (cb)
                {
                    cb->shared_ptr--;
                    if ( cb->shared_ptr == 0 )
                    {
                        if ( ptr != nullptr )
                        {
                            delete ptr;
                        }

                        if (cb->weak_ptr == 0)
                        {
                            delete cb;
                        }
                    }
                }
                ptr = other.ptr;
                cb = other.cb;
                if (cb) cb->shared_ptr++;
                return *this;
            }

            T& operator* () const
            {
                return *ptr;
            }

            T* operator-> () const
            {
                return ptr;
            }

            ~shared_ptr()
            {
                if ( cb == nullptr) return;

                cb->shared_ptr --;

                if (cb->shared_ptr == 0)
                {
                    if (ptr != nullptr)
                        delete ptr;

                    if (cb->weak_ptr == 0)
                    {
                        delete cb;
                    }
                }
            }

            T* get()
            {
                return ptr;
            }

            size_t use_count()
            {
                return (cb) ? cb->shared_ptr : 0;
            }


        private:
            T* ptr = nullptr;
            Control_block * cb = nullptr;

            shared_ptr (T* p, Control_block* cb) : ptr(p), cb(cb)
            {
                if (cb)
                {
                    cb->shared_ptr++;
                }
            }

            friend class weak_ptr<T>;

    };

    template <typename T>
    class weak_ptr {
        public:
            weak_ptr () = default;
            weak_ptr (const shared_ptr<T>& other) : ptr(other.ptr), cb(other.cb)
            {
                if (cb)
                {
                    cb->weak_ptr++;
                }
            }


            weak_ptr& operator= (const shared_ptr<T>& other)
            {
                if (cb)
                {
                    cb->weak_ptr--;
                    if (cb->weak_ptr == 0 && cb->shared_ptr == 0)
                    {
                        delete cb;
                    }
                }

                ptr = other.ptr;
                cb = other.cb;


                if (cb)
                {
                    cb->weak_ptr++;
                }

                return *this;
            }
            ~weak_ptr ()
            {
                if (cb)
                {
                    cb->weak_ptr--;
                    if (cb->shared_ptr == 0 && cb->weak_ptr == 0)
                    {
                        delete cb;
                    }
                }
            }

            bool expired ()
            {
                return !cb || cb->shared_ptr == 0; 
            }

            shared_ptr<T> lock ()
            {
                if ( expired() )
                {
                    return shared_ptr<T> ();
                }

                return shared_ptr<T> (this->ptr, this->cb);
            }
        private:
            T* ptr = nullptr;
            Control_block* cb = nullptr;

            friend class shared_ptr<T>;
    };

};
