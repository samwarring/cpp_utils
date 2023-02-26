#ifndef INCLUDED_SAMWARRING_CONTAINER_IOSTREAM_HPP
#define INCLUDED_SAMWARRING_CONTAINER_IOSTREAM_HPP

#include <array>
#include <deque>
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

    template <typename Elem, class Cont>
    static std::istream& read_dynamic_sequence(std::istream& in,
                                               Cont& container) {
        // Parse open brace. If no open brace, putback the char.
        char open_char{0};
        char close_char{0};
        in >> open_char;
        if (!in) {
            return in;
        }
        switch (open_char) {
        case '(': close_char = ')'; break;
        case '{': close_char = '}'; break;
        case '[': close_char = ']'; break;
        case '<': close_char = '>'; break;
        default: in.unget();
        }

        // Parse items into this temporary container.
        Cont dst;

        char sep_char{0};        // Maybe a ',' or ';'
        bool sep_defined{false}; // Enabled when separator is decided.
        for (;;) {
            // Try to parse an element.
            Elem element;
            in >> element;

            if (in.bad()) {
                // Internal error occurred. Nothing we can do.
                return in;
            } else if (in.fail()) {
                // Element could not be parsed.
                if (close_char) {
                    // Maybe we can parse a close character.
                    in.clear();
                    char ch{0};
                    in >> ch;
                    if (in.eof()) {
                        // Required a close characater, but hit EOF. Parsing
                        // failed.
                        in.setstate(std::ios_base::failbit);
                        return in;
                    } else if (in.bad()) {
                        // Internal error. Bail out.
                        return in;
                    }
                    if (ch == close_char) {
                        // Closed the container. All done!
                        container = std::move(dst);
                        return in;
                    } else {
                        // Unexpected close character, but found something
                        // else. Parsing failed.
                        in.unget();
                        in.setstate(std::ios_base::failbit);
                        return in;
                    }
                } else {
                    // Element not parsed, but no close char required.
                    if (dst.empty()) {
                        // Failed to parse a single element. Parsing fails.
                        return in;
                    } else {
                        // Parsed some elements. Parsing succeeds.
                        in.clear(in.rdstate() ^ std::ios_base::failbit);
                        container = std::move(dst);
                        return in;
                    }
                }
            }

            // Parsed an element.
            dst.insert(dst.end(), std::move(element));

            if (sep_defined && !sep_char) {
                // No separator required. Parse next element.
                continue;
            }

            char ch{0};
            in >> ch;
            if (in.bad()) {
                // Internal error. Bail out!
                return in;
            }
            if (in.eof()) {
                if (close_char) {
                    // Unclosed container. Parsing failed.
                    in.setstate(std::ios_base::failbit);
                    return in;
                } else {
                    // No close character or separator required. All done!
                    in.clear(in.rdstate() ^ std::ios_base::failbit);
                    container = std::move(dst);
                    return in;
                }
            }
            assert(in.good());
            if (sep_defined && ch != sep_char) {
                // Expected a separator, but found something else.
                if (!close_char) {
                    // Container can end suddenly. Not an error, just
                    // finished parsing.
                    in.unget();
                    container = std::move(dst);
                    return in;
                } else if (ch == close_char) {
                    // Parsed a closed container. All done!
                    container = std::move(dst);
                    return in;
                } else {
                    // Expected a separator or close character, but found
                    // neither.
                    in.unget();
                    in.setstate(std::ios_base::failbit);
                    return in;
                }
            } else if (!sep_defined) {
                // Expecting to detect the separator character.
                if (ch == ',' || ch == ';') {
                    sep_char = ch;
                } else {
                    // Not a valid separator. Must be part of the next
                    // element. Assume no separator character.
                    in.unget();
                }
                sep_defined = true;
            } else {
                assert(sep_defined && ch == sep_char);
            }
        }
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
    return detail::read_dynamic_sequence<T>(in, container);
}

template <typename T>
std::istream& operator>>(std::istream& in, std::list<T>& container) {
    return detail::read_dynamic_sequence<T>(in, container);
}

template <typename T>
std::istream& operator>>(std::istream& in, std::deque<T>& container) {
    return detail::read_dynamic_sequence<T>(in, container);
}

template <typename T>
std::istream& operator>>(std::istream& in, std::set<T>& container) {
    return detail::read_dynamic_sequence<T>(in, container);
}

template <typename T>
std::istream& operator>>(std::istream& in, std::unordered_set<T>& container) {
    return detail::read_dynamic_sequence<T>(in, container);
}

} // namespace samwarring::container_iostream

#endif