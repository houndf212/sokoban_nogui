#ifndef BOARDPARAM_H
#define BOARDPARAM_H
#include <memory>
#include "types.h"
#include "movelist.h"


//这个类描述一个推箱子所有需要的元素，在boardapi操作这个类

class BoardParam
{
public:
    //初始化函数
    void set_matrix(const ElementsMatrix &m);

    //唯一操作推箱子接口
    bool man_move(Direction &d);

    //内部数据read  readonly
    const Pos &man() const { return man_pos; }

    size_t goals_size() const
    {
        assert(m_goals!=nullptr);
        return m_goals->size();
    }
    const PosVector *goals() const
    {
        return m_goals.get();
    }
    const PosVector &boxes() const { return box_index; }

    //只包含 wall floor box, 即是说不包含goals和人
    ElementsMatrix cache_room() const;

    const ElementsMatrix *empty_room() const
    {
        assert(m_empty_room!=nullptr);
        return m_empty_room.get();
    }

    //空的即是说，不是墙，也不是box
    bool is_empty(Pos p) const
    {
        return !is_wall(p) && !is_box(p);
    }

    //测试函数
    bool is_done() const;
    bool is_wall(Pos p) const
    {
        assert(empty_room()!=nullptr);
        return empty_room()->get(p) == Elements::wall;
    }
    bool is_goal(Pos p) const;
    bool is_box(Pos p) const;
    bool is_same_boxes(const BoardParam &param) const;
    bool is_absolutely_dead_box(Pos box) const;
    bool is_absolutely_dead() const;
    bool can_solve(const MoveList &mlst) const;

    //用于自动推箱子
    std::list<BoardParam> next_move() const;

    //转化函数
    ElementsMatrix to_matrix() const;
    BoardParam to_goal() const;
private:
    void box_move(Pos box, Pos to);
    bool can_box_move(Pos box, Direction d) const;
public:
    bool precise_equal(const BoardParam &param) const;
    bool like_equal(const BoardParam &param) const;
private:
    //人所在的位置
    Pos man_pos;
    // box 的索引
    PosVector box_index;
    //目标位置, set之后就不会变了
    std::shared_ptr<const PosVector> m_goals;
    //只包含 wall floor的"空"房子,和goals一样，set之后就不会变了
    std::shared_ptr<const ElementsMatrix> m_empty_room;
};

bool operator == (const BoardParam &p1, const BoardParam &p2);

struct BoardHash
{
    size_t operator()(const BoardParam &param) const;
};

#endif // BOARDPARAM_H
