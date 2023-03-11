#ifndef INCLUDED_SAMWARRING_CONTAINER_OSTREAM_HPP
#define INCLUDED_SAMWARRING_CONTAINER_OSTREAM_HPP

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

namespace samwarring::container_ostream {

class detail {
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

    /**
     * @brief Overload of string formatting to avoid ambiguous output.
     *
     * @details Consider a vector<string> where one of the strings contains a
     * comma, whitespace, or a closing brace. In this case, attempts to parse
     * the formatted container back into itself would be ambiguous. This
     * method overrides the default string output behavior so that:
     * (1) all strings are enclosed in double-quotes;
     * (2) special characters are replaced with escape sequences;
     *
     * @note This overload takes precedence over the generic format<T> when T is
     * convertible to a string_view.
     */
    static std::ostream& format(std::ostream& out, std::string_view s) {
        out << '\"';
        std::size_t start = 0;
        std::size_t end = 0;
        do {
            end = s.find_first_of("\"\\\n\t\r", start);
            if (end == s.npos) {
                out << s.substr(start);
            } else {
                out << s.substr(start, end - start);
                switch (s[end]) {
                case '\n': out << "\\n"; break;
                case '\t': out << "\\t"; break;
                case '\r': out << "\\r"; break;
                default: out << '\\' << s[end]; break;
                }
                ++end;
            }
            start = end;
        } while (end != s.npos);
        out << '\"';
        return out;
    }

    /**
     * @brief Formats the parameter with the standard operator<<
     *
     * @details format(out, t) is equivalent to out << t. However, if t is a
     * string-like type, then this overload is rejected in favor of
     * format(ostream&, string_view).
     */
    template <typename T>
    static typename std::enable_if<
        !std::is_convertible<T, std::string_view>::value, std::ostream&>::type
    format(std::ostream& out, T&& t) {
        return out << t;
    }

    template <class Cont>
    static std::ostream& format_sequence(std::ostream& out,
                                         const Cont& container) {
        out << '{';
        auto it = container.begin();
        auto end = container.end();
        if (it != end) {
            format(out, *it);
            for (++it; it != end; ++it) {
                out << ", ";
                format(out, *it);
            }
        }
        out << '}';
        return out;
    }

    template <class Cont>
    static std::ostream& format_map(std::ostream& out, const Cont& container) {
        out << '{';
        auto it = container.begin();
        auto end = container.end();
        if (it != end) {
            format(out, it->first) << ": " << it->second;
            for (++it; it != end; ++it) {
                out << ", ";
                format(out, it->first) << ": " << it->second;
            }
        }
        out << '}';
        return out;
    }

    template <std::size_t N, class Cont>
    static std::ostream& format_tuple(std::ostream& out,
                                      const Cont& container) {
        out << '{';
        format_tuple_items<Cont, N, 0>(out, container);
        out << '}';
        return out;
    }

    template <class Cont, std::size_t Max, std::size_t Index>
    static void format_tuple_items(std::ostream& out, const Cont& container) {
        if constexpr (Index == Max) {
            // Do nothing
        } else if constexpr (Index == 0) {
            format(out, std::get<0>(container));
            format_tuple_items<Cont, Max, 1>(out, container);
        } else if constexpr (Index < Max) {
            out << ", ";
            format(out, std::get<Index>(container));
            format_tuple_items<Cont, Max, Index + 1>(out, container);
        }
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& container) {
    return detail::format_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& container) {
    return detail::format_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& container) {
    return detail::format_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& container) {
    return detail::format_sequence(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_set<T>& container) {
    return detail::format_sequence(out, container);
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& container) {
    return detail::format_sequence(out, container);
}

template <typename Key, typename Val>
std::ostream& operator<<(std::ostream& out,
                         const std::map<Key, Val>& container) {
    return detail::format_map(out, container);
}

template <typename Key, typename Val>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_map<Key, Val>& container) {
    return detail::format_map(out, container);
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out,
                         const std::pair<T1, T2>& container) {
    return detail::format_tuple<2>(out, container);
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& out,
                         const std::tuple<Ts...>& container) {
    return detail::format_tuple<sizeof...(Ts)>(out, container);
}

} // namespace samwarring::container_ostream

#endif