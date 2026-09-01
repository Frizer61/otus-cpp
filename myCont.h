#pragma once

#include <memory>

template<typename T, typename  Allocator = std::allocator<T>>
class MyContainer {
    struct Node {
        T value;
        Node* next = nullptr;
    };

    using allocatorTraits = std::allocator_traits<Allocator>;
    using nodeAllocator = typename allocatorTraits::template rebind_alloc<Node>;
    using nodeAllocatorTraits = std::allocator_traits<nodeAllocator>;

    Node* head = nullptr;
    Node* tail = nullptr;
    nodeAllocator allocator;

    public:
        ~MyContainer() {
            Node* curr = head;
            while (curr != nullptr) {
                Node* next = curr->next;

                nodeAllocatorTraits::destroy(allocator, curr);
                nodeAllocatorTraits::deallocate(allocator, curr, 1);

                curr = next;
            }
        }

        void pushBack(const T& value) {
            Node* newNode = nodeAllocatorTraits::allocate(allocator, 1);
            nodeAllocatorTraits::construct(allocator, newNode, Node{value, nullptr});

            if (head == nullptr) {
                head = newNode;
                tail = newNode;
                return;
            }

            tail->next = newNode;
            tail = newNode;
        }

        template<typename lambdaF>
        void forEach(lambdaF l) const {
            Node* curr = head;
            while (curr != nullptr) {
                l(curr->value);
                curr = curr->next;
            }
        }
};