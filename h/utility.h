#ifndef _UTILITY_H_
#define _UTILITY_H_

template<class T>
const T & max(const T & lhs, const T & rhs) {
    return lhs < rhs ? rhs : lhs;
}

template<class T>
const T & min(const T & lhs, const T & rhs) {
    return lhs > rhs ? rhs : lhs;
}

#endif
