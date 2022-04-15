#include "graph.h"
#include <string.h>
#include "bfs.h"

int &vertices_num = boardUtil::index_num;

struct vertex
{
    vertex* shift[4]; //对应box上下左右后的下一个vertex，也就是交换sokoban的位置
    vertex* push; // 对应vertex中push 后的另外一个vertex，push的方向是 vertex每4组中的第i个， push 方向 = push % 4
    int weight;
    bool active; // active 对应是否有效，对应box四个方向sokoban是否可以站过去
    vertex* src;

    int id_index;
    int id_pos;
};

using globals::deltaPosSize;
static_assert (deltaPosSize == 4, "" );
// 和下面注释一致，对应 box index 和 sokoban可以在的四个方向
FixedMatix<vertex, MAX_INNER, deltaPosSize> vertices;


void init_index()
{
    for (int i=0; i<MAX_INNER; ++i)
        for (int j=0; j<4; ++j)
        {
            vertex &ver = vertices.at(i, j);
            ver.id_index = i;
            ver.id_pos = j;
        }
}

// 初始化 active 字段，去除很多wall位置
void init_active(const board &b)
{
    // "active" tests if the player can be positioned in this side of the box

    for (int i = 0; i < index_num; i++)
        for (int j = 0; j < 4; j++)
            vertices.at(i, j).active = false;

    for (int i = 0; i < index_num; i++)
    {
        Pos p;
        index_to_y_x(i, p);

        if (b.at(p) & WALL)
        {
            // this can happen when boxes are converted to walls
            // in order to find possible moves of a single box
            for (int j = 0; j < 4; j++)
                vertices.at(i, j).active = false;
            continue;
        }


        for (auto e : globals::deltaPos)
        {
            Pos next = globals::getPosition(p, e);

            if (b.at(next) & WALL)
                vertices.at(i, int(e)).active = false;
            else
            {
                vertices.at(i, int(e)).active = true;
            }
        }
    }
}

void clear_shift()
{
    for (int i = 0; i < index_num; i++)
        for (int j = 0; j < 4; j++)
        {
            auto &arr = vertices.at(i, j).shift;
            //static_assert (std::size(vertex::shift) == 4, "");
            arr[0] = nullptr;
            arr[1] = nullptr;
            arr[2] = nullptr;
            arr[3] = nullptr;
        }
}

void init_shift(const board &b)
{
    board &mb = const_cast<board&>(b);


    // 等于0 是否合理，是否应该改成 -1？ index = 0 很有可能有意义
    // 0对应 inner【empty room】中的第一个index，也就是 最上，最左的那个点
    // 而shift[0]表示他的上方，那肯定不属于 inner，所以 0 那个位置肯定是 active == 0
    assert(vertices.at(0, 0).active == false);
    clear_shift();

    for (int i = 0; i < index_num; i++)
    {
        Pos p;
        index_to_y_x(i, p);

        if (b.at(p) & OCCUPIED)
            continue;

        //假定该位置先放 box，判断box上下左右的 conenct 属性，这里称之为 zones
        mb.at(p) |= BOX;
        int_board zones;
        mark_connectivities_around_place(b, zones, p);
        //回滚
        mb.at(p) &= ~BOX;

        for (auto eFrom : globals::deltaPos)
        {
            if (vertices.at(i, int(eFrom)).active == false) continue;
            Pos fromPos = globals::getPosition(p, eFrom);
            if (zones.at(fromPos) == MAX_DISTANCE) continue;

            for (auto eTo : globals::deltaPos)
            {
                if (eFrom == eTo) continue; // 不同才有设置
                if (vertices.at(i, int(eTo)).active == false) continue;
                Pos toPos = globals::getPosition(p, eTo);
                if (zones.at(toPos) == MAX_DISTANCE) continue;

                if (zones.at(fromPos) == zones.at(toPos))
                {
                    vertices.at(i, int(eFrom)).shift[int(eTo)] = &vertices.at(i, int(eTo));
                }

            }
        }
    } // index of box
}

//设置 push 的值，就是对应 vertex中另外一个位置
void init_push(const board &b)
{
    for (int i = 0; i < vertices_num; i++)
    {
        Pos p;
        index_to_y_x(i, p);
        for (auto e : globals::deltaPos)
        {
            if (vertices.at(i, int(e)).active == false)
                continue;

            if (b.at(p) & OCCUPIED)
            {
                util::exit_with_error("something is bad");
            }
            // 为什么是  i%4 因为 vertex是4个一组，所以 i%4肯定就是上下左右4次循环
            // 这里减去就是朝 sokoban的方向推动
            /* ----> x
         * |    0           2
         * |  3 @ 1   变成 1  3
         * |    2           0
         * y
        */
            // 方向 参数应该是  direction = i % 4
            Pos nextPos = globals::sokobanPush(p, e);

            if (b.at(nextPos) & OCCUPIED)
                vertices.at(i, int(e)).push = nullptr;
            else
            {
                // push 后，得到 push后 box的index， 也就是 vertex 对应 （push_x, push_y） , sokoban方向 就是上面的 i%4
                // 这里有一个基本原理，sokoban怎么push的，sokoban肯定还是在 box 上次的方向
                int new_index = y_x_to_index(nextPos);

                // push后的值，就是另外一个 vertex点，神奇！
                vertices.at(i, int(e)).push = &vertices.at(new_index, int(e));
                // sokoban 的位置一定是上次 推动的位置，也就是 往下推， sokoban开始在上，推动后还是再上
            }
        }
    }
}

// 将所有weight设置为 固定很大，src设置为-1
void set_graph_weights_to_infinity()
{
    for (int i = 0; i < vertices_num; i++)
    {
        for (int j=0; j<4; j++)
        {
            vertices.at(i, j).weight = MAX_DISTANCE;
            vertices.at(i, j).src = nullptr;
        }
    }
}


// empty room?
// 从board 初始化图，很有可能是将一些box设置为wall，判定一个box
void build_graph(const board &b)
{
    // set up the active vertices and edges, don't compute anything yet

    if (board_contains_boxes(b))
        exit_with_error("boxes in board");

    init_index();

    init_active(b);
    init_shift(b);

    init_push(b);

    set_graph_weights_to_infinity();
}

void clear_weight_around_cell(int index)
{
    if (vertices.at(index, 0).active)
        vertices.at(index, 0).weight = 0;

    if (vertices.at(index, 1).active)
        vertices.at(index, 1).weight = 0;

    if (vertices.at(index, 2).active)
        vertices.at(index, 2).weight = 0;

    if (vertices.at(index, 3).active)
        vertices.at(index, 3).weight = 0;
}

void do_graph_iterations()
{
    // 这几个static貌似没有多大意义，我觉得可能是数组太大了，避免栈溢出所以改成static
//#define MAX_BFS_NODES (MAX_INNER * 4)
//    static int current[MAX_BFS_NODES]; // current 和 next 记录每次迭代的vertex集合
//    static int next[MAX_BFS_NODES];
//    static int visited[MAX_BFS_NODES]; // flag 是否访问过
//    int i, j, next_vertex; //局部变量
//    int current_num = 0; // 这两个记录 size
//    int next_num = 0;

//    int v; //局部变量


    constexpr int MAX_BFS_NODES = (MAX_INNER * 4);
    FixedVector<vertex*, MAX_INNER*4> curVec, nextVec;

    FixedMatix<bool, MAX_INNER, 4> visited;
    zero_board_bool(visited, MAX_INNER, 4);

    // init from start vertices
    for (int i = 0; i < vertices_num; i++)
    {
        for (int j=0; j<4; ++j)
        {
            vertex &ver = vertices.at(i, j);
            if (ver.weight == 0) // init点的weight为0，这里就找到了，不如传进来，这样避免遍历？
            {
                ver.src = nullptr;
                nextVec.append(&ver);
                visited.at(i, j) = true;
            }
        }
    }

    auto pCurVec = &curVec;
    auto pNextVec = &nextVec;
    while (pNextVec->size() != 0)
    {
        std::swap(pCurVec, pNextVec);
        pNextVec->clear();

        for (int i = 0; i < pCurVec->size(); i++)
        {
            vertex *pCurVer = pCurVec->at(i);

            for (int j = 0; j < 4; j++)
            {
                vertex *pShiftVer = pCurVer->shift[j];
                if (pShiftVer == nullptr) continue;

                if (pShiftVer->active == false)
                    exit_with_error("internal error1");

                if (visited.at(pShiftVer->id_index, pShiftVer->id_pos)) continue;

                visited.at(pShiftVer->id_index, pShiftVer->id_pos) = true;
                pShiftVer->weight = pCurVer->weight;
                pShiftVer->src = pCurVer;
                pNextVec->append(pShiftVer);
            }

            vertex *pPushVer = pCurVer->push;

            if (pPushVer == nullptr) continue;

            if (pPushVer->active == false)
                exit_with_error("internal error2");

            if (visited.at(pPushVer->id_index, pPushVer->id_pos)) continue;

            visited.at(pPushVer->id_index, pPushVer->id_pos) = true;
            pPushVer->weight = pCurVer->weight + 1;
            pPushVer->src = pCurVer;
            pNextVec->append(pPushVer);

            if (pNextVec->size() >= MAX_BFS_NODES)
                exit_with_error("bfs overflow");
        } // loop on current vertices with weight d
    } // while next d

}

int get_weight_around_cell(int index)
{
    int min = MAX_DISTANCE;

    for (int j = 0; j < 4; j++)
    {
        const vertex &ver = vertices.at(index, j);
        if (ver.active == false) continue;

        if (ver.weight < min)
            min = ver.weight;
    }
    return min;
}
