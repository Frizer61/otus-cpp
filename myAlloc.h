#pragma once

#include <cstddef>
#include <memory>
#include <new>

template<std::size_t Capacity>
struct AllocatorState {
    void* memory = nullptr;
    std::size_t used = 0;
    std::size_t elementSize = 0;
    std::size_t alignment = 0;

    ~AllocatorState() {
        if (memory != nullptr) {
            ::operator delete(memory, std::align_val_t(alignment));
        }
    }
};


template<typename T, std::size_t Capacity>
class MyAllocator {
    template<typename, std::size_t>
    friend class MyAllocator;

public:
    using value_type = T;

    template<typename U>
    struct rebind {
        using other = MyAllocator<U, Capacity>;
    };

    MyAllocator()
        : state(std::make_shared<AllocatorState<Capacity>>()) {
    }

    template<typename U>
    MyAllocator(const MyAllocator<U, Capacity>& other) noexcept
        : state(other.state) {
    }

    T* allocate(std::size_t count) {
        if (count > Capacity - state->used) {
            throw std::bad_alloc{};
        }

        if (state->memory == nullptr) {
            state->memory = ::operator new(sizeof(T) * Capacity, std::align_val_t(alignof(T)));
            state->elementSize = sizeof(T);
            state->alignment = alignof(T);
        }

        if (state->elementSize != sizeof(T) || state->alignment != alignof(T)) {
            throw std::bad_alloc{};
        }

        auto* result = static_cast<T*>(state->memory) + state->used;

        state->used += count;

        return result;
    }

    void deallocate(T*, std::size_t) noexcept {
    }

    template<typename U>
    bool operator==(const MyAllocator<U, Capacity>& other) const noexcept {
        return state == other.state;
    }

    template<typename U>
    bool operator!=(const MyAllocator<U, Capacity>& other) const noexcept {
        return !(*this == other);
    }

private:
    std::shared_ptr<AllocatorState<Capacity>> state;
};