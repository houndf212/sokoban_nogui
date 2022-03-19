#include "types.h"
#include <numeric>
#include <algorithm>

void check_range_iterator()
{
    IntMatrix m(2, 2);

    int i=1;
    for (auto &v : m.range()) {
        v = i++;
    }

    i = 1;
    assert(m.get(Pos(0, 0)) == i++);
    assert(m.get(Pos(0, 1)) == i++);
    assert(m.get(Pos(1, 0)) == i++);
    assert(m.get(Pos(1, 1)) == i++);

    const IntMatrix &cm = m;
    i = 1;
    for (auto &v : cm.range()) {
        assert(v == i++);
    }
}

void check_range_reverse_iterator()
{
    IntMatrix m(2, 2);

    int i=1;
    for (auto &v : m.reverse_range()) {
        v = i++;
    }

    assert(m.get(Pos(0, 0)) == 4);
    assert(m.get(Pos(0, 1)) == 3);
    assert(m.get(Pos(1, 0)) == 2);
    assert(m.get(Pos(1, 1)) == 1);

    const IntMatrix &cm = m;
    i = 1;
    for (auto &v : cm.reverse_range()) {
        assert(v == i++);
    }

}

void check_row_iterator()
{
    IntMatrix m(2, 2);

    int i=10;
    for (auto &v : m.row_range(0)) {
        v = i;
        i+=10;
    }
    assert(m.get(Pos(0, 0)) == 10);
    assert(m.get(Pos(0, 1)) == 20);

    i=100;
    for (auto &v : m.row_range(1)) {
        v = i;
        i+=100;
    }
    assert(m.get(Pos(1, 0)) == 100);
    assert(m.get(Pos(1, 1)) == 200);
}

void check_row_reverse_iterator()
{
    IntMatrix m(2, 2);

    int i=10;
    for (auto &v : m.row_reverse_range(0)) {
        v = i;
        i+=10;
    }
    assert(m.get(Pos(0, 0)) == 20);
    assert(m.get(Pos(0, 1)) == 10);

    i=100;
    for (auto &v : m.row_reverse_range(1)) {
        v = i;
        i+=100;
    }
    i=100;
    const auto &cm = m;
    for (auto &v : cm.row_reverse_range(1)) {
        assert(v==i);
        i+=100;
    }
}

void check_col_iterator()
{
    IntMatrix m(2, 2);

    int i=10;
    for (auto &v : m.col_range(0)) {
        v = i;
        i+=10;
    }
    assert(m.get(Pos(0, 0)) == 10);
    assert(m.get(Pos(1, 0)) == 20);

    i=100;
    for (auto &v : m.col_range(1)) {
        v = i;
        i+=100;
    }
    assert(m.get(Pos(0, 1)) == 100);
    assert(m.get(Pos(1, 1)) == 200);
}

void check_col_reverse_iterator()
{
    IntMatrix m(2, 2);

    int i=10;
    for (auto &v : m.col_reverse_range(0)) {
        v = i;
        i+=10;
    }
    assert(m.get(Pos(0, 0)) == 20);
    assert(m.get(Pos(1, 0)) == 10);

    i=100;
    for (auto &v : m.col_reverse_range(1)) {
        v = i;
        i+=100;
    }

    const auto &cm = m;
    i = 100;
    for (auto &v : cm.col_reverse_range(1)) {
        assert(v==i);
        i+=100;
    }
}

void check_range_min_max()
{
    int n = 100;
    while(n-->0) {
        IntMatrix m(2, 3);

        std::vector<IntMatrix::value_type> vec(m.row_size()*m.col_size(), 0);
        std::iota(begin(vec), end(vec), n);
        std::random_shuffle(begin(vec), end(vec));

        std::copy(begin(vec), end(vec), m.begin());
        auto iter_min = std::min_element(m.begin(), m.end());
        assert(*iter_min == n);
        auto iter_max = std::max_element(m.rbegin(), m.rend());
        assert(*iter_max == n+int(vec.size())-1);
    }
}

void check_row_min_max()
{
    int n = 100;
    while(n-->0) {
        IntMatrix m(4, 4);

        std::vector<IntMatrix::value_type> vec(m.col_size(), 0);
        std::iota(begin(vec), end(vec), n);
        std::random_shuffle(begin(vec), end(vec));

        auto range = m.row_range(2);
        std::copy(begin(vec), end(vec), range.begin());
        auto iter_min = std::min_element(range.begin(),range.end());
        assert(*iter_min == n);
        auto iter_max = std::max_element(range.begin(),range.end());
        assert(*iter_max == n+int(vec.size())-1);
    }
}

void check_col_min_max()
{
    int n = 100;
    while(n-->0) {
        IntMatrix m(5, 6);;

        std::vector<IntMatrix::value_type> vec(m.row_size(), 0);
        std::iota(begin(vec), end(vec), n);
        std::random_shuffle(begin(vec), end(vec));

        auto range = m.col_range(3);
        std::copy(begin(vec), end(vec), range.begin());
        auto iter_min = std::min_element(range.begin(),range.end());
        assert(*iter_min == n);
        auto iter_max = std::max_element(range.begin(),range.end());
        assert(*iter_max == n+int(vec.size())-1);
    }
}

void check_forward_iterator()
{
    check_range_min_max();
    check_row_min_max();
    check_col_min_max();
}

void check_bidirectional_iterator()
{
    check_range_iterator();
    check_range_reverse_iterator();
    check_row_iterator();
    check_row_reverse_iterator();
    check_col_iterator();
    check_col_reverse_iterator();
}

void check_row_randomaccess_iterator()
{
    IntMatrix m(100, 50);

    std::vector<IntMatrix::value_type> vec(m.col_size(), 0);
    std::iota(begin(vec), end(vec), 0);
    std::random_shuffle(begin(vec), end(vec));

    auto range = m.row_range(10);
    std::copy(begin(vec), end(vec), range.begin());
    std::sort(range.begin(), range.end());

    assert(std::is_sorted(range.begin(), range.end()));

    {
        auto it = m.row_range(0).begin();
        ++it;
        it++;
        --it;
        it--;
    }
}

void check_col_randomaccess_iterator()
{
    IntMatrix m(100, 50);

    std::vector<IntMatrix::value_type> vec(m.row_size(), 0);
    std::iota(begin(vec), end(vec), 0);
    std::random_shuffle(begin(vec), end(vec));

    auto range = m.col_range(10);
    std::copy(begin(vec), end(vec), range.begin());
    std::sort(range.begin(), range.end());

    assert(std::is_sorted(range.begin(), range.end()));

    {
        auto it = m.col_range(0).end();
        ++it;
        it++;
        --it;
        it--;
    }
}

void check_range_rangdomaccess_iterator()
{
    IntMatrix m(5, 4);

    auto it = m.begin();
    *it = 10;
    auto it02 = it+2;
    *it02 = 20;
    auto it21 = it+ 2*m.col_size()+1;
    *it21 = 30;

    assert(m.get(Pos(0, 0)) == 10);
    assert(m.get(Pos(0, 2)) == 20);
    assert(m.get(Pos(2, 1)) == 30);


    std::vector<IntMatrix::value_type> vec(m.row_size()*m.col_size(), 0);
    std::iota(begin(vec), end(vec), 1);
    auto range = m.range();
    std::copy(begin(vec), end(vec), range.begin());
    {
        auto it = m.begin();
        assert(*it==1);
        auto it1 = it + 2;
        assert(*it1 == 1+2);
        auto it2 = it + 10;
        assert(*it2 == 1+10);

        assert(it2-it == 10);

        auto it3 = it2-5;
        assert(*it3 == *it2 - 5);
    }
    {
        auto it1 = m.begin();
        it1+=10;
        assert(*it1 == 1+10);
        it1-=5;
        assert(*it1 == 1+10-5);
    }

    {
        auto it = m.begin();
        ++it;


        auto it1 = m.begin();
        it1 += 10;
        assert(it1-it == 9);

        it--;
        it++;
    }


    std::random_shuffle(begin(vec), end(vec));

    std::copy(begin(vec), end(vec), range.begin());

    std::sort(range.begin(), range.end());

    assert(std::is_sorted(range.begin(), range.end()));
    assert(m.get(Pos(0, 3)) == 4);
    assert(m.get(Pos(1, 1)) == 6);

    assert(m.get(Pos(4, 3)) == 20);

    std::random_shuffle(range.begin(), range.end());
}

void check_randomaccess_iterator()
{
    check_row_randomaccess_iterator();
    check_col_randomaccess_iterator();
    check_range_rangdomaccess_iterator();
}

void check_operator_arrow()
{
    typedef Basic_Resize_Matrix<Pos, int> PosMatrix;
    PosMatrix m(2, 2);

    auto it = m.begin();
    *it = Pos(2, 2);
    assert(*it == Pos(2, 2));
    it->row() = 3;
    assert(*it == Pos(3, 2));
    assert(it->col() == 2);

    auto cit = m.col_range(1).begin();
    *cit = Pos(100, 200);
    assert(cit->row() == 100);
    cit->col() = 1000;
    assert((*cit).col() == 1000);
}

bool check_matrix_iterator()
{
    check_bidirectional_iterator();
    check_forward_iterator();
    check_randomaccess_iterator();

    check_operator_arrow();

    return true;
}
