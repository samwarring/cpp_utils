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
    /**
     * @name Constructors
     * @{
     */

    /**
     * Default constructor.
     *
     * Constructs a new ring buffer that cannot hold any items. Such instances
     * are useless unless they are assigned the contents of another ring_buffer
     * instance.
     */
    ring_buffer() noexcept : data_{nullptr}, size_{0}, next_{0} {}

    /**
     * Main constructor.
     *
     * Constructs a new ring buffer with `size` items. All items are
     * default-constructed.
     *
     * @param size Number of items in the buffer
     */
    ring_buffer(std::size_t size)
        : data_{new T[size]()}, size_{size}, next_{0} {}

    /**
     * Copy constructor.
     *
     * Constructs a ring buffer from an existing one. The new buffer first
     * default-constructs all elements, then copy-assigns each element from
     * the original buffer into the new one.
     *
     * @param other The original buffer.
     */
    ring_buffer(const ring_buffer<T>& other)
        : data_{new T[other.size_]()}, size_{other.size_}, next_{other.next_} {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    /**
     * Move constructor.
     *
     * Constructs a ring buffer by stealing the contents from an existing one.
     * The moved-from buffer becomes an empty buffer with no data and no
     * size.
     *
     * @param other
     */
    ring_buffer(ring_buffer<T>&& other)
        : data_{other.data_}, size_{other.size_}, next_{other.next_} {
        other.data_ = nullptr;
        other.size_ = 0;
        other.next_ = 0;
    }
    /**
     * @} End of Constructors
     */

    /**
     * Destructor.
     *
     * Destroys all items, and releases memory.
     */
    ~ring_buffer() {
        delete[] data_;
    }

    void swap(ring_buffer<T>& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(next_, other.next_);
    }

    ring_buffer<T>& operator=(const ring_buffer<T>& other) {
        ring_buffer<T> tmp{other};
        swap(tmp);
        return *this;
    }

    ring_buffer<T>& operator=(ring_buffer<T>&& other) noexcept {
        ring_buffer<T> tmp{std::move(other)};
        swap(tmp);
        return *this;
    }

    std::size_t size() const noexcept {
        return size_;
    }

    void push_back(T item) noexcept(std::is_nothrow_move_assignable<T>::value) {
        data_[next_] = std::move(item);
        next_ = (next_ + 1) % size_;
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
        using iterator_category = std::random_access_iterator_tag;
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
            increment();
            return *this;
        }

        iterator_base<U> operator++(int) noexcept {
            auto tmp = *this;
            increment();
            return tmp;
        }

        iterator_base<U>& operator--() noexcept {
            decrement();
            return *this;
        }

        iterator_base<U> operator--(int) noexcept {
            auto tmp = *this;
            decrement();
            return tmp;
        }

        iterator_base<U> operator+(difference_type offset) const noexcept {
            auto tmp = *this;
            tmp.adjust(offset);
            return tmp;
        }

        iterator_base<U> operator-(difference_type offset) const noexcept {
            auto tmp = *this;
            tmp.adjust(-offset);
            return tmp;
        }

        iterator_base<U>& operator+=(difference_type offset) noexcept {
            adjust(offset);
            return *this;
        }

        iterator_base<U>& operator-=(difference_type offset) noexcept {
            adjust(-offset);
            return *this;
        }

        difference_type operator-(const iterator_base<U> other) const noexcept {
            assert(data_begin_ == other.data_begin_);
            if (rollover_ == other.rollover_) {
                return pos_ - other.pos_;
            } else if (rollover_) {
                // this is ahead of other
                return (pos_ - data_begin_) + (data_end_ - other.pos_);
            } else {
                // other is ahead of this
                return (other.pos_ - data_begin_) - (data_end_ - pos_);
            }
        }

        bool operator<(const iterator_base<U> other) const noexcept {
            if (rollover_ == other.rollover_) {
                return pos_ < other.pos_;
            } else {
                return other.rollover_;
            }
        }

        bool operator<=(const iterator_base<U> other) const noexcept {
            if (rollover_ == other.rollover_) {
                return pos_ <= other.pos_;
            } else {
                return other.rollover_;
            }
        }

        bool operator>(const iterator_base<U> other) const noexcept {
            if (rollover_ == other.rollover_) {
                return pos_ > other.pos_;
            } else {
                return rollover_;
            }
        }

        bool operator>=(const iterator_base<U> other) const noexcept {
            if (rollover_ == other.rollover_) {
                return pos_ >= other.pos_;
            } else {
                return rollover_;
            }
        }

        U& operator[](difference_type offset) const noexcept {
            return *(*this + offset);
        }

      private:
        friend class ring_buffer<T>;

        iterator_base(U* data_begin, std::size_t size, U* pos, bool rollover)
            : pos_{pos}, data_begin_{data_begin}, data_end_{data_begin + size},
              rollover_{rollover} {}

        difference_type size() const noexcept {
            return data_end_ - data_begin_;
        }

        void increment() noexcept {
            assert(pos_ != data_end_);
            if (++pos_ == data_end_) {
                pos_ = data_begin_;
                rollover_ = true;
            }
        }

        void decrement() noexcept {
            assert((pos_ != data_begin_) || rollover_);
            if (pos_ == data_begin_) {
                pos_ = data_end_;
                rollover_ = false;
            }
            --pos_;
        }

        void adjust(difference_type offset) noexcept {
            if (offset > 0) {
                pos_ += offset;
                if (pos_ >= data_end_) {
                    rollover_ = true;
                    auto new_index = (pos_ - data_end_) % size();
                    pos_ = data_begin_ + new_index;
                }
            } else if (offset < 0) {
                pos_ += offset; // add a negative: subtraction
                if (pos_ < data_begin_) {
                    rollover_ = false;
                    auto new_index_from_end = (data_begin_ - pos_) % size();
                    pos_ = data_end_ - new_index_from_end;
                }
            }
            // else (offset == 0): no change
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

    iterator begin() noexcept {
        return iterator{data_, size_, data_ + front_index(), false};
    }

    const_iterator begin() const noexcept {
        return const_iterator{data_, size_, data_ + front_index(), false};
    }

    iterator end() noexcept {
        return iterator{data_, size_, data_ + front_index(), true};
    }

    const_iterator end() const noexcept {
        return const_iterator{data_, size_, data_ + front_index(), true};
    }

    unordered_iterator unordered_begin() noexcept {
        return data_;
    }

    const_unordered_iterator unordered_begin() const noexcept {
        return data_;
    }

    unordered_iterator unordered_end() noexcept {
        return data_ + size_;
    }

    const_unordered_iterator unordered_end() const noexcept {
        return data_ + size_;
    }

    template <class F>
    void for_each(F callback) noexcept(std::is_nothrow_invocable_v<F, T&>) {
        for (T* it = data_ + next_; it != data_ + size_; ++it) {
            callback(*it);
        }
        for (T* it = data_; it != data_ + next_; ++it) {
            callback(*it);
        }
    }

    template <class F>
    void for_each(F callback) const
        noexcept(std::is_nothrow_invocable_v<F, const T&>) {

        for (const T* it = data_ + next_; it != data_ + size_; ++it) {
            callback(*it);
        }
        for (const T* it = data_; it != data_ + next_; ++it) {
            callback(*it);
        }
    }

  private:
    std::size_t front_index() const noexcept {
        return next_;
    }

    std::size_t back_index() const noexcept {
        return (next_ == 0 ? size_ - 1 : next_ - 1);
    }

    std::size_t nth_index(std::size_t index) const noexcept {
        return (next_ + index) % size_;
    }

    T* data_;
    std::size_t size_;
    std::size_t next_;
};

} // namespace samwarring

#endif