#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <algorithm>

template <typename T>
bool find(const std::vector<T> &v, const T &item)
{
    if(std::find(v.begin(), v.end(), item) == v.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

#endif // UTILS_HPP
