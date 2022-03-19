#ifndef ASSIGNMENTPROBLEM_H
#define ASSIGNMENTPROBLEM_H
#include "types.h"
#include "hungarianalg.h"
#include <numeric>

namespace AssignmentProblem
{

template<typename weight_t>
std::pair<PosVector, weight_t>
enumeration_method(const IntMatrix &m)
{
    typedef std::vector<IntMatrix::size_type> ColVec;
    ColVec arr;
    arr.resize(m.row_size());
    std::iota(begin(arr), end(arr), IntMatrix::size_type(0));

    auto func = [](const IntMatrix &m, const ColVec &vec) {
        weight_t w = 0;
        for (auto row=m.szero(); row<m.row_size(); ++row) {
            Pos p(row, vec[row]);
            assert(m.isInMatrix(p));
            w += m.get(p);
        }
        return w;
    };

    ColVec min_col = arr;
    weight_t min = func(m, arr);
    while (std::next_permutation(begin(arr), end(arr))) {
        auto t = func(m, arr);
        if (t < min) {
            min = t;
            min_col = arr;
        }
    }

    PosVector min_vec;
    min_vec.resize(min_col.size());
    for (auto row=m.szero(); row<m.row_size(); ++row) {
        min_vec[row] = Pos(row, min_col[row]);
    }
    return std::make_pair(min_vec, min);
}

template<typename weight_t>
std::pair<PosVector, weight_t>
hungarian_method(const IntMatrix &m)
{
    HungarianAlg<IntMatrix::value_type> hun;
    return hun.solve(m);
}

template<typename weight_t>
std::pair<PosVector, weight_t>
min_assignment(const IntMatrix &m)
{
    assert(m.row_size() == m.col_size());

    //认为小于4 用迭代比较快
    if (m.row_size() <= 4)
        return enumeration_method<weight_t>(m);
    else
        return hungarian_method<weight_t>(m);
}

}

#endif // ASSIGNMENTPROBLEM_H
