#ifndef BASIC_MATRIX_COL_ITERATOR_H
#define BASIC_MATRIX_COL_ITERATOR_H

#include <iterator>
#include "basic_pos.h"

template<class _Matrix,
         class _Value_Type = typename _Matrix::value_type
         >
class Basic_Matrix_Col_Iterator
        : public std::iterator
        <std::random_access_iterator_tag,
        _Value_Type,
        typename _Matrix::size_type>
{
    typedef Basic_Matrix_Col_Iterator<_Matrix, _Value_Type> _Self;
public:
    typedef _Matrix Matrix;
    typedef std::iterator
    <std::forward_iterator_tag,
    _Value_Type,
    typename _Matrix::size_type> Parent;
    typedef typename Matrix::size_type size_type;
    Basic_Matrix_Col_Iterator(Matrix *m, size_type row, size_type col)
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
        return m_pos.row() - other.m_pos.row();
    }

    typename Parent::reference operator*() const
    {
        return m_matrix->iter_ref(m_pos);
    }

    _Self &operator++()
    {
        m_pos.row()++;
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
        m_pos.row()--;
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
        m_pos.row()+=d;
        return *this;
    }

    _Self operator+(size_type d) const
    {
        _Self other(*this);
        other.m_pos.row()+=d;
        return other;
    }

    _Self &operator-=(size_type d)
    {
        m_pos.row()-=d;
        return *this;
    }

    _Self operator-(size_type d) const
    {
        _Self other(*this);
        other.m_pos.row()-=d;
        return other;
    }

private:
    Matrix *m_matrix;
    Basic_Pos<size_type> m_pos;
};

#endif // BASIC_MATRIX_COL_ITERATOR_H
