#ifndef INCLUDED_SAMWARRING_RING_BUFFER_HPP
#define INCLUDED_SAMWARRING_RING_BUFFER_HPP

#include <type_traits>

namespace samwarring {

/**
 * Fixed-size buffer where each insertion overwrites the oldest element.
 *
 * A ring buffer is a data structure containing items in a contiguous span
 * of memory. As items are pushed into the buffer they replace the oldest
 * item. Other items remain untouched. This makes ring_buffer ideal for
 * implementing a fixed-size "sliding window" of values.
 *
 * Ring buffers are constructed with a maximum size, N, at which point all N
 * items are default-constructed. ring_buffer cannot be instantiated with
 * element types that are not default-constructable.
 *
 * Items are written to the buffer with push_back, and the parameter is move-
 * assigned into an existing element. ring_buffer cannot be instantated with
 * element types that are not move-assignable.
 *
 * The "back" always refers to the item most recently written to the buffer,
 * and conversely, the "front" always refers to the oldest item in the buffer.
 * Items are 0-indexed from the front.
 *
 * Example
 * -------
 *
 *      ring_buffer<int> buf{3};  // [0, 0, 0]
 *      buf.push_back(7);         // [0, 0, 7]
 *      buf.push_back(3);         // [0, 7, 3]
 *      buf.push_back(9);         // [7, 3, 9]
 *      buf.push_back(2);         // [3, 9, 2]
 */
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
    template <class U>
    class iterator_base {
      public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::remove_const_t<U>;
        using pointer = U*;
        using reference = U&;

        U& operator*() const noexcept {
            return *pos_;
        }

        U* operator->() const noexcept {
            return pos_;
        }

        bool operator==(const iterator_base& other) const noexcept {
            return (pos_ == other.pos_) && (rollover_ == other.rollover_);
        }

        bool operator!=(const iterator_base& other) const noexcept {
            return (pos_ != other.pos_) || (rollover_ != other.rollover_);
        }

        iterator_base<U>& operator++() noexcept {
            advance();
            return *this;
        }

        iterator_base<U> operator++(int) noexcept {
            auto tmp = *this;
            advance();
            return tmp;
        }

      private:
        friend class ring_buffer<T>;

        iterator_base(U* data_begin, std::size_t capacity, U* pos,
                      bool rollover)
            : pos_{pos}, data_begin_{data_begin},
              data_end_{data_begin + capacity}, rollover_{rollover} {}

        void advance() noexcept {
            if (++pos_ == data_end_) {
                pos_ = data_begin_;
                rollover_ = true;
            }
        }

        U* pos_;
        U* data_begin_;
        U* data_end_;
        bool rollover_;
    };

    using iterator = iterator_base<T>;
    using const_iterator = iterator_base<const T>;
    using unordered_iterator = T*;
    using const_unordered_iterator = const T*;
    using partial_iterator = T*;

    template <class U>
    class partition_base {
      public:
        partial_iterator begin() noexcept {
            return begin_;
        }

        partial_iterator end() noexcept {
            return end_;
        }

      private:
        friend class ring_buffer<T>;
        partition_base(U* begin, U* end) : begin_{begin}, end_{end} {}

        U* begin_;
        U* end_;
    };

    using partition = partition_base<T>;
    using const_partition = partition_base<const T>;

    iterator begin() noexcept {
        return iterator{data_, capacity_, data_ + front_index(), false};
    }

    const_iterator begin() const noexcept {
        return const_iterator{data_, capacity_, data_ + front_index(), false};
    }

    iterator end() noexcept {
        return iterator{data_, capacity_, data_ + front_index(), true};
    }

    const_iterator end() const noexcept {
        return const_iterator{data_, capacity_, data_ + front_index(), true};
    }

    unordered_iterator unordered_begin() noexcept {
        return data_;
    }

    const_unordered_iterator unordered_begin() const noexcept {
        return data_;
    }

    unordered_iterator unordered_end() noexcept {
        return data_ + capacity_;
    }

    const_unordered_iterator unordered_end() const noexcept {
        return data_ + capacity_;
    }

    partition first_part() noexcept {
        return partition{data_ + next_, data_ + capacity_};
    }

    const_partition first_part() const noexcept {
        return const_partition{data_ + next_, data_ + capacity_};
    }

    partition second_part() noexcept {
        return partition{data_, data_ + next_};
    }

    const_partition second_part() const noexcept {
        return const_partition{data_, data_ + next_};
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