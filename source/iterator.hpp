#ifndef MY_ITERATOR_H_
#define MY_ITERATOR_H_

#include <cstddef>

#include "type_traits"

namespace mystl {
    // 5种迭代器tag
    struct input_iterator_tag   {};
    struct output_iterator_tag  {};
    struct forward_iterator_tag : public input_iterator_tag                 {};
    struct bidrectional_iterator_tag : public forward_iterator_tag          {};
    struct random_access_iterator_tag : public bidrectional_iterator_tag    {};

    // 迭代器模板
    template <class _Category, 
             class _T,
             class _Difference = ptrdiff_t,
             class _Pointer = _T*,
             class _Reference = _T&>
    class iterator {
        typedef _Category       iterator_category;
        typedef _T              value_type;
        typedef _Difference     difference_type;
        typedef _Pointer        pointer;
        typedef _Reference      reference;
    };

    // 用于判断该类型T有无iterator_category类型
    template <typename T>
    struct has_iterator_cat {
    private:
        struct two {char, char};
        template <typename U> static struct two test(...);
        template <typename U> static char test(typename U::iterator_category* = 0);
    public:
        static const bool value = sizeof(char) == sizeof(test<T>(0));
    };

    template <typename Tt, bool>
    struct iterator_traits_impl {};

    template <typename It>
    struct iterator_traits_impl<It, true> {
        typedef It::iterator_category   iterator_category;
        typedef It::value_type          value_type;
        typedef It::difference_type     difference_type;
        typedef It::pointer             pointer;
        typedef It::reference           reference;
    };

    // 用于判断Iterator的iterator_category是不是能  
    // 转换为input_iterator_tag或output_iterator_tag
    template <typename It, bool>
    struct iterator_traits_helper {};

    template <typename It>
    struct iterator_traits_helper<It, true> 
        : public iterator_traits_impl<It, 
        std::is_convertible<typename It::iterator_category, input_iterator_tag>::value ||
        std::is_convertible<typename It::iterator_category, output_iterator_tag>::value>
    {
    };

    // iterator_traits
    template <typename It>
    struct iterator_traits
        : public iterator_traits_helper<It, has_iterator_cat<It>::value> 
    {
    };

    // iterator_traits针对指针类型的类模板特化
    template <typename T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    // iterator_traits针对const T*类型的特化
    template <typename T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef const T*                    pointer;
        typedef const T&                    reference;
    };

    template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of 
        : public m_bool_constant<std::is_convertible<
        typename iterator_traits<T>::iterator_category, U>::value>
    {
    };

    template <class T, class U>
    struct has_iterator_cat_of<T, U, false> : public my_false_type 
    {
    };

    // iterator的类型萃取
    template <class It>
    struct is_input_iterator : public has_iterator_cat_of<It, input_iterator_tag> {};
    template <class It>
    struct is_output_iterator : public has_iterator_cat_of<It, output_iterator_tag> {};
    template <class It>
    struct is_bidirectional_iterator : public has_iterator_cat_of<It, bidrectional_iterator_tag> {};
    template <class It>
    struct is_forward_iterator : public has_iterator_cat_of<It, forward_iterator_tag> {};
    template <class It>
    struct is_random_access_iterator : public has_iterator_cat_of<It, random_access_iterator_tag> {};

    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // distance的委托函数
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance_dispatch(InputIterator beg, InputIterator end, input_iterator_tag) {
        typename iterator_traits<beg>::difference_type dis = 0;
        while(beg != end) {
            ++beg;
            ++dis;
        }
        return dis;
    }

    template<class RandomAccessIterator>
    typename iterator_traits<RandomAccessIterator>::difference_type
    distance_dispatch(RandomAccessIterator beg, RandomAccessIterator end, random_access_iterator_tag) {
        return static_cast<typename iterator_traits<RandomAccessIterator>::difference_type>(end - beg);    
    }

    // 包装函数
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator beg, InputIterator end) {
        return distance_dispatch(beg, end, iterator_category(beg));
    }

    // advance的委托函数
    template<class InputIterator, class Distance>
    void advance_dispatch(InputIterator &beg, Distance n, input_iterator_tag) {
        while(n--)
            ++beg;
    }

    template<class BidrectionalIterator, class Distance>
    void advance_dispatch(BidrectionalIterator &beg, Distance n, bidrectional_iterator_tag) {
        if(n < 0)
            while(n++) {--beg;}
        else
            while(n--) {++beg;}
    }

    template<class RandomAccessIterator, class Distance>
    void advance_dispatch(RandomAccessIterator &beg, Distance n, random_access_iterator_tag) {
        beg += n;
    }

    template<class InputIterator, class Distance = typename iterator_traits<InputIterator>::difference_type>
    void advance(InputIterator &beg, Distance n) {
        advance_dispatch(beg, n, iterator_category(beg));
    }


    /********************************************************************
     * 模板类reverse_iterator
     * 是iterator的适配器，前进为后退，后退为前进。
     ********************************************************************/
    template <class Iterator>
    class reverse_iterator {
    private:
        Iterator current;
    
    public:
        // 反向迭代器typedef
        typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
        typedef typename iterator_traits<Iterator>::value_type          value_type;
        typedef typename iterator_traits<Iterator>::difference_type     difference_type;
        typedef typename iterator_traits<Iterator>::pointer             pointer;
        typedef typename iterator_traits<Iterator>::reference           reference;

        typedef typename Iterator                                       iterator_type;
        typedef reverse_iterator<Iterator>                              self;

    public:
        // 构造函数
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type i) : current(i){}
        reverse_iterator(const self &rhs) : current(rhs.current) {}

    public:
        // 取出对应的正向迭代器
        iterator_type base() const 
        {return current;}

        // 重载操作符*和->
        reference operator*() {
            auto tmp = current;
            return --*tmp;
        }
        pointer operator->() {
            return &(operator*());
        }

        // 重载前进(++)运算符        
        self& operator++() {
            --current;
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            --current;
            return tmp;
        }
        // 重载后退(--)运算符
        self& operator--() {
            ++current;
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            ++current;
            return tmp;
        }

        self& operator+=(difference_type n) {
            current -= n;
            return *this;
        }
        self operator+(difference_type n) {
            return self(current - n);
        }
        self& operator-=(difference_type n) {
            current += n;
            return *this;
        }
        self operator-(difference_type n) {
            return self(current + n);
        }

        reference operator[](difference_type n) {
            return *(*this + n);
        }
    };
};                  /* end of MY_ITERATOR_H_ */
#endif              /* end of MY_ITERATOR_H_ */