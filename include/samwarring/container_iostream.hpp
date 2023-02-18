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

struct detail {
    class out_seq {
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

        template <char BegChar, char EndChar, class Cont>
        static std::ostream& out(std::ostream& out, const Cont& container) {
            out << BegChar;
            auto it = container.begin();
            auto end = container.end();
            if (it != end) {
                out << *it;
                for (++it; it != end; ++it) {
                    out << ", " << *it;
                }
            }
            out << EndChar;
            return out;
        }
    };

    class out_map {
        template <typename Key, typename Val>
        friend std::ostream& operator<<(std::ostream&,
                                        const std::map<Key, Val>&);
        template <typename Key, typename Val>
        friend std::ostream& operator<<(std::ostream&,
                                        const std::unordered_map<Key, Val>&);

        template <class Cont>
        static std::ostream& out(std::ostream& out, const Cont& container) {
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
    };

    class out_tuple {
        template <typename T1, typename T2>
        friend std::ostream& operator<<(std::ostream&,
                                        const std::pair<T1, T2>&);
        template <typename... Ts>
        friend std::ostream& operator<<(std::ostream&,
                                        const std::tuple<Ts...>&);

        template <std::size_t N, class Cont>
        static std::ostream& out(std::ostream& out, const Cont& container) {
            out << '(';
            out_items<Cont, N, 0>(out, container);
            out << ')';
            return out;
        }

        template <class Cont, std::size_t Max, std::size_t Index>
        static void out_items(std::ostream& out, const Cont& container) {
            if constexpr (Index == Max) {
                // Do nothing
            } else if constexpr (Index == 0) {
                out << std::get<0>(container);
                out_items<Cont, Max, 1>(out, container);
            } else if constexpr (Index < Max) {
                out << ", " << std::get<Index>(container);
                out_items<Cont, Max, Index + 1>(out, container);
            }
        }
    };
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& container) {
    return detail::out_seq::out<'[', ']'>(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& container) {
    return detail::out_seq::out<'[', ']'>(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& container) {
    return detail::out_seq::out<'[', ']'>(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& container) {
    return detail::out_seq::out<'{', '}'>(out, container);
}

template <typename T>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_set<T>& container) {
    return detail::out_seq::out<'{', '}'>(out, container);
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<T, N>& container) {
    return detail::out_seq::out<'[', ']'>(out, container);
}

template <typename Key, typename Val>
std::ostream& operator<<(std::ostream& out,
                         const std::map<Key, Val>& container) {
    return detail::out_map::out(out, container);
}

template <typename Key, typename Val>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_map<Key, Val>& container) {
    return detail::out_map::out(out, container);
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out,
                         const std::pair<T1, T2>& container) {
    return detail::out_tuple::out<2>(out, container);
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& out,
                         const std::tuple<Ts...>& container) {
    return detail::out_tuple::out<sizeof...(Ts)>(out, container);
}

} // namespace samwarring::container_iostream

#endif