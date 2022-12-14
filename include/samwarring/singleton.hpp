#ifndef INCLUDED_SAMWARRING_SINGLETON_HPP
#define INCLUDED_SAMWARRING_SINGLETON_HPP

#include <atomic>
#include <memory>
#include <mutex>

namespace samwarring {

class default_singleton_tag {};

template <class T, class Tag = default_singleton_tag>
T& singleton() {
    static T instance;
    return instance;
}

template <class T, class Tag = default_singleton_tag>
std::shared_ptr<T> reference_counted_singleton() {
    static std::weak_ptr<T> instance;
    static std::mutex mtx;

    // Will set to false when an object is being constructed, and back to true
    // *after* the instance has been destroyed.
    static std::atomic<bool> deleted{true};

    std::unique_lock<std::mutex> lk{mtx};
    std::shared_ptr<T> shared = instance.lock();
    if (!shared) {
        // The refcount is 0, but has the object actually been destroyed yet?
        // Spin until it is.
        bool expected;
        do {
            expected = true;
        } while (!deleted.compare_exchange_weak(expected, false));

        deleted.store(false);
        shared = std::shared_ptr<T>(new T{}, [&](T* ptr) {
            delete ptr;
            deleted.store(true);
        });
        instance = shared;
    }
    return shared;
}

} // namespace samwarring

#endif