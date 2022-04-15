#include "preprocess.h"
#include "globals.h"
#include "debug_print.h"
#include "board.h"
#include "deadlock.h"
#include "distance.h"

bool PreProcess::sanity_checks(const board &b)
{
    int box_num = boardUtil::boxes_in_level(b);

    if (box_num >= MAX_BOXES)
    {
        printf("%d boxes\n", box_num);
        util::exit_with_error("too many boxes\n");
        return false;
    }
    return true;
}

void PreProcess::close_holes_in_board(board &b)
{
    assert(b.row_size() >=2 && b.col_size()>=2);

    do
    {
        bool changed = false;
        for (int r=1; r<b.row_size()-1; ++r)
            for (int c=1; c<b.col_size()-1; ++c)
            {
                Pos p(r, c);
                if (b.get(p) != SPACE) continue;

                int sum = 0;
                for (auto e : globals::deltaPos)
                {
                    Pos p1 = globals::getPosition(p, e);
                    if (b.get(p1) == Elements::WALL)
                        sum++;
                }

                if (sum >= 3)
                {
                    b.set(p, Elements::WALL);
                    changed = true;
                }
            }

        if (!changed)
            return;

    }while(true);
}

void PreProcess::remove_boxes_out_of_inner(board &b)
{
    board inner;
    boardUtil::clear_boxes(b, inner);
    boardUtil::expand_sokoban_cloud(inner);

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
            if ((inner.get(i, j) & SOKOBAN) == 0)
            {
                auto e = inner.get(i, j);
                b.set(i, j, e & ~BOX & ~TARGET);
            }
}

void PreProcess::turn_decorative_boxes_to_walls(board &b)
{
    remove_boxes_out_of_inner(b);

    board c;
    boardUtil::clear_boxes(b, c);

    /*      @
     *      $
     *    @$.$@
     *      $
     *      @
     *  也就是说 target只能是从上下左右四个方向推进去，那么 sokoban 就是 上上下下左左右右位置推进去
     *  如果 这两个位置都被占用了，肯定是推不进去
    */

    // eliminate pullable boxes

    for (int i = 0; i < b.row_size(); i++)
        for (int j = 0; j < b.col_size(); j++)
            if (c.get(i, j) & TARGET)
                c.set(i, j, BOX);


    while (true)
    {
        bool changed = false;

        for (int i = 0; i < c.row_size(); i++)
            for (int j = 0; j < c.col_size(); j++)
            {
                if ((c.get(i, j) & BOX) == 0) continue;
                // 同理，这里可以优化，和下面一样，原理可以简单，将两个if合并
                // 并且 提前break
#if 0
                for (k = 0; k < 4; k++)
                {
                    if (c[i + delta_y[k]][j + delta_x[k]] & OCCUPIED) continue;
                    if (c[i + delta_y[k] * 2][j + delta_x[k] * 2] & OCCUPIED) continue;

                    c[i][j] &= ~BOX;
                    changed = 1;
                }
#endif
                Pos boxp(i, j);
                for (auto ep : globals::deltaPos)
                {
                    Pos p1 = globals::getPosition(boxp, ep);
                    Pos p2 = globals::getPosition(p1, ep);
                    if ((c.get(p1) & OCCUPIED) == 0 && (c.get(p2) & OCCUPIED) == 0)
                    {
                        c.set(i, j, c.get(i, j) & ~BOX);
                        changed = true;
                        break;
                    }
                }
            }

        if (!changed)
            break;
    }

    // 没有去掉的box 就是肯定不能推进去的
    for (int i = 0; i < c.row_size(); i++)
        for (int j = 0; j < c.col_size(); j++)
            if (c.get(i, j) & BOX)
            {
                b.set(i, j, WALL);
            }


    /*
     *      0
     *     3$1
     *      2
     *
     *  box位置如果相反两个方向都占用了，肯定是不能推动的，一个道理，这里感觉不用检查，以后检查fixed_box会检查吧？
    */

    // eliminate pushable boxes
    boardUtil::copy_board(b, c);

    while (true)
    {
        bool changed = false;

        for (int i = 0; i < c.row_size(); i++)
            for (int j = 0; j < c.col_size(); j++)
            {
                if ((c.get(i, j) & BOX) == 0) continue;
                //这里感觉不用 到4，因为两两相反，可以简化为，更容易理解
                /*
                 *  for (k=0; i<2; ++k)
                 *      if (!（c[i + delta_y[k]][j + delta_x[k]] & OCCUPIED) &&
                 *          !(c[i - delta_y[k]][j - delta_x[k]] & OCCUPIED）)
                 *          {
                 *          					c[i][j] &= ~BOX;
                 *                              changed = 1;
                 *                              break; // break 很重要，以为一个box只能反设置一次
                 *          }
                 *
                 *     意思就是 box 有一个对面是空的，就不会死，就可以推出来，
                */
#if 0
                for (k = 0; k < 4; k++)
                {
                    if (c[i + delta_y[k]][j + delta_x[k]] & OCCUPIED) continue;
                    if (c[i - delta_y[k]][j - delta_x[k]] & OCCUPIED) continue;

                    c[i][j] &= ~BOX;
                    changed = 1;
                }
#endif

                Pos boxp(i, j);
                Pos p1 = globals::getPosition(boxp, globals::ePosition::eUp);
                Pos p2 = globals::getPosition(boxp, globals::ePosition::eDown);

                if (!(c.get(p1) & OCCUPIED) && !(c.get(p2) & OCCUPIED))
                {
                    c.set(boxp, c.get(boxp) & ~BOX);
                    changed = true;
                    break;
                }

                p1 = globals::getPosition(boxp, globals::ePosition::eLeft);
                p2 = globals::getPosition(boxp, globals::ePosition::eRight);

                if (!(c.get(p1) & OCCUPIED) && !(c.get(p2) & OCCUPIED))
                {
                    c.set(boxp, c.get(boxp) & ~BOX);
                    changed = true;
                    break;
                }

            }

        if (!changed)
            break;
    }

    // 没有去掉的box 就是肯定不能推进去的
    for (int i = 0; i < c.row_size(); i++)
        for (int j = 0; j < c.col_size(); j++)
            if (c.get(i, j) & BOX)
            {
                b.set(i, j, WALL);
            }
}

static void getInnerMat(board &b)
{
    clear_boxes(initial_board, b);
    int height = initial_board.row_size();
    int width  = initial_board.col_size();

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
        {
            if (inner.at(i, j))
                b.at(i, j) = SPACE;
        }

}

static void getForBiddenTunnelMat(board &f)
{
    int height = initial_board.row_size();
    int width  = initial_board.col_size();
    clear_boxes(initial_board, f);
    clear_sokoban_inplace(f);
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
        {
            if (deadlock::forbidden_tunnel.at(i, j))
                f.at(i, j) = BOX;
        }
}

static void printAfterPreProcessInfo()
{
    int height = boardUtil::initial_board.row_size();
    int width  = boardUtil::initial_board.col_size();
    cout << "======PreProcess=======\n";
    cout << "init_board:\n";
    print_board(boardUtil::initial_board);
    cout << "box size: " << boardUtil::global_boxes_in_level <<endl;
    cout << "sokoban pos: ";
    print(boardUtil::global_initial_player);

    cout << "inner:\n";
    board in;
    getInnerMat(in);
    print(in);
    cout << "inner size: " << boardUtil::index_num << endl;
    cout << "=======================\n";

    cout << "forbidden:\n";
    board forb;
    getForBiddenTunnelMat(forb);
    print(forb);
    cout << "=======================\n";
}

bool PreProcess::preprocess_level(board &b)
{
    if (sanity_checks(b) == false)
        return false;

    turn_decorative_boxes_to_walls(b);
    close_holes_in_board(b);

    boardUtil::init_inner(b);
    boardUtil::init_index_x_y();

    boardUtil::keep_boxes_in_inner(b);
    boardUtil::save_initial_board(b);
    boardUtil::expand_sokoban_cloud(b);


    deadlock::set_forbidden_tunnel();

    set_distances(b);

#ifndef NDEBUG
    printAfterPreProcessInfo();
#endif

    return true;
}


