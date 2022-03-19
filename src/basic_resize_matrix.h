#ifndef BASIC_RESIZE_MATRIX_H
#define BASIC_RESIZE_MATRIX_H
#include <vector>
#include <limits>
#include <assert.h>
#include "basic_pos.h"
//#include "basic_matrix_iterator.h"
//#include "basic_matrix_row_iterator.h"
//#include "basic_matrix_col_iterator.h"
#include "iteratorrange.h"
#include "basic_matrix_index_iterator.h"

template<typename T, typename S>
class Basic_Resize_Matrix
{
public:
    typedef T value_type;
    typedef S size_type;

    static constexpr size_type szero() { return S(0); }
    static constexpr value_type vmax() { return std::numeric_limits<value_type>::max(); }

//    Basic_Resize_Matrix(const Basic_Resize_Matrix&) = delete;
//    Basic_Resize_Matrix &operator =(const Basic_Resize_Matrix&) = delete;

    Basic_Resize_Matrix() : n_row(0), n_col(0) {}
    Basic_Resize_Matrix(size_type row, size_type col, value_type val=value_type())
    {
        n_row = row;
        n_col = col;

        std::vector<value_type> vec(n_col, val);
        mat = std::vector<std::vector<value_type>>(row, vec);
    }

    //完成 扩展矩阵
    void resize(size_type row, size_type col, value_type val=value_type())
    {
        if (row == row_size() && col == col_size()) {
            return;
        }
        else if (row == row_size()) {
            resize_col(col, val);
        }
        else {
            if (col == col_size()) {
                resize_row(row, col, val);
                //return;
            }
            //这两个交换顺序为的是减少行resize的时候减少调用次数
            else if (row > row_size()) {
                //当扩展行时，先对其列
                resize_col(col, val);
                resize_row(row, col, val);
            }
            else {
                //当缩小行时，先缩小列
                resize_row(row, col, val);
                resize_col(col, val);
            }

        }
    }

    void fill(value_type val)
    {
        for (auto &vec : mat) {
            std::fill(std::begin(vec), std::end(vec), val);
        }
    }

    template<typename U>
    value_type get(Basic_Pos<U> p) const
    {
        assert(isInMatrix(p));
        return mat[p.row()][p.col()];
    }

    template<typename U>
    void set(Basic_Pos<U> p, value_type val)
    {
        assert(isInMatrix(p));
        mat[p.row()][p.col()] = val;
    }

    template<typename U>
    bool isInMatrix(Basic_Pos<U> p) const
    {
        return 0<=p.row() && p.row()<row_size() && 0<=p.col() && p.col()<col_size();
    }

    size_type row_size() const { return n_row; }
    size_type col_size() const { return n_col; }

    bool equal(const Basic_Resize_Matrix& bm) const
    {
        assert(size()==bm.size());
        return mat == bm.mat;
    }

    bool less(const Basic_Resize_Matrix& bm) const
    {
        assert(size()==bm.size());
        return mat < bm.mat;
    }
private:
    void resize_row(size_type row, size_type col, value_type val)
    {
        assert(row != row_size());
        mat.resize(row, std::vector<value_type>(col, val));
        n_row = row;
    }

    void resize_col(size_type col, value_type val)
    {
        assert(col_size() != val);
        for (auto &vec : mat) {
            vec.resize(col, val);
        }
        n_col = col;
    }

    std::pair<size_type, size_type> size() const
    {
        return std::make_pair(row_size(), col_size());
    }
private:
    size_type n_row;
    size_type n_col;
    std::vector<std::vector<value_type>> mat;

private:
    value_type &iter_ref(const Basic_Pos<size_type> &p)
    {
        assert(isInMatrix(p));
        return mat[p.row()][p.col()];
    }

    const value_type &iter_ref(const Basic_Pos<size_type> &p) const
    {
        assert(isInMatrix(p));
        return mat[p.row()][p.col()];
    }
    //迭代器
//    friend class Basic_Matrix_Iterator<Basic_Resize_Matrix>;
//    typedef Basic_Matrix_Iterator<Basic_Resize_Matrix> iterator;

//    friend class Basic_Matrix_Iterator<const Basic_Resize_Matrix, const value_type>;
//    typedef Basic_Matrix_Iterator<const Basic_Resize_Matrix, const value_type> const_iterator;

    //行迭代器
//    friend class Basic_Matrix_Row_Iterator<Basic_Resize_Matrix>;
//    typedef Basic_Matrix_Row_Iterator<Basic_Resize_Matrix> row_iterator;

//    friend class Basic_Matrix_Row_Iterator<const Basic_Resize_Matrix, const value_type>;
//    typedef Basic_Matrix_Row_Iterator<const Basic_Resize_Matrix, const value_type> const_row_iterator;

//    typedef std::reverse_iterator<row_iterator>            row_reverse_iterator;
//    typedef std::reverse_iterator<const_row_iterator>      const_row_reverse_iterator;

//    //列迭代器
//    friend class Basic_Matrix_Col_Iterator<Basic_Resize_Matrix>;
//    typedef Basic_Matrix_Col_Iterator<Basic_Resize_Matrix> col_iterator;

//    friend class Basic_Matrix_Col_Iterator<const Basic_Resize_Matrix, const value_type>;
//    typedef Basic_Matrix_Col_Iterator<const Basic_Resize_Matrix, const value_type> const_col_iterator;

//    typedef std::reverse_iterator<col_iterator>            col_reverse_iterator;
//    typedef std::reverse_iterator<const_col_iterator>      const_col_reverse_iterator;
public:
    // range iterator
    friend class Basic_Matrix_Index_Iterator<Basic_Resize_Matrix, value_type>;
    friend class Basic_Matrix_Index_Iterator<const Basic_Resize_Matrix, const value_type>;

    typedef Basic_Matrix_Index_Iterator<Basic_Resize_Matrix, value_type>             iterator;
    typedef Basic_Matrix_Index_Iterator<const Basic_Resize_Matrix, const value_type> const_iterator;
    typedef std::reverse_iterator<iterator>                                          reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                                    const_reverse_iterator;

    iterator
    begin()
    { return {this, 0}; }

    iterator
    end()
    { return {this, row_size()*col_size()}; }

    const_iterator
    begin() const
    { return {this, 0}; }

    const_iterator
    end() const
    { return {this, row_size()*col_size()}; }

    reverse_iterator
    rbegin()
    { return reverse_iterator(end()); }

    reverse_iterator
    rend()
    { return reverse_iterator(begin()); }

    const_reverse_iterator
    rbegin() const
    { return const_reverse_iterator(end()); }

    const_reverse_iterator
    rend() const
    { return const_reverse_iterator(begin()); }

    IteratorRange<iterator>
    range()
    { return {begin(), end()}; }

    IteratorRange<const_iterator>
    range() const
    { return {begin(), end()}; }

    IteratorRange<reverse_iterator>
    reverse_range()
    { return {rbegin(), rend()}; }

    IteratorRange<const_reverse_iterator>
    reverse_range() const
    { return {rbegin(), rend()}; }

    // row iterator, same as range iterator
    typedef iterator                row_iterator;
    typedef const_iterator          const_row_iterator;
    typedef reverse_iterator        row_reverse_iterator;
    typedef const_reverse_iterator  const_row_reverse_iterator;

    row_iterator
    row_begin(size_type row)
    { return {this, row*col_size()}; }

    row_iterator
    row_end(size_type row)
    { return {this, (row+1)*col_size()}; }

    const_row_iterator
    row_begin(size_type row) const
    { return {this, row*col_size()}; }

    const_row_iterator
    row_end(size_type row) const
    { return {this, (row+1)*col_size()}; }

    row_reverse_iterator
    row_rbegin(size_type row)
    { return row_reverse_iterator(row_end(row)); }

    row_reverse_iterator
    row_rend(size_type row)
    { return row_reverse_iterator(row_begin(row)); }

    const_row_reverse_iterator
    row_rbegin(size_type row) const
    { return const_row_reverse_iterator(row_end(row)); }

    const_row_reverse_iterator
    row_rend(size_type row) const
    { return const_row_reverse_iterator(row_begin(row)); }

    IteratorRange<row_iterator>
    row_range(size_type row)
    { return {row_begin(row), row_end(row)}; }

    IteratorRange<const_row_iterator>
    row_range(size_type row) const
    { return {row_begin(row), row_end(row)}; }

    IteratorRange<row_reverse_iterator>
    row_reverse_range(size_type row)
    { return {row_rbegin(row), row_rend(row)}; }

    IteratorRange<const_row_reverse_iterator>
    row_reverse_range(size_type row) const
    { return {row_rbegin(row), row_rend(row)}; }

    // col iterator
    typedef Basic_Matrix_Col_Index_Iterator<Basic_Resize_Matrix, value_type>                col_iterator;
    typedef Basic_Matrix_Col_Index_Iterator<const Basic_Resize_Matrix, const value_type>    const_col_iterator;
    typedef std::reverse_iterator<col_iterator>                                             col_reverse_iterator;
    typedef std::reverse_iterator<const_col_iterator>                                       const_col_reverse_iterator;

    col_iterator
    col_begin(size_type col)
    { return {this, col}; }

    col_iterator
    col_end(size_type col)
    { return {this, row_size()*col_size()+col}; }

    const_col_iterator
    col_begin(size_type col) const
    { return {this, col}; }

    const_col_iterator
    col_end(size_type col) const
    { return {this, row_size()*col_size()+col}; }

    col_reverse_iterator
    col_rbegin(size_type col)
    { return col_reverse_iterator(col_end(col)); }

    col_reverse_iterator
    col_rend(size_type col)
    { return col_reverse_iterator(col_begin(col)); }

    const_col_reverse_iterator
    col_rbegin(size_type col) const
    { return const_col_reverse_iterator(col_end(col)); }

    const_col_reverse_iterator
    col_rend(size_type col) const
    { return const_col_reverse_iterator(col_begin(col)); }

    IteratorRange<col_iterator>
    col_range(size_type col)
    { return {col_begin(col), col_end(col)}; }

    IteratorRange<const_col_iterator>
    col_range(size_type col) const
    { return {col_begin(col), col_end(col)}; }

    IteratorRange<col_reverse_iterator>
    col_reverse_range(size_type col)
    { return {col_rbegin(col), col_rend(col)}; }

    IteratorRange<const_col_reverse_iterator>
    col_reverse_range(size_type col) const
    { return {col_rbegin(col), col_rend(col)}; }
};

template<typename T, typename S>
inline bool operator == (const Basic_Resize_Matrix<T, S> &a, const Basic_Resize_Matrix<T, S> &b)
{
    return a.equal(b);
}

#endif // BASIC_RESIZE_MATRIX_H
