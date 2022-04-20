#pragma once
#include <vector>
#include <utility>
#include <algorithm>

using std::vector;

template <typename T>
vector<T> operator+(const vector<T> &A, const vector<T> &B) {
    vector<T> AB;
    AB.reserve(A.size() + B.size());                // preallocate memory
    AB.insert(AB.end(), A.begin(), A.end());        // add A;
    AB.insert(AB.end(), B.begin(), B.end());        // add B;
    return AB;
}

template <typename T>
vector<T> &operator+=(vector<T> &A, const vector<T> &B) {
    A.reserve(A.size() + B.size());                // preallocate memory without erase original data
    A.insert(A.end(), B.begin(), B.end());         // add B;
    return A;                                        // here A could be named AB
}

template < typename T>
std::pair<bool, int > findInVector(const std::vector<T>& vecOfElements, const T& element)
{
    std::pair<bool, int > result;
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
    if (it != vecOfElements.end())
    {
        result.second = distance(vecOfElements.begin(), it);
        result.first = true;
    }
    else
    {
        result.first = false;
        result.second = -1;
    }
    return result;
}
