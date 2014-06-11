#ifndef UTILS_MATH_HPP
#define UTILS_MATH_HPP

#ifdef MODULE_APRICOTUTILS
    using std::min;
    using std::max;
#endif // MODULE_APRICOTUTILS

template <typename T>
inline const T& clamp(const T& a, const T& min, const T& max)
{
    if (a <= min)
        return min;
    if (max <= a)
        return max;
    else
        return a;
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}


#endif // UTILS_MATH_HPP
