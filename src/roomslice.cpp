#include "roomslice.h"

RoomSlice::RoomSlice(const BoardParam &pa)
    :param(pa)
{
    slice.resize(param.empty_room()->row_size(), param.empty_room()->col_size(), k_unflagged);

    for (auto p : param.boxes()) {
        slice.set(p, k_block);
    }

    IntMatrix::value_type g = 1;
    for (auto it=param.empty_room()->range(); it; ++it) {
        Pos p = it.pos();
        if (slice.get(p) != k_unflagged)
            continue;

        seedPos(p, g);
        ++g;
    }
}

bool RoomSlice::can_man_to(Pos p) const
{
    assert(slice.isInMatrix(p));
    return slice.get(p) == slice.get(param.man());
}

void RoomSlice::seedPos(Pos p, IntMatrix::value_type val)
{
    if (slice.isInMatrix(p)
            && slice.get(p)==k_unflagged
            && param.is_empty(p)) {

        slice.set(p, val);
        seedPos(Pos(p.row()+1, p.col()), val);
        seedPos(Pos(p.row()-1, p.col()), val);
        seedPos(Pos(p.row(), p.col()+1), val);
        seedPos(Pos(p.row(), p.col()-1), val);
    }
}

