#ifndef INCLUDED_SAMWARRING_RING_BUFFER_HPP
#define INCLUDED_SAMWARRING_RING_BUFFER_HPP

#include <type_traits>

namespace samwarring {

template <class T>
class ring_buffer {
    static_assert(std::is_default_constructible_v<T>,
                  "Item type is not default-constructable");
    static_assert(std::is_move_assignable_v<T>,
                  "Item type is not move-assignable");

  public:
    ring_buffer() noexcept : data_{nullptr}, capacity_{0}, next_{0} {}

    ring_buffer(std::size_t capacity)
        : data_{new T[capacity]()}, capacity_{capacity}, next_{0} {}

    ~ring_buffer() {
        delete[] data_;
    }

    void push_back(T item) noexcept(std::is_nothrow_move_assignable<T>::value) {
        data_[next_] = std::move(item);
        next_ = (next_ + 1) % capacity_;
    }

    T& operator[](std::size_t index) noexcept {
        return data_[nth_index(index)];
    }

    const T& operator[](std::size_t index) const noexcept {
        return data_[nth_index(index)];
    }

    T& front() noexcept {
        return data_[front_index()];
    }

    const T& front() const noexcept {
        return data_[front_index()];
    }

    T& back() noexcept {
        return data_[back_index()];
    }

    const T& back() const noexcept {
        return data_[back_index()];
    }

  public:
    class iterator_base {
      public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        bool operator==(const iterator_base& other) const noexcept {
            return (pos_ == other.pos_) && (rollover_ == other.rollover_);
        }

        bool operator!=(const iterator_base& other) const noexcept {
            return (pos_ != other.pos_) || (rollover_ != other.rollover_);
        }

      protected:
        iterator_base(ring_buffer<T>& parent, std::size_t data_index,
                      bool rollover)
            : pos_{parent.data_ + data_index}, data_begin_{parent.data_},
              data_end_{parent.data_ + parent.capacity_}, rollover_{rollover} {}

        void advance() noexcept {
            if (++pos_ == data_end_) {
                pos_ = data_begin_;
                rollover_ = true;
            }
        }

        T* pos_;
        T* data_begin_;
        T* data_end_;
        bool rollover_;
    };

    class iterator : public iterator_base {
      public:
        T& operator*() const noexcept {
            return *pos_;
        }

        T* operator->() const noexcept {
            return pos_;
        }

        iterator& operator++() noexcept {
            advance();
            return *this;
        }

        iterator operator++(int) noexcept {
            auto tmp = *this;
            advance();
            return tmp;
        }

      private:
        friend class ring_buffer<T>;
        using iterator_base::iterator_base;
    };

    class const_iterator : public iterator_base {
      public:
        const T& operator*() const noexcept {
            return *pos_;
        }

        const T* operator->() const noexcept {
            return pos_;
        }

        const_iterator& operator++() noexcept {
            advance();
            return *this;
        }

        const_iterator operator++(int) noexcept {
            auto tmp = *this;
            advance();
            return tmp;
        }

      private:
        friend class ring_buffer<T>;
        using iterator_base::iterator_base;
    };

    iterator begin() noexcept {
        return iterator{*this, front_index(), false};
    }

    iterator end() noexcept {
        return iterator{*this, front_index(), true};
    }

    const_iterator cbegin() noexcept {
        return const_iterator{*this, front_index(), false};
    }

    const_iterator cend() noexcept {
        return const_iterator{*this, front_index(), true};
    }

  private:
    std::size_t front_index() const noexcept {
        return next_;
    }

    std::size_t back_index() const noexcept {
        return (next_ == 0 ? capacity_ - 1 : next_ - 1);
    }

    std::size_t nth_index(std::size_t index) const noexcept {
        return (next_ + index) % capacity_;
    }

    T* data_;
    std::size_t capacity_;
    std::size_t next_;
};

} // namespace samwarring

#endif