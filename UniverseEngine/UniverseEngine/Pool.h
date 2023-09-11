#pragma once

#include <queue>

#include "Logging.h"
#include "Option.h"

namespace UniverseEngine {
    template <typename T>
    class Pool;

    template <typename T>
    struct Handle {
    public:
        Handle() = default;

        static Handle<T> Null() {
            return Handle<T>(0, 0);
        }

        static Handle<T> Invalid() {
            UE_FATAL("Invalid handle reached.");
            return Handle<T>(0, 0);
        }

        void IsNull() const {
            return this->index == 0;
        }

    private:
        friend class Pool<T>;
        Handle(size_t index, size_t generation) : index(index), generation(generation) {
        }

        size_t index;
        size_t generation;

        size_t Index() const {
            UE_ASSERT_MSG(this->index > 0, "Tried to get index of null handle.");
            return index - 1;
        }
    };

    template <typename T>
    class Pool {
    public:
        Pool();
        Pool(const Pool& other) = delete;
        Pool& operator=(const Pool& other) = delete;

        OptionalPtr<T> Value(Handle<T> handle);

        Handle<T> Alloc();
        void Free(Handle<T> handle);

    private:
        std::queue<Handle<T>> freeHandles;

        std::vector<T> data;
        std::vector<size_t> generations;

        size_t capacity;
    };

    template <typename T>
    Pool<T>::Pool() : freeHandles{}, data{}, generations{}, capacity(32) {
        this->data.reserve(this->capacity);
        this->generations.reserve(this->capacity);

        for (size_t i = 0; i < this->capacity; i++) {
            this->freeHandles.push(Handle<T>(i + 1, 0));
            this->data.push_back(T{});
            this->generations.push_back(0);
        }
    }

    template <typename T>
    OptionalPtr<T> Pool<T>::Value(Handle<T> handle) {
        UE_ASSERT_MSG(handle.Index() < this->capacity, "Invalid handle.");

        if (this->generations[handle.Index()] == handle.generation) {
            T* value = &this->data[handle.Index()];
            return OptionalPtr<T>::Some(value);
        } else {
            return OptionalPtr<T>::None();
        }
    }

    template <typename T>
    Handle<T> Pool<T>::Alloc() {
        if (this->freeHandles.empty()) {
            size_t newCapacity = this->capacity * 2;
            this->data.reserve(newCapacity);
            this->generations.reserve(newCapacity);

            for (size_t i = this->capacity; i < newCapacity; i++) {
                this->freeHandles.push(Handle<T>(i + 1, 0));
                this->data.push_back(T{});
                this->generations.push_back(0);
            }

            this->capacity = newCapacity;
        }

        Handle<T> handle = this->freeHandles.front();
        this->freeHandles.pop();
        handle.generation = this->generations[handle.Index()];

        return handle;
    }

    template <typename T>
    void Pool<T>::Free(Handle<T> handle) {
        UE_ASSERT_MSG(handle.Index() < this->capacity, "Invalid handle.");

        if (this->generations[handle.Index()] == handle.generation) {
            this->generations[handle.Index()]++;
            this->freeHandles.push(handle);
        }
    }
}  // namespace UniverseEngine