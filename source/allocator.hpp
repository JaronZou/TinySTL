#ifndef MY_ALLOCATOR_H_
#define MY_ALLOCATOR_H_

#include "construct.hpp"
#include "util.hpp"

namespace mystl {

// 模板类：allocator
// 模板函数代表数据类型
template <class T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

public:
    allocator() {}
    allocator(const allocator &) {}

    static T* allocate();
    static T* allocate(size_type n);
    
    static void deallocate(T *p);
    static void deallocate(T *p, size_type n);
    
    static void construct(T *p);
    static void construct(T *p, const T &value);
    static void construct(T *p, T &&value);

    template <class... Args>
    static void construct(T *p, Args** ...args);

    static void destroy(T *p);
    static void destroy(T *first, T *last);
};

template <class T>
static T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}

template <class T>
static T* allocator<T>::allocate(size_type n) {
    if(n == 0)
        return nullptr;
    return static_cast<T*>(::operator new(n*sizeof(T)));
}

template <class T>
static void allocator<T>::deallocate(T *p) {
    if(p == nullptr)
        return;
    ::operator delete(p);
}

template <class T>
static void allocator<T>::deallocate(T *p, size_type n) {
    if(p == nullptr)
        return;
    ::operator delete(p);
}

template <class T>
static void allocator<T>::construct(T *p) {
    mystl::construct(p);
}

template <class T>
static void allocator<T>::construct(T *p, const T &value) {
    mystl::construct(p, mystl::move(value));
}

template <class T>
static void allocator<T>::construct(T *p, T &&value) {
    mystl::construct(p, mystl::move(value));
}

template <class T>
template <class... Args>
static void allocator<T>::construct(T *p, Args** ...args) {
    mystl::construct(p, mystl::move(args));
}

template <class T>
static void allocator<T>::destroy(T *p) {
    mystl::destroy(p);
}

template <class T>
static void allocator<T>::destroy(T *first, T *last) {
    mystl::destroy(first, last);
}

}               /* end of namespace mystl */

#endif          /* end of MY_ALLOCATOR_H_ */