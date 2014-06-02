#ifndef MATH_HPP
#define MATH_HPP

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
