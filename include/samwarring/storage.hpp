#ifndef INCLUDED_SAMWARRING_STORAGE_HPP
#define INCLUDED_SAMWARRING_STORAGE_HPP

#include <type_traits>

namespace samwarring {

template <class T>
class storage {
  public:
    void construct() noexcept(std::is_nothrow_default_constructible_v<T>) {
        new (&holder_.value) T{};
    }

    template <class... Args>
    void construct(Args&&... args) noexcept(
        noexcept(T(std::forward<Args>(args)...))) {

        new (&holder_.value) T{std::forward<Args>(args)...};
    }

    void destroy() noexcept {
        (&holder_.value)->~T();
    }

    T& operator*() noexcept {
        return holder_.value;
    }

    const T& operator*() const noexcept {
        return holder_.value;
    }

    T* operator->() noexcept {
        return &holder_.value;
    }

    const T* operator->() const noexcept {
        return &holder_.value;
    }

  private:
    union value_holder {
        T value;
    };
    value_holder holder_;
};

} // namespace samwarring

#endif