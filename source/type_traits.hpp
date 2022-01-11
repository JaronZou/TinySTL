#ifndef MY_TYPE_TRAITS_H_
#define MY_TYPE_TRAITS_H_

#include <type_traits>

#include "util.hpp"

namespace mystl {

    template<class T, T v> 
    struct m_integral_constant {
        static constexpr T value = v;
    };

    template<bool b>
    using m_bool_constant = m_integral_constant<bool, b>;
    
    typedef m_bool_constant<true>   my_true_type;
    typedef m_bool_constant<false>  my_false_type;

    //*********************************************************************
    
};              /* end of namespace mystl */

#endif          /* end of MY_TYPE_TRAITS_H_ */