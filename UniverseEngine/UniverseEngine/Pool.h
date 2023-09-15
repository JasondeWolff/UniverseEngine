#pragma once

#include <queue>
#include <functional>

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

        bool IsNull() const {
            return this->index == 0;
        }

        operator bool() {
            return !IsNull();
        }

        bool operator<(const Handle<T>& other) const {
            return this->index < other.index;
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
        explicit Pool(const Pool& other) = delete;
        Pool& operator=(const Pool& other) = delete;
        explicit Pool(Pool&& other) noexcept = default;
        Pool& operator=(Pool&& other) noexcept = default;

        OptionalPtr<T> Value(Handle<T> handle);
        const std::vector<std::reference_wrapper<T>> AllValues();

        Handle<T> Alloc();
        void Free(Handle<T> handle);

    private:
        std::queue<Handle<T>> freeHandles;

        std::vector<T> data;
        std::vector<size_t> generations;
        std::vector<bool> alive;

        size_t capacity;
    };

    template <typename T>
    Pool<T>::Pool() : freeHandles{}, data{}, generations{}, capacity(32) {
        this->data.reserve(this->capacity);
        this->generations.reserve(this->capacity);
        this->alive.reserve(this->capacity);

        for (size_t i = 0; i < this->capacity; i++) {
            this->freeHandles.push(Handle<T>(i + 1, 0));
            this->data.emplace_back(T{});
            this->generations.push_back(0);
            this->alive.push_back(false);
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
    const std::vector<std::reference_wrapper<T>> Pool<T>::AllValues() {
        std::vector<std::reference_wrapper<T>> values;
        values.reserve(this->capacity - this->freeHandles.size());

        for (size_t i = 0; i < capacity; i++) {
            if (alive[i]) {
                values.push_back(std::ref(data[i]));
            }
        }

        return values;
    }

    template <typename T>
    Handle<T> Pool<T>::Alloc() {
        if (this->freeHandles.empty()) {
            size_t newCapacity = this->capacity * 2;
            this->data.reserve(newCapacity);
            this->generations.reserve(newCapacity);
            this->alive.reserve(newCapacity);

            for (size_t i = this->capacity; i < newCapacity; i++) {
                this->freeHandles.push(Handle<T>(i + 1, 0));
                this->data.emplace_back(T{});
                this->generations.push_back(0);
                this->alive.push_back(false);
            }

            this->capacity = newCapacity;
        }

        Handle<T> handle = this->freeHandles.front();
        this->freeHandles.pop();
        handle.generation = this->generations[handle.Index()];
        this->alive[handle.Index()] = true;

        return handle;
    }

    template <typename T>
    void Pool<T>::Free(Handle<T> handle) {
        UE_ASSERT_MSG(handle.Index() < this->capacity, "Invalid handle.");

        if (this->generations[handle.Index()] == handle.generation) {
            this->generations[handle.Index()]++;
            this->alive[handle.Index()] = false;
            this->freeHandles.push(handle);
        }
    }
}  // namespace UniverseEngine