#ifndef MY_UTIL_H_
#define MY_UTIL_H_

#include <cstddef>

#include "type_traits.hpp"

// 这个文件包含一些工具：如swap、move、forward、pair等。
namespace mystl {

// remove_reference
template <class T>
struct remove_reference         {typedef T type;};
template <class T>
struct remove_reference<T&>     {typedef T type;};
template <class T>
struct remove_reference<T&&>    {typedef T type;};

// move

template <class T>
typename remove_reference<T>::type&& move(T &&arg) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

// forward

template <class T>
T&& forward(typename std::remove_reference<T>::type &arg) noexcept {
    return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type &&arg) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "bad_forward");
    return static_cast<T&&>(arg);
}

// swap
template <class Tp>
void swap(Tp &lhs, Tp &rhs) {
    auto tmp(mystl::move(lhs));
    lhs = mystl::move(rhs);
    rhs = mystl::move(tmp);
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter1 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2) {
    for(;first1 != last1; ++first1, ++first2)
        mystl::swap(*first1, *first2);
    return first2;
}

template <class Tp, size_t N>
void swap(Tp(&a)[N], Tp(&b)[N]) {
    mystl::swap_range(a, a + N, b);
}


};          /* end of namespace mystl */

#endif      /* end of MY_UTIL_H_ */