#include "boardparam.h"
#include "roomslice.h"
#include "xsb.h"
#include "debug_print.h"

void BoardParam::set_matrix(const ElementsMatrix &m)
{
    globals::setOriginBoard(m);
    box_index.clear();

    for (auto it=m.range(); it; ++it) {
        Pos p = it.pos();
        auto e = *it;
        switch (e) {
        case (Elements::BOX | Elements::TARGET):
        case Elements::BOX:
            box_index.push_back(p);
            break;
        case Elements::SOKOBAN:
            man_pos = p;
            break;
        default:
            break;
        }
    }
    box_index.shrink_to_fit();
    sort_boxes_index();
    boxMat.resize(m.row_size(), m.col_size(), false);
    for (auto p : box_index)
        boxMat.set(p, true);
}

bool BoardParam::man_move(Direction &d)
{
    assert(d!=Direction::NotValid);

    Pos to = pos_move(man(), d);

    if (!globals::empty_room()->isInMatrix(to))
        return false;

    if (is_wall(to)) {
        return false;
    }

    if (is_box(to)) {
        Pos too = pos_move(to, d);
        if (!globals::empty_room()->isInMatrix(too))
            return false;

        if (!is_wall(too)) {
            box_move(to, too);
            d = add_push(d);
            return true;
        }
        return false;
    } else {
        assert(!is_wall(to));
        man_pos = to;
        assert(!is_push(d));
        return true;
    }
}

ElementsMatrix BoardParam::cache_room() const
{
    auto em = *globals::empty_room();
    for (auto p : boxes()) {
        em.set(p, Elements::BOX);
    }
    return em;
}



void BoardParam::box_move(Pos box, Pos to)
{
    assert(pos_to(box, to) != Direction::NotValid);
    assert(globals::empty_room()->isInMatrix(box));
    assert(globals::empty_room()->isInMatrix(to));
    assert(is_box(box));
    assert(globals::empty_room()->get(box)!=Elements::wall);
    assert(!is_box(to));
    assert(box == Pos((man().row()+to.row())/2, (man().col()+to.col())/2));

    man_pos = box;
    auto it = std::find(begin(box_index), end(box_index), box);
    assert(it != end(box_index));
    *it = to;
    boxMat.set(box, false);
    boxMat.set(to , true);
}

std::list<BoardParam> BoardParam::next_move() const
{
    std::list<BoardParam> nexts;

    auto func = [&](Pos box, Direction d) {
        if (can_box_move(box, d)) {
            Pos man_to = pos_move(box, reverse(d));
            Pos to = pos_move(box, d);
            BoardParam pa = *this;
            //如果需要计算移动步骤，这里人是跳跃移动的
            pa.man_pos = man_to;
            pa.box_move(box, to);
            if (pa.is_goal(to) || !pa.is_absolutely_dead_box(to))
            {
                // make sure boxes index is sorted
                pa.sort_boxes_index();
                nexts.push_back(pa);
            }
        }
    };

    for (auto p : boxes()) {
        func(p, Direction::up);
        func(p, Direction::down);
        func(p, Direction::left);
        func(p, Direction::right);
    }
    return nexts;
}

bool BoardParam::can_box_move(Pos box, Direction d) const
{
    assert(globals::empty_room()->isInMatrix(box));
    assert(is_box(box));

    Pos to = pos_move(box, d);
    if (!is_empty(to))
        return false;

    Pos man_to = pos_move(box, reverse(d));
    if (!is_empty(man_to))
        return false;

    RoomSlice s(*this);
    return s.can_man_to(man_to);
}

bool BoardParam::precise_equal(const BoardParam &param) const
{
    //assert(goals() == param.goals());
    return man() == param.man() && is_same_boxes(param);
}

bool BoardParam::like_equal(const BoardParam &param) const
{
    //assert(goals() == param.goals());
    return is_same_boxes(param) && RoomSlice(*this).can_man_to(param.man());
}

bool BoardParam::is_done() const
{
    auto this_is_box = [this](const Pos &p) { return is_box(p); };
#if 0
    return std::all_of(goals()->begin(), goals()->end(), this_is_box);
#else
    const auto &goals = globals::getOriginGoals();
    return std::all_of(goals.begin(), goals.end(), this_is_box);
#endif
}


bool BoardParam::is_goal(Pos p) const
{
#if 0
    return std::find(goals()->begin(), goals()->end(), p) != goals()->end();
#else
    return globals::getOriginBoard().get(p) == Elements::TARGET;
#endif
}

bool BoardParam::is_box(Pos p) const
{
#if 0
    return std::find(box_index.begin(), box_index.end(), p) != box_index.end();
#else
    return boxMat.get(p);
#endif
}



bool BoardParam::is_same_boxes(const BoardParam &param) const
{
    auto this_is_box = [this](const Pos &p) { return is_box(p); };
    return std::all_of(param.boxes().begin(), param.boxes().end(), this_is_box);
}

bool BoardParam::is_absolutely_dead_box(Pos box) const
{
    // 左上右下  相邻 两个方向都是 墙 那么一定死了
    assert(is_box(box));
    //假定 board 周围必须有一堵墙卡住
    Pos left(box.row(), box.col()-1);
    Pos up(box.row()-1, box.col());
    Pos right(box.row(), box.col()+1);
    Pos down(box.row()+1, box.col());

    if ((is_wall(left) || is_wall(right))
            && (is_wall(up) || is_wall(down))) {
        return true;

    }
    // 如果推动一个box，给box所在位置造成了 2x2那么也是死了
    /*
     *    $$                ???
     *  @$ #  类似的         ?$?   12
     *                      ???   34
    */
#if 1
    Pos start(box.row()-1, box.col()-1);
    for (Pos::value_type r=0; r<=1; ++r)
        for (Pos::value_type c=0; c<=1; ++c)
        {
            Pos pos1(start.row()+r, start.col()+c);
            Pos pos2(pos1.row(), pos1.col()+1);
            Pos pos3(pos1.row()+1, pos1.col());
            Pos pos4(pos1.row()+1, pos1.col()+1);
            if (is_occupied(pos1) && is_occupied(pos2) && is_occupied(pos3) && is_occupied(pos4))
            {
                //std::cout << "find deadlock\n";
                return true;
            }
        }
#endif

    return false;
}

bool BoardParam::is_absolutely_dead() const
{
    auto dead_box = [this](const Pos &p) { return !is_goal(p) && is_absolutely_dead_box(p); };
    return std::any_of(begin(boxes()), end(boxes()), dead_box);
}

ElementsMatrix BoardParam::to_matrix() const
{
    ElementsMatrix m = *globals::empty_room();

    m.set(man(), Elements::SOKOBAN);

    for (auto p : boxes()) {
        m.set(p, Elements::BOX);
    }

    for (auto p : globals::getOriginGoals()) {
        auto e = m.get(p);
        m.set(p, e | TARGET);
    }
    return m;
}

BoardParam BoardParam::to_goal() const
{
    BoardParam pa = *this;
    pa.box_index = globals::getOriginGoals();
    for (auto p : pa.boxes())
        pa.boxMat.set(p, true);
    assert(pa.is_done());
    return pa;
}

bool BoardParam::can_solve(const MoveList &mlst) const
{
    BoardParam param = *this;

    for (auto d : mlst) {
        if (!param.man_move(d))
            return false;
    }
    return param.is_done();
}

bool operator ==(const BoardParam &p1, const BoardParam &p2)
{
    //assert(p1.goals() == p2.goals());

    // 这里需要特殊处理完成的状态，因为完成时人在哪里也不知道，所以特殊处理
    if (p1.is_done() && p2.is_done())
        return true;

    return p1.like_equal(p2);
}

inline size_t
gnu_hash_str(const void *p, size_t n)
{
    return std::_Hash_impl::hash(p, n);
}
//推箱子唯一不同的是 man的位置 和 boxex的位置，
//而man的位置不是关键之处
//所以可以大幅度提高hash的计算速度
#include <type_traits>
size_t BoardHash::operator()(const BoardParam &param) const
{
    const PosVector &boxes = param.boxes();
    assert(std::is_sorted(boxes.begin(), boxes.end()));
    static_assert(std::is_trivial<PosVector::value_type>(), "cannot use normal hash!");
    int mem_size = boxes.size()*sizeof(PosVector::value_type);
    assert(!boxes.empty());
    assert(reinterpret_cast<const char*>(&boxes[boxes.size()])
            == reinterpret_cast<const char*>(&boxes[0])+mem_size);
#if 0
    std::string str(reinterpret_cast<const char*>(&boxes[0]), mem_size);
    return std::hash<std::string>()(str);
#else
    return gnu_hash_str(&boxes[0], mem_size);
#endif
}
