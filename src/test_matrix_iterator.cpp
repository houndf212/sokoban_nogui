#include "types.h"
#include "debug_print.h"

void test_set(IntMatrix &mat)
{
    int n=1;
    for (auto it=mat.range(); it; ++it) {
        *it = n++;
    }
}

void test_get(const IntMatrix &mat)
{
    for (auto it=mat.range(); it; ++it) {
        cout << *it;
    }
}

void test_for_set(IntMatrix &mat)
{
    int n=10;
    for (auto& v : mat) {
        v = n;
        n+=10;
    }
}

void test_for_get(const IntMatrix &mat)
{
    for (auto& v : mat) {
        cout << v <<endl;
    }
}

void test_row_set(IntMatrix &mat)
{
    int n=100;
    for (auto it=mat.row_range(0); it; ++it) {
        *it = n;
        n+=100;
    }
}

void test_row_get(const IntMatrix &mat)
{
    for (auto it=mat.row_range(0); it; ++it) {
        cout << *it <<endl;
    }
}

void test_col_set(IntMatrix &mat)
{
    int n=1000;
    for (auto it=mat.col_range(1); it; ++it) {
        *it = n;
        n+=1000;
    }
}

void test_col_get(const IntMatrix &mat)
{
    for (auto it=mat.col_range(1); it; ++it) {
        cout << *it;
    }
}

void test_new_range_set(IntMatrix &mat)
{
    int i=-1;
    for (auto range=mat.range(); range; range.next()) {
        *range = i--;
    }
}

void test_new_range_get(const IntMatrix &mat)
{
    for (auto range=mat.col_range(1); range; range.next()) {
        cout << *range <<endl;
    }
}

void test_for_range_set(IntMatrix &mat)
{
    int i=-10;
    for (auto &v : mat.range()) {
        v = i;
        i -= 10;
    }
}

void test_for_range_get(const IntMatrix &mat)
{
    for (auto &v : mat.row_range(1)) {
        cout << v <<endl;
    }
}

void test_matrix_iterator()
{
    IntMatrix mat(3, 3);

//    test_set(mat);
//    print(mat);
//    test_get(mat);

//    test_for_set(mat);
//    print(mat);
//    test_for_get(mat);

//    test_row_set(mat);
//    print(mat);
//    test_row_get(mat);

//    test_col_set(mat);
//    print(mat);
//    test_col_get(mat);

//    test_new_range_set(mat);
//    print(mat);
//    test_new_range_get(mat);

    test_for_range_set(mat);
    print(mat);
    test_for_range_get(mat);

}
