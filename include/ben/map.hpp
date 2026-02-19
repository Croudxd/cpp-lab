#pragma once
#include <map>

/**
 * std::map
 *
 * Red-black graph data structure.
 * Implement a Node which will hold T data, a color and a left and right child (which can be nullptr)
 * We then create a map using the map, class and we use the node sto store the data T on the heap.
 */

namespace ben
{
    enum class Node_color
    {
        RED,
        BLACK
    };

    template<typename key, typename value>
    struct Node
    {
        Node_color color;
        key id;
        value Data;

        Node* left_child = nullptr;
        Node* right_child = nullptr;
        Node* parent = nullptr;
    };

    template<typename key, typename value>
    class map
    {
        public:
            map () {}

            void insert(key id, value data) 
            {
                if (head == nullptr)
                {
                    head = new Node<key, value>{Node_color::BLACK, id, data};
                    return;
                }
                else 
                {
                    Node<key, value>* current = head;
                    Node<key, value>* parent = nullptr;

                    while (current!=nullptr)
                    {
                        parent = current;
                        if (id == current->id)
                        {
                            head->Data = data;
                            return;
                        }
                        if (id > current->id)
                        {
                            current = current->right_child;
                        }
                        if (id < current->id)
                        {
                            current = current->left_child;
                        }
                    }
                    Node<key, value>* nn = new Node<key, value>{Node_color::RED, id, data};
                    nn->parent = parent;
                    if (id < parent->id) 
                    {
                        parent->left_child = nn;
                    }
                    else 
                    {
                        parent->right_child = nn;
                    }

                    if (parent->color == Node_color::RED)
                    {
                        rebalance(nn);
                    }

                }
            }

            void empty() {}
            void size() {}
            void clear() {}
            void emplace() {}
            void find() {}


        private:
            Node<key, value>* get_uncle(Node<key, value>* current)
            {
                auto grand_parent = current->parent->parent;
                if (current->parent == grand_parent->left_child) return grand_parent->right_child;
                else return grand_parent->left_child;
            }

            void rotate(Node<key, value>* A)
            {
                Node<key, value>* B = A->left_child;
                A->left_child = B->right_child;

                if (B->right_child != nullptr) B->right_child->parent = A; 

                B->parent = A->parent;

                if (A->parent == nullptr) head = B; 

                else if (A == A->parent->right_child) A->parent->right_child = B;

                else A->parent->left_child = B;

                B->right_child = A;
                A->parent = B;
            }

            void rebalance(Node<key, value>* current)
            {
                while (current != head && current->parent->color == Node_color::RED)
                {
                    Node<key, value>* uncle = get_uncle(current);
                    if (uncle != nullptr && uncle->color == Node_color::RED)
                    {
                        current->parent->color = Node_color::BLACK;
                        uncle->color = Node_color::BLACK;
                        current->parent->parent->color = Node_color::RED;
                        current = current->parent->parent;
                    }
                    else 
                    {
                        rotate(current->parent->parent);
                        break;
                    }
                    head->color = Node_color::BLACK;
                }
            }

            Node<key, value>* head = nullptr;
    };
}
