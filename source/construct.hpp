#ifndef MY_CONSTRUCT_H_
#define MY_CONSTRUCT_H_

#include <new>

#include "type_traits.hpp"
#include "iterator.hpp"
#include "util.hpp"

namespace mystl {

template <class T>
void construct(T *ptr) {
    ::new ((void*)ptr) T();
}

template <class T1, class T2>
void construct(T1 *ptr, const T2 &val) {
    ::new ((void*)ptr) T1(val);
}

template <class T, class... Args>
void construct(T *ptr, Args&&... args) {
    ::new ((void*)ptr) T(mystl::forward<Args>(args)...);
}

// destroy
template <class Ty>
void destroy_one(Ty*, std::true_type) {}

template <class Ty>
void destroy_one(Ty *pointer, std::false_type) {
    if(pointer != nullptr) {
        pointer->~Ty();
    }
}

template <class ForwardIter>
void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
    for(;first != last; ++first)
        destroy(&*first);
}

template <class Ty>
void destroy(Ty *pointer) {
    destroy_one(pointer, std::is_trivially_destructible<Ty>::type);
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last) {
    destroy_cat(first, last, std::is_trivially_destructible<
                typename iterator_traits<ForwardIter>::value_type>::type);
}

};      /* end of namespace mystl */

#endif  /* end of MY_CONSTRUCT_H_ */