#ifndef BASIC_MATRIX_INDEX_ITERATOR_H
#define BASIC_MATRIX_INDEX_ITERATOR_H

#include <iterator>
#include "basic_pos.h"

//forward declear
template<class _Matrix, class _Value_Type >
class Basic_Matrix_Col_Index_Iterator;

template<class _Matrix,
         class _Value_Type = typename _Matrix::value_type
         >
class Basic_Matrix_Index_Iterator
        : public std::iterator
        <std::random_access_iterator_tag,
        _Value_Type,
        typename _Matrix::size_type>
{
    typedef Basic_Matrix_Index_Iterator<_Matrix, _Value_Type> _Self;
public:
    typedef _Matrix Matrix;
    typedef std::iterator
    <std::forward_iterator_tag,
    _Value_Type,
    typename _Matrix::size_type> Parent;
    typedef typename Matrix::size_type size_type;
    Basic_Matrix_Index_Iterator(Matrix *m, size_type index)
        :m_matrix(m), m_index(index)
    {}

    Basic_Pos<size_type> pos() const
    {
        return {m_index/m_matrix->col_size(), m_index%m_matrix->col_size()};
    }

    bool operator < (const _Self &right) const
    {
        assert(m_matrix == right.m_matrix);
        return  m_index < right.m_index;
    }

    bool operator != (const _Self &right) const
    {
        assert(m_matrix == right.m_matrix);
        return  m_index != right.m_index;
    }

    bool operator == (const _Self &right) const
    {
        assert(m_matrix == right.m_matrix);
        return  m_index == right.m_index;
    }

    size_type operator - (const _Self &other) const
    {
        assert(m_matrix == other.m_matrix);
        return  m_index - other.m_index;
    }

    typename Parent::reference operator*() const
    {
        return m_matrix->iter_ref(pos());
    }

    typename Parent::pointer operator ->() const
    {
        return &m_matrix->iter_ref(pos());
    }

    _Self &operator++()
    {
        ++m_index;
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
        --m_index;
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
        m_index += d;
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
        m_index -= d;
        return *this;
    }

    _Self operator-(size_type d) const
    {
        _Self other(*this);
        other -= d;
        return other;
    }
private:
    Matrix *m_matrix;
    size_type m_index;
    friend class Basic_Matrix_Col_Index_Iterator<Matrix, _Value_Type>;
};

//col iterator
template<class _Matrix,
         class _Value_Type = typename _Matrix::value_type
         >
class Basic_Matrix_Col_Index_Iterator
        : public std::iterator
        <std::random_access_iterator_tag,
        _Value_Type,
        typename _Matrix::size_type>
{
    typedef Basic_Matrix_Col_Index_Iterator<_Matrix, _Value_Type> _Self;
    typedef Basic_Matrix_Index_Iterator<_Matrix, _Value_Type> IndexIter;
public:
    typedef _Matrix Matrix;
    typedef std::iterator
    <std::forward_iterator_tag,
    _Value_Type,
    typename _Matrix::size_type> Parent;
    typedef typename Matrix::size_type size_type;
    Basic_Matrix_Col_Index_Iterator(Matrix *m, size_type index)
        :m_iter(m, index)
    {}

    Basic_Pos<size_type> pos() const
    {
        return m_iter.pos();
    }

    bool operator < (const _Self &right) const
    {
        return m_iter < right.m_iter;
    }

    bool operator != (const _Self &right) const
    {
        return  m_iter != right.m_iter;
    }

    bool operator == (const _Self &right) const
    {
        return  m_iter == right.m_iter;
    }

    size_type operator - (const _Self &other) const
    {
        return  (m_iter - other.m_iter) / m_iter.m_matrix->col_size();
    }

    typename Parent::reference operator*() const
    {
        return m_iter.operator *();
    }

    typename Parent::pointer operator ->() const
    {
        return m_iter.operator ->();
    }

    _Self &operator++()
    {
        m_iter += m_iter.m_matrix->col_size();
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
        m_iter -= m_iter.m_matrix->col_size();
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
        m_iter += d*m_iter.m_matrix->col_size();
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
        m_iter -= d*m_iter.m_matrix->col_size();
        return *this;
    }

    _Self operator-(size_type d) const
    {
        _Self other(*this);
        other -= d;
        return other;
    }
private:
    IndexIter m_iter;
};

#endif // BASIC_MATRIX_INDEX_ITERATOR_H
