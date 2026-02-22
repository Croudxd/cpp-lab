#pragma once
#include <iostream>

namespace ll
{
    namespace internal
    {
        template <typename T>
        class Node
        {
        public:
            Node(T value) : data(value), next(nullptr) {}
            T data;
            Node* next;
            void toString()
            {
                std::cout << data << std::endl;
            }

        };
    } 

    template<class T>
    class LinkedList
    {
    public:
        LinkedList() : head(nullptr){}
        LinkedList(const LinkedList&) = default;
        LinkedList<T>& operator=(const LinkedList& other) = default;
        ~LinkedList();
        void push_back(T value);
        void print();
        int size();
        internal::Node<T>* get_middle(internal::Node<T> * h);
        void merge_sort();
        internal::Node<T>* getHead();


    private:
        internal::Node<T>* head;
        internal::Node<T>* recursive_sort(internal::Node<T> * h);
        internal::Node<T>* merge(internal::Node<T> * a, internal::Node<T> * b);

    };


    template<typename T>
    LinkedList<T>::~LinkedList() {
        internal::Node<T>* current = head;
        while (current != nullptr)
        {
            internal::Node<T>* next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
    }

    template<typename T>
    void LinkedList<T>::push_back(T value)
    {
        internal::Node<T>* newNode = new internal::Node<T>(value);
        if (head == nullptr)
        {
            newNode->next = nullptr;
            head = newNode;
        }
        else
        {
            internal::Node<T>* current = head;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    template<typename T>
    void LinkedList<T>::print()
    {
        if (head == nullptr)
        {
            std::cout << "null linked list..." << std::endl;
        }
        else
        {
            internal::Node<T>* current = head;
            while (current != nullptr)
            {
                current->toString();
                current = current->next;
            }
        }
    }

    template<typename T>
    int LinkedList<T>::size()
    {
        int size = 0;
        if (head == nullptr)
        {
            return 0;
        }
        else
        {
            internal::Node<T>* current = head;
            while (current != nullptr)
            {
                size++;
                current = current->next;
            }
        }
        return size;
    }

    template<typename T>
    internal::Node<T>* LinkedList<T>::merge(internal::Node<T>* a, internal::Node<T>* b)
    {
        if (a == nullptr) return a;
        if (b == nullptr) return b;

        internal::Node<T>* result = nullptr;
        
        if (a->value <= b->value)
        { 
            result = a;
            result->next = merge(a->next, b);
        }

        else
        { 
            result = b;
            result->next = merge(b->next, a);
        }
        return result;
    }

    template<typename T>
    internal::Node<T>* LinkedList<T>::recursive_sort(internal::Node<T> * h)
    {
        if ( h == nullptr || h->next == nullptr) return h;

        internal::Node<T>* mid = get_middle(h);
        internal::Node<T>* middle_next = mid->next;

        mid->next = nullptr;

        internal::Node<T>* left = recursive_sort(h);
        internal::Node<T>* right = recursive_sort(middle_next);

        return merge(left, right);
    }

    template<typename T>
    internal::Node<T>* LinkedList<T>::get_middle(internal::Node<T> * h)
    {
        if ( h == nullptr ) return h;
        internal::Node<T>* slow = h;
        internal::Node<T>* fast = h->next;

        while (fast != nullptr && slow != nullptr ) 
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    template<typename T>
    void LinkedList<T>::merge_sort()
    {
        if (head == nullptr || head->next == nullptr) return;

        head = recursive_sort(head);
    }

    template<typename T>
    internal::Node<T>* LinkedList<T>::getHead()
    {
        return this->head;
    }
}
