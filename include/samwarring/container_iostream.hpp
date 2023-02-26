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
    template <typename T, std::size_t N>
    friend std::istream& operator>>(std::istream&, std::array<T, N>&);

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

    template <typename Elem, std::size_t ElemCount, class Container>
    static std::istream& read_fixed_sequence(std::istream& in,
                                             Container& container) {
        enum class state {
            begin,
            end,
            require_open,
            require_open_or_elem,
            determine_close,
            require_close,
            require_elem,
            require_sep,
            require_sep_or_close,
            determine_sep,
        };
        state s = state::begin;
        char ch = 0;
        bool open_required = false;
        char close_char = 0;
        char sep_char = 0;
        std::size_t elems_parsed = 0;
        while (s != state::end) {
            switch (s) {
            case state::begin:
                // Empty sequence requires braces. Non-empty sequence can start
                // with brace, or with element.
                if constexpr (ElemCount == 0) {
                    s = state::require_open;
                } else {
                    s = state::require_open_or_elem;
                }
                break;
            case state::require_open:
                // Read an open brace. When determining the closing brace,
                // should error if open brace was not parsed.
                in >> ch;
                if (!in) {
                    s = state::end;
                } else {
                    open_required = true;
                    s = state::determine_close;
                }
                break;
            case state::require_open_or_elem:
                // Read an open brace. When determining the closing brace, it's
                // ok if no open brace was parsed.
                in >> ch;
                if (!in) {
                    s = state::end;
                } else {
                    open_required = false;
                    s = state::determine_close;
                }
                break;
            case state::determine_close:
                // Determine the matching closing brace for the parsed
                // character. If char is not an open brace, that may be ok if
                // the open was not required.
                switch (ch) {
                case '(': close_char = ')'; break;
                case '[': close_char = ']'; break;
                case '{': close_char = '}'; break;
                case '<': close_char = '>'; break;
                default:
                    if (open_required) {
                        in.setstate(std::ios_base::failbit);
                        s = state::end;
                        continue;
                    } else {
                        in.unget();
                    }
                }
                if constexpr (ElemCount > 0) {
                    s = state::require_elem;
                } else {
                    s = state::require_close;
                }
                break;
            case state::require_close:
                // A closing brace is required.
                in >> ch;
                if (in && ch != close_char) {
                    in.setstate(std::ios_base::failbit);
                }
                s = state::end;
                break;
            case state::require_elem:
                // At least one more element needs to be parsed.
                in >> container[elems_parsed++];
                if (in.fail()) {
                    s = state::end;
                } else if (in.eof()) {
                    if (elems_parsed < ElemCount || close_char) {
                        in.setstate(std::ios_base::failbit);
                    }
                    s = state::end;
                } else {
                    if (elems_parsed == ElemCount && !close_char) {
                        s = state::end;
                    } else if (elems_parsed == 1) {
                        s = state::determine_sep;
                    } else if (elems_parsed == ElemCount && !sep_char) {
                        s = state::require_close;
                    } else if (elems_parsed == ElemCount && sep_char) {
                        s = state::require_sep_or_close;
                    } else if (sep_char) {
                        s = state::require_sep;
                    } else {
                        s = state::require_elem;
                    }
                }
                break;
            case state::determine_sep:
                // After the first element, the separator must be determined.
                in >> ch;
                if (!in) {
                    s = state::end;
                } else {
                    switch (ch) {
                    case ',':
                    case ';': sep_char = ch; break;
                    default: sep_char = 0; in.unget();
                    }
                    if (elems_parsed == ElemCount) {
                        s = state::require_close;
                    } else {
                        s = state::require_elem;
                    }
                }
                break;
            case state::require_sep:
                // Midway through the sequence, the separator is established and
                // is not empty. It must be present here.
                in >> ch;
                if (!in || ch != sep_char) {
                    in.setstate(std::ios_base::failbit);
                    s = state::end;
                } else {
                    s = state::require_elem;
                }
                break;
            case state::require_sep_or_close:
                // Parsed all elements. Both sep and close chars are not 0. The
                // final element may or have a redundant trailing separator.
                in >> ch;
                if (!in) {
                    s = state::end;
                } else if (ch == sep_char) {
                    s = state::require_close;
                } else if (ch == close_char) {
                    s = state::end;
                } else {
                    in.setstate(std::ios_base::failbit);
                    s = state::end;
                }
            }
        }
        return in;
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

template <typename T, std::size_t N>
std::istream& operator>>(std::istream& in, std::array<T, N>& container) {
    return detail::read_fixed_sequence<T, N>(in, container);
}

} // namespace samwarring::container_iostream

#endif