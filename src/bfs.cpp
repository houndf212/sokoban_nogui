#include "bfs.h"

// 初始化 距离， 也就是 box 离 target有多远， 默认一个很大的值
void init_dist(int_board &dist)
{
    int height = initial_board.row_size();
    int width  = initial_board.col_size();

    for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
        dist.at(i, j) = MAX_DISTANCE;
}

// 宽度优先搜索算法，相对应的是深度优先  dfs
//这个算法是以 start点为中心，每次循环走上下左右，并让 distance + 1 ，由于宽度优先，所以下次的点肯定更远，
// 这个也就是 seed算法，计算每个点（也就是 index)到初始点有多远，需要多少步
void bfs_from_place(const board &b, int_board &dist, Pos start, bool ignore_boxes)
{
    // do bfs, assuming "dist" is initialized
    int occupied_mask = OCCUPIED;
    if (ignore_boxes)
        occupied_mask = WALL;

    FixedVector<Pos, MAX_SIZE * MAX_SIZE> currentVec, nextVec;
    currentVec.append(start);

    int d = 0;
    dist.at(start) = d;

    auto pCurrent = &currentVec;
    auto pNext = &nextVec;

    do
    {
        pNext->clear();
        d++;
        for (int i = 0; i < pCurrent->size(); i++)
        {
            Pos p = pCurrent->at(i);
            for (auto e : globals::deltaPos)
            {
                Pos next = globals::getPosition(p, e);
                // 这里的 判断 dist[y][x] == 1000000 可以换成 dist[y][x] > d，就变成正真的最短路径算法
                //由于board的步长一定是1 ，并且没有magic short path之类的，所以只要是初始值就是未访问过的
                // 只要不是初始值， 那么 dist[y][x] 一定不等于初始值，并且等于==d
                if ((b.at(next) & occupied_mask) == 0 && dist.at(next) == MAX_DISTANCE)
                {
                    //每次添加一个新的点
                    pNext->append(next);
                    dist.at(next) = d;
                }
            }
        } // i

        std::swap(pCurrent, pNext);

    } while (pCurrent->size() > 0);
}

inline
void bfs(const board &b, int_board &dist, Pos start, bool ignore_boxes)
{
    init_dist(dist);
    bfs_from_place(b, dist, start, ignore_boxes);
}

//这个算法和 自己实现的sokoban 的 roomslice 一致，就是标记empty room中哪些点是属于同一块的
// 也就是这里所谓的connectivity，本质上一个意思，自己的算法是 深度优先算法，如果只是标记flag，本质上是一样的。
// 返回值就是 分成了多少块，也就是连接区域有多少块
int mark_connectivities(const board &b, int_board &dist)
{
    init_dist(dist);

    int c = 0;
    for (int i=0; i<boardUtil::index_num; ++i)
    {
        Pos p;
        index_to_y_x(i, p);
        if (dist.at(p) != MAX_DISTANCE) continue; // 设置
        if (b.at(p) & OCCUPIED) continue;

        int_board local_dist;
        bfs(b, local_dist, p, false);

        for (int k=0; k<boardUtil::index_num; ++k)
        {
            index_to_y_x(k, p);
            if (local_dist.at(p) != MAX_DISTANCE) // or <
                dist.at(p) = c;
        }
        c++;
    }
    return c;
}

//简化版的 mark_connectivities， 只需要区分 start点上下左右的connectivity性质，也就是最多切4片，当然最少一片
void mark_connectivities_around_place(const board &b, int_board &dist, Pos start)
{
    init_dist(dist);

    for (auto e : globals::deltaPos)
    {
        Pos p = globals::getPosition(start, e);

        if (isInInner(p) == false) continue;
        if (dist.at(p) != MAX_DISTANCE) continue;
        if (b.at(p) & OCCUPIED) continue;

        int_board local_dist;
        bfs(b, local_dist, p, false);

        for (int k=0; k<boardUtil::index_num; ++k)
        {
            index_to_y_x(k, p);
            if (local_dist.at(p) != MAX_DISTANCE) // or <
                dist.at(p) = static_cast<int>(e);
        }
    }
}
