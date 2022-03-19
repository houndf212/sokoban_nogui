#ifndef ITERATORRANGE_H
#define ITERATORRANGE_H

template <typename _Iter>
class IteratorRange
{
public:
    IteratorRange(_Iter &&begin, _Iter &&end)
        : m_begin(begin), m_end(end)
    {}

    Basic_Pos<typename _Iter::difference_type> pos() const
    {
        return m_begin.pos();
    }

    typename _Iter::difference_type row() const
    {
        return m_begin.pos().row();
    }

    typename _Iter::difference_type col() const
    {
        return  m_begin.pos().col();
    }

    operator bool() const
    {
        return m_begin != m_end;
    }

    void operator ++()
    {
        ++m_begin;
    }

    void next()
    {
        ++m_begin;
    }

    typename _Iter::reference operator * () const
    {
        return *m_begin;
    }

    //为了 实现 for (auto &v : m.range([0]));
    _Iter begin() const { return m_begin; }
    _Iter end() const { return m_end; }

private:
    _Iter m_begin;
    const _Iter m_end;
};

#endif // ITERATORRANGE_H
