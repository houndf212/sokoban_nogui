/*
 *   Copyright (c) 2007 John Weaver
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/*
 * Some example code.
 *
 */

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "munkres.h"
#include "hungarianalg.h"
#include "debug_print.h"

IntMatrix from(const Matrix<int> &m)
{
    IntMatrix matrix(m.rows(), m.columns());

    for (size_t row=0; row<m.rows(); ++row) {
        for (size_t col=0; col<m.columns(); ++col) {
            matrix.set(Pos(row, col), m(row, col));
        }
    }
    return matrix;
}

IntMatrix munkres(Matrix<int> matrix)
{
    // Apply Munkres algorithm to matrix.
    Munkres<int> m;
    m.solve(matrix);
    IntMatrix ret = from(m.mask_matrix);
    return ret;
}

IntMatrix hunalg(const IntMatrix &m)
{
    typedef HungarianAlg<IntMatrix::value_type> Hun;
    Hun hun;
    hun.solve(m);
    return hun.mask();
}

int check_min(const IntMatrix& m, const IntMatrix &mask)
{
    int sum=0;
    for (auto it=mask.range(); it; ++it) {
        if (*it == 1)
            sum += m.get(it.pos());
    }

    return sum;
}

void test_hunalg_and_munkres()
{
    int nrows = 10;
    int ncols = 10;

    Matrix<int> matrix(nrows, ncols);


    // Initialize matrix with random values.
    for ( int row = 0 ; row < nrows ; row++ ) {
        for ( int col = 0 ; col < ncols ; col++ ) {
            int val = std::rand() % 100;
            matrix(row,col) = val;
        }
    }

    IntMatrix mat = from(matrix);

    auto ret1 = munkres(matrix);
    auto ret2 = hunalg(mat);
    if (check_min(mat, ret1)!=check_min(mat, ret2)) {
        print(mat);
        print(ret1);
        print(ret2);
        assert(false);
    }
}

void test_munkres()
{
    std::srand(time(nullptr)); // Seed random number generator.
    int k=10000;
    while(k-->0) {
        test_hunalg_and_munkres();
    }
}
