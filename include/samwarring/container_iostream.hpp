#ifndef INCLUDED_SAMWARRING_CONTAINER_IOSTREAM_HPP
#define INCLUDED_SAMWARRING_CONTAINER_IOSTREAM_HPP

#include <array>
#include <deque>
#include <functional>
#include <istream>
#include <list>
#include <map>
#include <ostream>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace samwarring::container_iostream {

class detail {
    // Stream output
    template <typename T>
    friend std::ostream& operator<<(std::ostream&, const std::vector<T>&);
    template <typename T>
    friend std::ostream& operator<<(std::ostream&, const std::list<T>&);
    template <typename T>
    friend std::ostream& operator<<(std::ostream&, const std::deque<T>&);
    template <typename T>
    friend std::ostream& operator<<(std::ostream&, const std::set<T>&);
    template <typename T>
    friend std::ostream& operator<<(std::ostream&,
                                    const std::unordered_set<T>&);
    template <typename T, std::size_t N>
    friend std::ostream& operator<<(std::ostream&, const std::array<T, N>&);
    template <typename T1, typename T2>
    friend std::ostream& operator<<(std::ostream&, const std::pair<T1, T2>&);
    template <typename... Ts>
    friend std::ostream& operator<<(std::ostream&, const std::tuple<Ts...>&);

    template <typename Key, typename Val>
    friend std::ostream& operator<<(std::ostream&, const std::map<Key, Val>&);
    template <typename Key, typename Val>
    friend std::ostream& operator<<(std::ostream&,
                                    const std::unordered_map<Key, Val>&);

    // Stream input
    template <typename T>
    friend std::istream& operator>>(std::istream&, std::vector<T>&);
    template <typename T>
    friend std::istream& operator>>(std::istream&, std::list<T>&);
    template <typename T>
    friend std::istream& operator>>(std::istream&, std::deque<T>&);
    template <typename T>
    friend std::istream& operator>>(std::istream&, std::set<T>&);
    template <typename T>
    friend std::istream& operator>>(std::istream&, std::unordered_set<T>&);
    template <typename T, std::size_t N>
    friend std::istream& operator>>(std::istream&, std::array<T, N>&);
    template <typename K, typename V>
    friend std::istream& operator>>(std::istream&, std::map<K, V>&);
    template <typename K, typename V>
    friend std::istream& operator>>(std::istream&, std::unordered_map<K, V>&);

    template <class Cont>
    static std::ostream& write_sequence(std::ostream& out,
                                        const Cont& container) {
        out << '{';
        auto it = container.begin();
        auto end = container.end();
        if (it != end) {
            out << *it;
            for (++it; it != end; ++it) {
                out << ", " << *it;
            }
        }
        out << '}';
        return out;
    }

    template <class Cont>
    static std::ostream& write_map(std::ostream& out, const Cont& container) {
        out << '{';
        auto it = container.begin();
        auto end = container.end();
        if (it != end) {
            out << it->first << ": " << it->second;
            for (++it; it != end; ++it) {
                out << ", " << it->first << ": " << it->second;
            }
        }
        out << '}';
        return out;
    }

    template <std::size_t N, class Cont>
    static std::ostream& write_tuple(std::ostream& out, const Cont& container) {
        out << '{';
        write_tuple_items<Cont, N, 0>(out, container);
        out << '}';
        return out;
    }

    template <class Cont, std::size_t Max, std::size_t Index>
    static void write_tuple_items(std::ostream& out, const Cont& container) {
        if constexpr (Index == Max) {
            // Do nothing
        } else if constexpr (Index == 0) {
            out << std::get<0>(container);
            write_tuple_items<Cont, Max, 1>(out, container);
        } else if constexpr (Index < Max) {
            out << ", " << std::get<Index>(container);
            write_tuple_items<Cont, Max, Index + 1>(out, container);
        }
    }

    /**
     * @brief Generalized parser for all containers.
     */
    class parser {
      private:
        bool expect_open_{false};
        bool expect_elem_{false};
        bool expect_sep_{false};
        bool expect_close_{false};
        char sep_char_{false};
        char close_char_{false};
        bool done_{false};
        bool result_{false};
        bool sep_defined_{false};
        std::size_t elem_count_{0};
        bool is_array_{false};
        std::size_t array_size_{0};

      private:
        void reset_expected() {
            expect_open_ = false;
            expect_elem_ = false;
            expect_sep_ = false;
            expect_close_ = false;
        }

      protected:
        /**
         * @brief Pure-virtual implementation for parsing a container element.
         *
         * @details Subclasses provide the implementation for this method
         * because they each parse elements in slightly different ways.
         * Sequences parse their elements into a temporary container; arrays
         * parse their elements directly into the destination; and maps need to
         * parse key-value pairs.
         */
        virtual void parse_element(std::istream& in) = 0;

      public:
        parser() = default;
        parser(bool is_array, std::size_t array_size)
            : is_array_{is_array}, array_size_{array_size} {}

        /**
         * @brief Parses the container from the input stream.
         *
         * @details The method has no return value, but the input stream's
         * stream-state is updated accordingly. (e.g. If the container could not
         * be parsed, the stream's failbit is set). The derived class must
         * provide a method for retrieving the parsed container.
         *
         * @warning This method can not be called more than once.
         */
        void parse(std::istream& in) {
            if (is_array_ && array_size_ == 0) {
                expect_open_ = true;
            } else {
                expect_open_ = true;
                expect_elem_ = true;
            }
            while (!done_) {
                bool found_expected = false;
                if (expect_open_) {
                    found_expected = parse_open(in);
                }
                if (!found_expected && expect_sep_) {
                    found_expected = parse_sep(in);
                }
                if (!found_expected && expect_close_) {
                    found_expected = parse_close(in);
                }
                if (expect_elem_) {
                    if (parse_elem(in)) {
                        found_expected = true;
                    } else if (found_expected) {
                        in.clear(in.rdstate() & ~std::ios_base::failbit);
                    }
                }
                if (!found_expected) {
                    result_ = false;
                    done_ = true;
                }
            }
            if (result_) {
                in.clear(in.rdstate() & ~std::ios_base::failbit);
            } else {
                in.setstate(std::ios_base::failbit);
            }
        }

        bool parse_open(std::istream& in) {
            char ch = 0;
            in >> ch;
            if (!in) {
                return false;
            }
            switch (ch) {
            case '(': close_char_ = ')'; break;
            case '[': close_char_ = ']'; break;
            case '{': close_char_ = '}'; break;
            case '<': close_char_ = '>'; break;
            default: in.unget(); return false;
            }
            reset_expected();
            if (is_array_ && array_size_ == 0) {
                expect_close_ = true;
            } else if (is_array_) {
                expect_elem_ = true;
            } else {
                expect_close_ = true;
                expect_elem_ = true;
            }
            return true;
        }

        bool parse_elem(std::istream& in) {
            parse_element(in);
            if (in.fail()) {
                return false;
            }
            reset_expected();
            ++elem_count_;
            if (is_array_ && elem_count_ == array_size_ && !close_char_) {
                result_ = true;
                done_ = true;
            } else if (is_array_ && elem_count_ < array_size_) {
                expect_sep_ = true;
            } else {
                expect_sep_ = true;
                expect_close_ = true;
            }
            return true;
        }

        bool parse_close(std::istream& in) {
            if (!close_char_) {
                result_ = true;
                done_ = true;
                return true;
            }
            char ch = 0;
            in >> ch;
            if (!in) {
                return false;
            }
            if (ch != close_char_) {
                in.unget();
                return false;
            }
            result_ = true;
            done_ = true;
            return true;
        }

        bool parse_sep(std::istream& in) {
            if (!sep_defined_) {
                char ch = 0;
                in >> ch;
                if (!in) {
                    return false;
                }
                if (ch == ',' || ch == ';') {
                    sep_char_ = ch;
                } else {
                    in.unget();
                }
                sep_defined_ = true;
            } else if (sep_char_) {
                char ch = 0;
                in >> ch;
                if (!in) {
                    return false;
                }
                if (ch != sep_char_) {
                    in.unget();
                    return false;
                }
            }
            reset_expected();
            if (is_array_ && elem_count_ == array_size_) {
                expect_close_ = true;
            } else if (is_array_ && elem_count_ < array_size_) {
                expect_elem_ = true;
            } else {
                expect_close_ = true;
                expect_elem_ = true;
            }
            return true;
        }
    };

    /**
     * @brief Parser for dynamically-sized containers, including sets.
     */
    template <typename Elem, class Container>
    class sequence_parser : public parser {
      private:
        Container dst_;

      public:
        /**
         * @brief Parses container from an input stream.
         *
         * @details The parser::parse method will parse values into an internal
         * container. The user can then move this container into the desired
         * result if the parsing is successful. This method is a wrapper around
         * that use case.
         *
         * @param dst Destination container. If parsing fails, this container is
         * not modified.
         */
        void parse(std::istream& in, Container& dst) {
            parser::parse(in);
            if (!in.fail()) {
                dst = std::move(dst_);
            }
        }

      private:
        void parse_element(std::istream& in) override {
            Elem e;
            in >> e;
            if (!in.fail()) {
                dst_.insert(dst_.end(), std::move(e));
            }
        }
    };

    template <typename Elem>
    class array_parser : public parser {
      private:
        Elem* dst_;

      public:
        array_parser(Elem* dst, std::size_t size)
            : parser(true, size), dst_(dst) {}

      private:
        void parse_element(std::istream& in) override {
            in >> *dst_;
            if (!in.fail()) {
                ++dst_;
            }
        }
    };

    template <typename KeyType, typename ValueType, class Container>
    class map_parser : public parser {
      private:
        Container dst_;

      public:
        /**
         * @brief Parses container from an input stream.
         *
         * @details The parser::parse method will parse values into an internal
         * container. The user can then move this container into the desired
         * result if the parsing is successful. This method is a wrapper around
         * that use case.
         *
         * @param dst Destination container. If parsing fails, this container is
         * not modified.
         */
        void parse(std::istream& in, Container& dst) {
            parser::parse(in);
            if (!in.fail()) {
                dst = std::move(dst_);
            }
        }

      private:
        void parse_element(std::istream& in) override {
            std::pair<KeyType, ValueType> elem;
            in >> elem.first;
            if (!in) {
                in.setstate(std::ios_base::failbit);
                return;
            }
            char ch = 0;
            in >> ch;
            if (!in) {
                return;
            }
            if (ch != ':') {
                in.unget();
                in.setstate(std::ios_base::failbit);
                return;
            }
            in >> elem.second;
            if (in.fail()) {
                return;
            }
            dst_.insert(dst_.end(), std::move(elem));
        }
    };
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& container) {
    return detail::write_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& container) {
    return detail::write_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& container) {
    return detail::write_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& container) {
    return detail::write_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_set<T>& container) {
    return detail::write_sequence(out, container);
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& container) {
    return detail::write_sequence(out, container);
}

template <typename Key, typename Val>
std::ostream& operator<<(std::ostream& out,
                         const std::map<Key, Val>& container) {
    return detail::write_map(out, container);
}

template <typename Key, typename Val>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_map<Key, Val>& container) {
    return detail::write_map(out, container);
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out,
                         const std::pair<T1, T2>& container) {
    return detail::write_tuple<2>(out, container);
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& out,
                         const std::tuple<Ts...>& container) {
    return detail::write_tuple<sizeof...(Ts)>(out, container);
}

template <typename T>
std::istream& operator>>(std::istream& in, std::vector<T>& container) {
    detail::sequence_parser<T, std::vector<T>> psr;
    psr.parse(in, container);
    return in;
}

template <typename T>
std::istream& operator>>(std::istream& in, std::list<T>& container) {
    detail::sequence_parser<T, std::list<T>> psr;
    psr.parse(in, container);
    return in;
}

template <typename T>
std::istream& operator>>(std::istream& in, std::deque<T>& container) {
    detail::sequence_parser<T, std::deque<T>> psr;
    psr.parse(in, container);
    return in;
}

template <typename T>
std::istream& operator>>(std::istream& in, std::set<T>& container) {
    detail::sequence_parser<T, std::set<T>> psr;
    psr.parse(in, container);
    return in;
}

template <typename T>
std::istream& operator>>(std::istream& in, std::unordered_set<T>& container) {
    detail::sequence_parser<T, std::unordered_set<T>> psr;
    psr.parse(in, container);
    return in;
}

template <typename T, std::size_t N>
std::istream& operator>>(std::istream& in, std::array<T, N>& container) {
    detail::array_parser<T> psr(container.data(), N);
    psr.parse(in);
    return in;
}

template <typename K, typename V>
std::istream& operator>>(std::istream& in, std::map<K, V>& container) {
    detail::map_parser<K, V, std::map<K, V>> psr;
    psr.parse(in, container);
    return in;
}

template <typename K, typename V>
std::istream& operator>>(std::istream& in,
                         std::unordered_map<K, V>& container) {
    detail::map_parser<K, V, std::unordered_map<K, V>> psr;
    psr.parse(in, container);
    return in;
}

} // namespace samwarring::container_iostream

#endif