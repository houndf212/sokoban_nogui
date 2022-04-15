#include "moves.h"
namespace moveNS
{
// 推动box
void apply_move(board &b, const move &m)
{
    clear_sokoban_inplace(b);

    Pos p;
    index_to_y_x(m.from, p); // box to push: initial position

    if ((b.at(p) & BOX) == 0) util::exit_with_error("missing box2");

    b.at(p) &= ~BOX;

    index_to_y_x(m.to, p); // box to push: final position

    if (b.at(p) & OCCUPIED) util::exit_with_error("occupied place");

    b.at(p) |= BOX;

    Pos sokoban = globals::getPosition(p, m.player_position);
    b.at(sokoban) |= SOKOBAN;

    expand_sokoban_cloud_one_sokoban(b, sokoban);
}

}
