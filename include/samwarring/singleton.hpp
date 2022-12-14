#ifndef INCLUDED_SAMWARRING_SINGLETON_HPP
#define INCLUDED_SAMWARRING_SINGLETON_HPP

#include <atomic>
#include <memory>
#include <mutex>

namespace samwarring {

/**
 * @brief Default singleton identifier tag.
 *
 * The @ref singleton and @ref reference_counted_singleton classes use "tag
 * types" to distinguish between different singletons of the same type. Any type
 * can be used for the tag. This is the default type when no tag is specified.
 */
class default_singleton_tag {};

/**
 * @brief Returns a singleton instance of a type.
 *
 * This defines a common technique for contolling access to a single instance of
 * a type. On first invocation, the singleton type is constructed with its
 * default constructor. Following invocations return a reference to the same
 * instance.
 *
 * Once constructed, the singleton object remains alive for the duration of the
 * program. It cannot be destoyed earlier. This makes code that uses this
 * function fail unit-tests in unexpected ways, because the singleton object
 * cannot be "torn down" in between tests. Tests affecting the singleton's state
 * can inadvertently affect the results of subsequent tests.
 *
 * If a program wants to use multiple singletons of the same type, it can
 * distinguish between them using the `Tag` template parameter. Calls that use
 * the same type for `Tag` will retrieve the same instance. Calls using
 * different types for `Tag` will retrieve different instances.
 *
 * This function is thread-safe.
 *
 * @tparam T The type of singleton object. Must be default-constructable.
 * @tparam Tag Distinguishes between different singletons of the same type.
 * @return T& Reference to the singleton object.
 */
template <class T, class Tag = default_singleton_tag>
T& singleton() {
    static_assert(std::is_default_constructible_v<T>,
                  "Singleton type is not default constructable");

    static T instance;
    return instance;
}

/**
 * @brief Returns a shared reference-counted singleton object.
 *
 * This returns a shared pointer to a singleton object. On first invocation, the
 * object is constructed with its default constructor. Following invocations
 * return a shared pointer to the same object.
 *
 * Once all shared pointers are destroyed, the singleton object is destroyed as
 * well. Any following call will construct a new singleton object with its
 * default constructor.
 *
 * If a program wants to use multiple singletons of the same type, it can
 * distinguish between them using the `Tag` template parameter. Calls that use
 * the same type for `Tag` will retrieve the same instance. Calls using
 * different types for `Tag` will retrieve different instances.
 *
 * This function is thread-safe.
 *
 * @tparam T The type of singleton object. Must be default-constructable.
 * @tparam Tag Distinguishes between different singletons of the same type.
 * @return std::shared_ptr<T> Shared pointer to the singleton object.
 */
template <class T, class Tag = default_singleton_tag>
std::shared_ptr<T> reference_counted_singleton() {
    static_assert(std::is_default_constructible_v<T>,
                  "Singleton type is not default constructable");

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