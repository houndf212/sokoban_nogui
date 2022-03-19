#ifndef MOVEDIRECTION_H
#define MOVEDIRECTION_H
#include <assert.h>

typedef unsigned char Direction_t;
enum class Direction : Direction_t
{
    NotValid,
    up = 0x1 | 0x1 <<1, // 0011
    down = up<<2, // 1100
    left = 0x1 | 0x1<<2, // 0101
    right = left<<1, //1010

    push = 0x1 << 4,
    push_up = push | up,
    push_down = push | down,
    push_left = push | left,
    push_right = push | right,
};

template<Direction d>
constexpr Direction_t toInt()
{
    return static_cast<Direction_t>(d);
}

inline bool is_reverse(Direction d1, Direction d2)
{
    static_assert((toInt<Direction::up>()&toInt<Direction::down>()) == 0, "");
    static_assert((toInt<Direction::left>()&toInt<Direction::right>()) == 0, "");

    static_assert((toInt<Direction::up>()&toInt<Direction::right>()) != 0, "");
    static_assert((toInt<Direction::up>()&toInt<Direction::left>()) != 0, "");

    static_assert((toInt<Direction::down>()&toInt<Direction::right>()) != 0, "");
    static_assert((toInt<Direction::down>()&toInt<Direction::left>()) != 0, "");
    extern bool is_push(Direction);
    return !is_push(d1) && !is_push(d2) && (Direction_t(d1)&Direction_t(d2)) == 0;
}

inline Direction reverse(Direction d)
{
    assert(d!=Direction::NotValid);
    constexpr Direction_t full = toInt<Direction::up>() | toInt<Direction::down>();

    static_assert((full^toInt<Direction::up>()) == toInt<Direction::down>(), "");
    static_assert((full^toInt<Direction::down>()) == toInt<Direction::up>(), "");
    static_assert((full^toInt<Direction::right>()) == toInt<Direction::left>(), "");
    static_assert((full^toInt<Direction::left>()) == toInt<Direction::right>(), "");

    return Direction(Direction_t(d)^full);
}

inline bool is_push(Direction d)
{
    static_assert((toInt<Direction::push_up>() & toInt<Direction::push>()) == toInt<Direction::push>(),"");
    static_assert((toInt<Direction::push_down>() & toInt<Direction::push>()) == toInt<Direction::push>(),"");
    static_assert((toInt<Direction::push_right>() & toInt<Direction::push>()) == toInt<Direction::push>(),"");
    static_assert((toInt<Direction::push_left>() & toInt<Direction::push>()) == toInt<Direction::push>(),"");

    return (Direction_t(d) & toInt<Direction::push>()) == toInt<Direction::push>();
}

inline Direction add_push(Direction d)
{
    static_assert((toInt<Direction::up>() | toInt<Direction::push>()) == toInt<Direction::push_up>(),"");
    static_assert((toInt<Direction::down>() | toInt<Direction::push>()) == toInt<Direction::push_down>(),"");
    static_assert((toInt<Direction::right>() | toInt<Direction::push>()) == toInt<Direction::push_right>(),"");
    static_assert((toInt<Direction::left>() | toInt<Direction::push>()) == toInt<Direction::push_left>(),"");

    assert(d != Direction::NotValid);
    return Direction(Direction_t(d) | toInt<Direction::push>());
}
#endif // MOVEDIRECTION_H
