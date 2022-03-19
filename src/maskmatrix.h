#ifndef MASKMATRIX_H
#define MASKMATRIX_H
#include "types.h"

class MaskMatrix : public BoolMatrix
{
public:
    MaskMatrix(size_type row, size_type col) : BoolMatrix(row, col, k_unmask) {}
//    void unmask_all() { fill(k_unmask); }
    void mask(Pos p) { set(p, k_mask); }
    bool is_mask(Pos p) const { return get(p) == k_mask; }
private:
    static constexpr bool k_mask = true;
    static constexpr bool k_unmask = false;
};

#endif // MASKMATRIX_H
