#include "board.h"
#include "globals.h"

int boardUtil::global_boxes_in_level = 0;
Pos boardUtil::global_initial_player(0, 0);
board boardUtil::initial_board;
BoolMatrix boardUtil::inner;
int boardUtil::index_num = 0;

//记录inner board index 和  xy的转换
static int y_x_to_index_table[MAX_SIZE][MAX_SIZE];
static Pos index_to_x_y[MAX_INNER];

int boardUtil::boxes_in_level(const board &b)
{
    int sum = 0;

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Elements e = b.get(Pos(i, j));
            if (e & Elements::box)
                sum++;
        }
    return sum;
}

void boardUtil::clear_boxes(const board &b, board &board_without_boxes)
{
    copy_board(b, board_without_boxes);

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Pos p(i, j);
            Elements e = board_without_boxes.get(p);
            board_without_boxes.set(p, Elements(e & ~Elements::box));
        }
}

void boardUtil::expand_sokoban_cloud(board &b)
{
    Pos que[MAX_SIZE*MAX_SIZE];
    int queue_len = 0;
    int queue_pos = 0;

    // init queue from existing player positions
    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            Pos p(i, j);
            if (b.get(p) & SOKOBAN)
            {
                que[queue_len] = p;
                queue_len++;
            }
        }

    while (queue_pos < queue_len)
    {
        Pos p = que[queue_pos];
        for (auto e : globals::deltaPos)
        {
            Pos next = globals::getPosition(p, e);
            auto ne = b.get(next);
            if (ne & SOKOBAN) continue;
            if (ne & OCCUPIED) continue;

            b.set(next, Elements(ne | SOKOBAN));

            que[queue_len] = next;
            queue_len++;
        }
        queue_pos++;
    }
}

void boardUtil::init_inner(const board &b)
{
    board c;
    int i, j;

    clear_boxes(b, c);
    expand_sokoban_cloud(c);

    // set inner
    BoolMatrix &in = const_cast<BoolMatrix&>(inner);
    in.resize(b.row_size(), b.col_size());
    for (i = 0; i < b.row_size(); i++)
        for (j = 0; j < b.col_size(); j++)
        {
            in.set(i, j, false);
            if (c.get(i, j) & SOKOBAN)
                in.set(i, j, true);
        }
}

void boardUtil::init_index_x_y()
{
    int num = 0;
    for (int i = 0; i < inner.row_size(); i++)
    {
        for (int j = 0; j < inner.col_size(); j++)
        {
            y_x_to_index_table[i][j] = -1;

            if (inner.get(i, j) == false)
                continue;

            y_x_to_index_table[i][j] = num;
            index_to_x_y[num] = Pos(i, j);
            num++;

            if (num >= MAX_INNER)
                util::exit_with_error("inner too big");

        }
    }
    index_num = num;
}

int boardUtil::y_x_to_index(const Pos &p)
{
    return y_x_to_index_table[p.row()][p.col()];
}

void boardUtil::index_to_y_x(int index, Pos &p)
{
    p = index_to_x_y[index];
}

void boardUtil::keep_boxes_in_inner(board &b)
{
    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            if (inner.get(i, j) == false)
                if (b.get(i, j) & BOX)
                    b.set(i, j, WALL);
        }
}

void boardUtil::save_initial_board(const board &b)
{
    board &init = const_cast<board&>(initial_board);
    copy_board(b, init);

    Pos &play = const_cast<Pos &>(global_initial_player);
    get_sokoban_position(b, play);
    const_cast<int&>(global_boxes_in_level) = boxes_in_level(b);
}

void boardUtil::get_sokoban_position(const board b, Pos &p)
{
    for (int i = 0; i < b.row_size() ; i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            if (b.get(i, j) & SOKOBAN)
            {
                p.row() = i;
                p.col() = j;
                return;
            }
        }

    print_board(b);
    util::exit_with_error("Sokoban not found\n");
}
