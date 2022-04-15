#include "board.h"
#include "globals.h"

int boardUtil::global_boxes_in_level = 0;
Pos boardUtil::global_initial_player(0, 0);
board boardUtil::initial_board;
FixedMatix<bool, MAX_SIZE, MAX_SIZE> boardUtil::inner;
int boardUtil::index_num = 0;

//记录inner board index 和  xy的转换
FixedMatix<int, MAX_SIZE, MAX_SIZE>  indexTable;
FixedVector<Pos, MAX_INNER>  innerPosVec;

int boardUtil::boxes_in_level(const board &b)
{
    int sum = 0;

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            auto e = b.get(i, j);
            if (e & Elements::BOX)
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
            board_without_boxes.at(i, j) &= ~Elements::BOX;
        }
}

static inline void
getSokobanAllPos(const board &b, FixedVector<Pos, MAX_INNER> &que)
{
    // init queue from existing player positions
    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            if (b.get(i, j) & SOKOBAN)
            {
                que.append(i, j);
            }
        }
}

static inline void
expand_sokoban(board &b, FixedVector<Pos, MAX_INNER> &que)
{
    int queue_pos = 0;
    while (queue_pos < que.size())
    {
        Pos p = que.at(queue_pos);
        for (auto e : globals::deltaPos)
        {
            Pos next = globals::getPosition(p, e);
            auto ne = b.get(next);
            if (ne & SOKOBAN) continue;
            if (ne & OCCUPIED) continue;

            b.at(next) |= SOKOBAN;
            que.append(next);
        }
        queue_pos++;
    }
}


void boardUtil::expand_sokoban_cloud(board &b)
{
    FixedVector<Pos, MAX_INNER> que;
    getSokobanAllPos(b, que);
    expand_sokoban(b, que);
}
void boardUtil::expand_sokoban_cloud_one_sokoban(board &b, Pos p)
{
    assert(b.at(p) & SOKOBAN);
    FixedVector<Pos, MAX_INNER> que;
    que.append(p);
    expand_sokoban(b, que);
}

void boardUtil::init_inner(const board &b)
{
    board c;
    int i, j;

    clear_boxes(b, c);
    expand_sokoban_cloud(c);

    // set inner

    for (i = 0; i < b.row_size(); i++)
        for (j = 0; j < b.col_size(); j++)
        {
            inner.at(i, j) = false;
            if (c.get(i, j) & SOKOBAN)
                inner.at(i, j) = true;
        }
}

void boardUtil::init_index_x_y()
{
    int height = initial_board.row_size();
    int width = initial_board.col_size();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            indexTable.at(i, j) = -1;

            if (inner.at(i, j) == false)
                continue;

            indexTable.at(i,j) = innerPosVec.size();
            innerPosVec.append(i, j);

            if (innerPosVec.size() >= MAX_INNER)
                util::exit_with_error("inner too big");

        }
    }
    index_num = innerPosVec.size();
}

int boardUtil::y_x_to_index(const Pos &p)
{
    return indexTable.at(p);
}

void boardUtil::index_to_y_x(int index, Pos &p)
{
    p = innerPosVec.at(index);
}

void boardUtil::keep_boxes_in_inner(board &b)
{
    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
        {
            if (inner.at(i, j) == false)
                if (b.get(i, j) & BOX)
                    b.at(i, j) = WALL;
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

void boardUtil::zero_board(board &b)
{
    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
            b.at(i, j) = SPACE;
}

void boardUtil::clear_sokoban_inplace(board &b)
{
    int height = b.row_size();
    int width = b.col_size();
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            b.at(i,j) &= ~SOKOBAN;
}

bool boardUtil::board_contains_boxes(const board &b)
{
    int height = initial_board.row_size();
    int width  = initial_board.col_size();
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (b.at(i, j) & BOX)
                return true;

    return false;
}
