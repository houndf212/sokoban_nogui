#include "boardparam.h"
#include "roomslice.h"
#include <algorithm>

void BoardParam::set_matrix(const ElementsMatrix &m)
{
    m_goals.reset(new PosVector);
    PosVector* _m_goals = const_cast<PosVector*>(m_goals.get());

    m_empty_room.reset(new ElementsMatrix(m));
    ElementsMatrix* _m_empty_room = const_cast<ElementsMatrix*>(m_empty_room.get());
    box_index.clear();

    for (auto it=m.range(); it; ++it) {
        Pos p = it.pos();
        auto e = *it;
        switch (e) {
        case Elements::man:
            man_pos = p;
            _m_empty_room->set(p, Elements::floor);
            break;
        case Elements::box:
            box_index.push_back(p);
            _m_empty_room->set(p, Elements::floor);
            break;
        case Elements::goal:
            _m_goals->push_back(p);
            _m_empty_room->set(p, Elements::floor);
            break;
        case Elements::man_goal:
            man_pos = p;
            _m_goals->push_back(p);
            _m_empty_room->set(p, Elements::floor);
            break;
        case Elements::box_goal:
            box_index.push_back(p);
            _m_goals->push_back(p);
            _m_empty_room->set(p, Elements::floor);
            break;
        default:
            break;
        }
    }
    _m_goals->shrink_to_fit();
    box_index.shrink_to_fit();
}

bool BoardParam::man_move(Direction &d)
{
    assert(d!=Direction::NotValid);

    Pos to = pos_move(man(), d);

    if (!empty_room()->isInMatrix(to))
        return false;

    if (is_wall(to)) {
        return false;
    }

    if (is_box(to)) {
        Pos too = pos_move(to, d);
        if (!empty_room()->isInMatrix(too))
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
    auto em = *empty_room();
    for (auto p : boxes()) {
        em.set(p, Elements::box);
    }
    return em;
}



void BoardParam::box_move(Pos box, Pos to)
{
    assert(pos_to(box, to) != Direction::NotValid);
    assert(empty_room()->isInMatrix(box));
    assert(empty_room()->isInMatrix(to));
    assert(is_box(box));
    assert(empty_room()->get(box)!=Elements::wall);
    assert(!is_box(to));
    assert(box == Pos((man().row()+to.row())/2, (man().col()+to.col())/2));

    man_pos = box;
    auto it = std::find(begin(box_index), end(box_index), box);
    assert(it != end(box_index));
    *it = to;
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
                nexts.push_back(pa);
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
    assert(empty_room()->isInMatrix(box));
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
    assert(goals() == param.goals());
    assert(empty_room() == param.empty_room());
    return man() == param.man() && is_same_boxes(param);
}

bool BoardParam::like_equal(const BoardParam &param) const
{
    assert(goals() == param.goals());
    assert(empty_room() == param.empty_room());
    return is_same_boxes(param) && RoomSlice(*this).can_man_to(param.man());
}

bool BoardParam::is_done() const
{
    auto this_is_box = [this](const Pos &p) { return is_box(p); };
    return std::all_of(goals()->begin(), goals()->end(), this_is_box);
}


bool BoardParam::is_goal(Pos p) const
{
    return std::find(goals()->begin(), goals()->end(), p) != goals()->end();
}

bool BoardParam::is_box(Pos p) const
{
    return std::find(box_index.begin(), box_index.end(), p) != box_index.end();
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
    return false;
}

bool BoardParam::is_absolutely_dead() const
{
    auto dead_box = [this](const Pos &p) { return !is_goal(p) && is_absolutely_dead_box(p); };
    return std::any_of(begin(boxes()), end(boxes()), dead_box);
}

ElementsMatrix BoardParam::to_matrix() const
{
    ElementsMatrix m = *empty_room();

    m.set(man(), Elements::man);

    for (auto p : boxes()) {
        m.set(p, Elements::box);
    }

    for (auto p : *goals()) {
        auto e = m.get(p);
        m.set(p, add_goal(e));
    }
    return m;
}

BoardParam BoardParam::to_goal() const
{
    BoardParam pa = *this;
    pa.box_index = *pa.goals();
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
    assert(p1.goals() == p2.goals());
    assert(p1.empty_room() == p2.empty_room());

    // 这里需要特殊处理完成的状态，因为完成时人在哪里也不知道，所以特殊处理
    if (p1.is_done() && p2.is_done())
        return true;

    return p1.like_equal(p2);
}

//推箱子唯一不同的是 man的位置 和 boxex的位置，
//而man的位置不是关键之处
//所以可以大幅度提高hash的计算速度
#include <type_traits>
size_t BoardHash::operator()(const BoardParam &param) const
{
    PosVector boxes = param.boxes();
    std::sort(boxes.begin(), boxes.end());
    static_assert(std::is_trivial<PosVector::value_type>(), "cannot use normal hash!");
    int mem_size = boxes.size()*sizeof(PosVector::value_type);
    assert(!boxes.empty());
    assert(reinterpret_cast<const char*>(&boxes[boxes.size()])
            == reinterpret_cast<const char*>(&boxes[0])+mem_size);
    std::string str(reinterpret_cast<const char*>(&boxes[0]), mem_size);
    return std::hash<std::string>()(str);
}
