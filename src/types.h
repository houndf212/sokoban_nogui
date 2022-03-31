#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <list>
#include <set>
#include <iostream>

#include "basic_pos.h"
#include "basic_resize_matrix.h"
#include "elements.h"
#include "movedirection.h"
#include "global.h"

typedef short type_value;
typedef int type_size;

typedef Basic_Pos<type_size> Pos;

typedef Basic_Resize_Matrix<bool, type_size> BoolMatrix;
typedef Basic_Resize_Matrix<type_value, type_size> IntMatrix;
typedef Basic_Resize_Matrix<Elements, type_size> ElementsMatrix;

typedef std::vector<Pos> PosVector;
typedef std::list<Pos> PosList;

template<class T>
T Manhattan_Distance(const Basic_Pos<T> &p1, const Basic_Pos<T> &p2)
{
    return std::abs(p1.row()-p2.row())+std::abs(p1.col()-p2.col());
}

template <class T>
Basic_Pos<T> pos_move(const Basic_Pos<T> &from, Direction d)
{
    Basic_Pos<T> to = from;
    switch (d) {
    case Direction::up:
    case Direction::push_up:
        to.row()--;
        break;
    case Direction::down:
    case Direction::push_down:
        to.row()++;
        break;
    case Direction::left:
    case Direction::push_left:
        to.col()--;
        break;
    case Direction::right:
    case Direction::push_right:
        to.col()++;
        break;
    default:
        assert(false);
        break;
    }
    return to;
}

template <class T>
Direction pos_to(const Basic_Pos<T> &from, const Basic_Pos<T> &to)
{
    if (from.row() == to.row()) {
        auto dcol = to.col() - from.col();
        if (dcol == 1)
            return Direction::right;
        else if (dcol == -1)
            return Direction::left;
    }
    else if (from.col() == to.col()) {
        auto drow = to.row() - from.row();
        if (drow == 1)
            return Direction::down;
        else if (drow == -1)
            return Direction::up;
    }
    return Direction::NotValid;
}

#endif // TYPES_H
