#ifndef BASIC_MATRIX_ITERATOR_H
#define BASIC_MATRIX_ITERATOR_H

#include <iterator>
#include "basic_pos.h"

template<class _Matrix,
         class _Value_Type = typename _Matrix::value_type
         >
class Basic_Matrix_Iterator
        : public std::iterator
        <std::random_access_iterator_tag,
        _Value_Type,
        typename _Matrix::size_type>
{
    typedef Basic_Matrix_Iterator<_Matrix, _Value_Type> _Self;
public:
    typedef _Matrix Matrix;
    typedef std::iterator
    <std::forward_iterator_tag,
    _Value_Type,
    typename _Matrix::size_type> Parent;
    typedef typename Matrix::size_type size_type;
    Basic_Matrix_Iterator(Matrix *m, size_type row, size_type col)
        :m_matrix(m), m_pos(row, col)
    {}

    Basic_Pos<size_type> pos() const
    {
        return m_pos;
    }

    bool operator < (const _Self &right) const
    {
        assert(m_matrix == right.m_matrix);
        return  m_pos < right.m_pos;
    }

    bool operator != (const _Self &right) const
    {
        assert(m_matrix == right.m_matrix);
        return  m_pos != right.m_pos;
    }

    bool operator == (const _Self &right) const
    {
        assert(m_matrix == right.m_matrix);
        return  m_pos == right.m_pos;
    }

    size_type operator - (const _Self &other) const
    {
        assert(m_matrix == other.m_matrix);
        return to_index() - other.to_index();
    }

    typename Parent::reference operator*() const
    {
        return m_matrix->iter_ref(m_pos);
    }

    _Self &operator++()
    {
        m_pos.col()++;
        if (m_pos.col() == m_matrix->col_size()) {
            m_pos.col() = 0;
            m_pos.row()++;
        }
        return *this;
    }

    _Self operator++(int)
    {
        _Self tmp(*this);
        ++*this;
        return tmp;
    }

    _Self &operator--()
    {
        if (m_pos.col() == 0) {
            m_pos.col() = m_matrix->col_size()-1;
            m_pos.row()--;
        }
        else {
            m_pos.col()--;
        }
        return *this;
    }

    _Self operator--(int)
    {
        _Self tmp(*this);
        --*this;
        return tmp;
    }

    _Self &operator+=(size_type d)
    {
        from_index(to_index()+d);
        return *this;
    }

    _Self operator+(size_type d) const
    {
        _Self other(*this);
        other += d;
        return other;
    }

    _Self &operator-=(size_type d)
    {
        from_index(to_index()-d);
        return *this;
    }

    _Self operator-(size_type d) const
    {
        _Self other(*this);
        other -= d;
        return other;
    }
private:
    inline size_type to_index() const
    {
        return m_pos.row()*m_matrix->col_size() + m_pos.col();
    }

    inline void from_index(size_type index)
    {
        m_pos.row() = index / m_matrix->col_size();
        m_pos.col() = index % m_matrix->col_size();
    }
private:
    Matrix *m_matrix;
    Basic_Pos<size_type> m_pos;
};
#endif // BASIC_MATRIX_ITERATOR_H
