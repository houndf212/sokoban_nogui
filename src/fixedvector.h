#ifndef FIXEDVECTOR_H
#define FIXEDVECTOR_H

#include <assert.h>
#include "util.h"

using FIXED_SIZE_TYPE = int;

template<typename T, FIXED_SIZE_TYPE N>
class FixedVector
{
public:
    FixedVector() : m_size(0) {}
    FIXED_SIZE_TYPE size() const { return m_size; }

    T & at(size_t n)
    {
        assert(0 <= n && n < m_size);
        return m_arr[n];
    }

    template<typename... ARGS>
    void append(ARGS... args)
    {
        if (m_size >= N)
            util::exit_with_error("Fixed vector size out of range");

        m_arr[m_size] = T(args...);
        m_size++;
    }
private:
    void setSize(FIXED_SIZE_TYPE n)
    {
        assert(0 <= n && n < N);
        m_size = n;
    }
private:
    T               m_arr[N];
    FIXED_SIZE_TYPE m_size;
};

template<typename T, FIXED_SIZE_TYPE _R, FIXED_SIZE_TYPE _C>
class FixedMatix
{
public:
    FixedMatix() = default;
    template<typename Pos>
    T & at(const Pos &p)
    {
        return at(p.row(), p.col());
    }
    T & at(FIXED_SIZE_TYPE r, FIXED_SIZE_TYPE c)
    {
        assert(0 <= r && r < _R  && 0 <= c && c < _C);
        return m_mat[r][c];
    }
private:
    T m_mat[_R][_C];
};


#endif // FIXEDVECTOR_H
