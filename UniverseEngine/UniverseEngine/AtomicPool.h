#pragma once

#include <mutex>
#include <queue>

#include "Option.h"

namespace UniverseEngine {
    template <typename T>
    class AtomicPool;

    template <typename T>
    struct AtomicHandle {
    public:
        AtomicHandle() = default;

        AtomicHandle(const AtomicHandle& other)
            : index(other.index),
              strongCount(other.strongCount),
              mutex(other.mutex),
              pool(other.pool) {
            std::lock_guard<std::mutex> lock(*this->mutex);
            (*this->strongCount)++;
        }

        AtomicHandle& operator=(const AtomicHandle& other) {
            this->Clean();

            this->index = other.index;
            this->strongCount = other.strongCount;
            this->mutex = other.mutex;
            this->pool = other.pool;

            std::lock_guard<std::mutex> lock(*this->mutex);
            (*this->strongCount)++;
            return *this;
        }

        AtomicHandle(AtomicHandle&& other)
            : index(other.index),
              strongCount(other.strongCount),
              mutex(other.mutex),
              pool(other.pool) {
            *other.strongCount = 0;
        }

        AtomicHandle& operator=(AtomicHandle&& other) {
            this->Clean();

            this->index = other.index;
            this->strongCount = other.strongCount;
            this->mutex = other.mutex;
            this->pool = other.pool;

            *other.strongCount = 0;
            return *this;
        }

        ~AtomicHandle() {
            this->Clean();
        }

        static AtomicHandle<T> Null() {
            return AtomicHandle<T>(0, nullptr);
        }

        static AtomicHandle<T> Invalid() {
            UE_FATAL("Invalid handle reached.");
            return AtomicHandle<T>(0, nullptr);
        }

        void IsNull() const {
            return this->index == 0;
        }

    private:
        friend class AtomicPool<T>;
        AtomicHandle(size_t index, AtomicPool<T>* pool)
            : index(index), strongCount(new size_t(1)), mutex(new std::mutex()), pool(pool) {
        }

        size_t index;
        size_t* strongCount;
        std::mutex* mutex;
        AtomicPool<T>* pool;

        size_t Index() const {
            UE_ASSERT_MSG(this->index > 0, "Tried to get index of null handle.");
            return index - 1;
        }

        void Clean() {
            if (*this->strongCount == 0)
                return;

            {
                std::lock_guard<std::mutex> lock(*this->mutex);
                (*this->strongCount)--;
            }

            if (*this->strongCount == 0) {
                this->strongCount = nullptr;
                this->mutex = nullptr;
                this->pool->Free(*this);
            }
        }
    };

    template <typename T>
    class AtomicPool {
    public:
        AtomicPool();
        AtomicPool(const AtomicPool& other) = delete;
        AtomicPool& operator=(const AtomicPool& other) = delete;

        OptionalPtr<T> Value(AtomicHandle<T> handle);
        AtomicHandle<T> Alloc();

    private:
        std::queue<AtomicHandle<T>> freeHandles;
        std::vector<T> data;
        size_t capacity;

        friend struct AtomicHandle<T>;
        void Free(AtomicHandle<T> handle);
    };

    template <typename T>
    AtomicPool<T>::AtomicPool() : freeHandles{}, data{}, capacity(32) {
        this->data.reserve(this->capacity);

        for (size_t i = 0; i < this->capacity; i++) {
            this->freeHandles.emplace(AtomicHandle<T>(i + 1, this));
            this->data.emplace_back(T{});
        }
    }

    template <typename T>
    OptionalPtr<T> AtomicPool<T>::Value(AtomicHandle<T> handle) {
        UE_ASSERT_MSG(handle.Index() < this->capacity, "Invalid handle.");

        T* value = &this->data[handle.Index()];
        return OptionalPtr<T>::Some(value);
    }

    template <typename T>
    AtomicHandle<T> AtomicPool<T>::Alloc() {
        if (this->freeHandles.empty()) {
            size_t newCapacity = this->capacity * 2;
            this->data.reserve(newCapacity);

            for (size_t i = this->capacity; i < newCapacity; i++) {
                this->freeHandles.push(AtomicHandle<T>(i + 1, this));
                this->data.push_back(T{});
            }

            this->capacity = newCapacity;
        }

        AtomicHandle<T> handle = this->freeHandles.front();
        this->freeHandles.pop();
        *handle.strongCount = 1;

        return handle;
    }

    template <typename T>
    void AtomicPool<T>::Free(AtomicHandle<T> handle) {
        UE_ASSERT_MSG(handle.Index() < this->capacity, "Invalid handle.");

        this->freeHandles.push(handle);
    }
}  // namespace UniverseEngine