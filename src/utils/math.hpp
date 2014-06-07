#ifndef MATH_HPP
#define MATH_HPP

#ifdef APRICOTUTILS_HPP
    using std::min;
    using std::max;
#endif // APRICOTUTILS_HPP

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

#endif // MATH_HPP
