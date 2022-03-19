#ifndef HUNGARIANALG_H
#define HUNGARIANALG_H
#include "types.h"
#include <algorithm>
#include <bitset>

template<class T>
class HungarianAlg
{
private:
    enum class Step
    {
        Done,
        One,
        Two,
        Three,
        Four,
        Five,
    };
public:
    IntMatrix mask() const { return mask_matrix; }
    std::pair<PosVector, T> solve(const Basic_Resize_Matrix<T, type_size> &m)
    {
        assert(m.row_size() == m.col_size());
        const auto size = m.row_size();

        //复制矩阵用来操作
        matrix = m;

        mask_matrix = IntMatrix(matrix.row_size(), matrix.col_size(), NORMAL);

        //先设置全部未标记
        row_mask.resize(size);
        clear_mask(row_mask);

        col_mask.resize(size);
        clear_mask(col_mask);

        minimize_row(matrix);
        minimize_col(matrix);

        // Follow the steps
        Step step = Step::One;
        while ( step != Step::Done ) {
            switch ( step ) {
            case Step::One:
                step = step1();
                // step is always 2
                break;
            case Step::Two:
                step = step2();
                // step is always either 0 or 3
                break;
            case Step::Three:
                step = step3();
                // step in [3, 4, 5]
                break;
            case Step::Four:
                step = step4();
                // step is always 2
                break;
            case Step::Five:
                step = step5();
                // step is always 3
                break;
            case Step::Done:
            default:
                assert(false);
                break;
            }
        }
        return calc_min(m, mask_matrix);
    }
private:
    // helper func
    //计算最小值
    static std::pair<PosVector, T> calc_min(const Basic_Resize_Matrix<T, type_size> &m, const IntMatrix &mask)
    {
        PosVector vec;
        T sum = 0;

        for (auto it=mask.range(); it; ++it) {
            if (*it == STAR) {
                vec.push_back(it.pos());
                sum += m.get(it.pos());
            }
        }

        return std::make_pair(vec, sum);
    }
    //清空mask
    static void clear_mask(std::vector<bool> &mask)
    {
        std::fill(begin(mask), end(mask), false);
    }

    template<class R>
    static void minimize_range(const R &range)
    {
        auto min_val = *std::min_element(range.begin(), range.end());
        for (auto &v : range) {
            v -= min_val;
        }
    }

    //每行减去行最小值
    static void minimize_row(Basic_Resize_Matrix<T, type_size> &m)
    {
        assert(m.row_size()>=2 && m.col_size()>=2);
        for (auto row=m.szero(); row<m.row_size(); ++row)
            minimize_range(m.row_range(row));

    }
    //每列减去列最小值
    static void minimize_col(Basic_Resize_Matrix<T, type_size> &m)
    {
        assert(m.row_size()>=2 && m.col_size()>=2);
        for (auto col=m.szero(); col<m.col_size(); ++col)
            minimize_range(m.col_range(col));
    }
private:
    //找到未被覆盖的 0 ，即是 独立0 也就是 prime
    bool find_uncovered_zero_in_matrix(type_size &row, type_size &col) const
    {
        for (row=matrix.szero(); row<matrix.row_size(); ++row) {
            if (row_mask[row])
                continue;

            for (col=matrix.szero(); col<matrix.col_size(); ++col) {
                if (col_mask[col])
                    continue;

                if (matrix.get(Pos(row, col)) == 0)
                    return true;
            }
        }
        return false;
    }
    //找到此行 的 star 标记
    bool star_in_row(int row, int &col) const
    {
        auto range = mask_matrix.row_range(row);
        auto it = std::find(range.begin(), range.end(), T(STAR));
        if (it == range.end()) {
            return false;
        }
        else {
            col = it.pos().col();
            return true;
        }

    }

    //找到未被标记的 最小值
    T find_smallest_in_uncover() const
    {
        auto h = matrix.vmax();
        for (auto row=matrix.szero(); row<matrix.row_size(); ++row) {
            if (row_mask[row])
                continue;

            for (auto col=matrix.szero(); col<matrix.col_size(); ++col) {
                if (col_mask[col])
                    continue;
                assert(matrix.get(Pos(row, col))!=0);
                h = std::min(h, matrix.get(Pos(row, col)));
            }
        }
        assert(h!=matrix.vmax());
        return h;
    }

    //标记 star :每列只取一个 0 标记为 star (优先取行！)
    Step step1()
    {
        std::vector<bool> row_cover(matrix.row_size(), false);
        std::vector<bool> col_cover(matrix.col_size(), false);

        for (auto row=matrix.szero(); row<matrix.row_size(); ++row) {
            if (row_cover[row])
                continue;

            for (auto col=matrix.szero(); col<matrix.col_size(); ++col) {
                if (col_cover[col])
                    continue;

                Pos p(row, col);
                if (matrix.get(p) == 0) {
                    mask_matrix.set(p, STAR);
                    row_cover[row] = true;
                    col_cover[col] = true;
                    break;
                }
            }
        }

        return Step::Two;
    }

    //计算列标记条数(由于算法可以计算 mxn 匹配，所以列数大于行数，固这里要计算)
    Step step2()
    {
        int covercount = 0;

        for (auto it=mask_matrix.range(); it; ++it) {
            if (STAR == *it) {
                col_mask[it.col()] = true;
                ++covercount;
            }
        }

        //如果 超过 行数 那么已经是最优匹配
        if (covercount >= matrix.row_size()) {
            return Step::Done;
        }
        else {
            return Step::Three;
        }
    }

    Step step3()
    {
        //如果找到一个未被覆盖的0 那么这个0 一定是主元
        if (find_uncovered_zero_in_matrix(saverow, savecol)) {

            //设置为主元
            mask_matrix.set(Pos(saverow, savecol), PRIME);

            //这个算法以行为主，所以如果这一行有其他标星的0 那么 只需要标记这一行,并取消标记这一列
            //那么这个0未被标记的0 就一定会被标记,如此循环，那么 一定会出现一个 行无标星的0，那么跳到step4
            int col = -1;
            if (star_in_row(saverow, col)) {
                row_mask[saverow] = true;
                col_mask[col] = false;
                return Step::Three;
            }
            else {
                return Step::Four;
            }
        }
        //如果 没有未被覆盖的0 那么就创造一个 0
        else {
            return Step::Five;
        }
    }

    //交换 prime 和star
    //这一步完成， 会增加一个标星 0
    Step step4()
    {
        PosVector seq;

        auto pos_in_vec = [&seq](const Pos& p) {
          return std::find(begin(seq), end(seq), p) != end(seq);
        };

        Pos z0(saverow, savecol);
        seq.push_back(z0);

        Pos z1(-1, -1);
        Pos z2n(-1, -1);

        type_size row;
        type_size col = savecol;

        bool madepair;
        do{
            madepair = false;
            for (row=0; row<mask_matrix.row_size(); ++row) {
                Pos p(row, col);
                if (mask_matrix.get(p) == STAR) {
                    z1 = p;
                    if (pos_in_vec(p))
                        continue;

                    madepair = true;
                    seq.push_back(z1);
                    break;
                }
            }

            if (!madepair)
                break;

            madepair = false;

            for (col=0; col<mask_matrix.col_size(); ++col) {
                Pos p(row, col);
                if (mask_matrix.get(p) == PRIME) {
                    z2n = p;
                    if (pos_in_vec(p))
                        continue;

                    madepair = true;
                    seq.push_back(z2n);
                    break;
                }
            }

        }while(madepair);

        for (const Pos &p : seq) {
            if (mask_matrix.get(p) == STAR)
                mask_matrix.set(p, NORMAL);
            else if (mask_matrix.get(p) == PRIME)
                mask_matrix.set(p, STAR);
        }

        for (auto &v : mask_matrix) {
            if (v == PRIME)
                v = NORMAL;
        }

        clear_mask(row_mask);
        clear_mask(col_mask);
        return Step::Two;
    }

    //在未标记的列中制作出一个0
    Step step5()
    {
        auto h = find_smallest_in_uncover();

        for (auto row=matrix.szero(); row<matrix.row_size(); ++row) {
            if (row_mask[row]) {
                for (auto col=matrix.szero(); col<matrix.col_size(); ++col) {
                    Pos p(row, col);
                    matrix.set(p, matrix.get(p) + h);
                }
            }
        }

        for (auto col=matrix.szero(); col<matrix.col_size(); ++col) {
            if (!col_mask[col]) {
                for (auto row=matrix.szero(); row<matrix.row_size(); ++row) {
                    Pos p(row, col);
                    matrix.set(p, matrix.get(p) - h);
                }
            }
        }

        return Step::Three;
    }

private:
    //覆盖矩阵 记录 normal star prime
    IntMatrix mask_matrix;
    //记录操作矩阵
    Basic_Resize_Matrix<T, type_size> matrix;
    //标记的行
    std::vector<bool> row_mask;
    //标记的列
    std::vector<bool> col_mask;
    //记录 未被覆盖的 0 即主元  prime
    type_size saverow;
    type_size savecol;

private:
    //一般元素
    static constexpr T NORMAL = 0;
    //分配元素 标星
    static constexpr T STAR   = 1;
    //主元
    static constexpr T PRIME  = 2;
};

#endif // HUNGARIANALG_H
