#include "boardgraph.h"
#include <string>
#include <queue>
#include "assignmentproblem.h"
#include "maskmatrix.h"
#include "matrixgraph.h"
#include "dijkstra.h"

MoveList BoardGraph::solve(const BoardGraph::vertex_t &start)
{
    typedef BoardGraph BG;
    typedef Dijkstra<BG> G;
    BG graph;

    auto path = G::AStart_path(graph, start, start.to_goal());
    path.first.push_front(start);
    auto mlst = trans_to(path.first);
    assert(start.can_solve(mlst));
    return mlst;
}

BoardGraph::VertexList BoardGraph::neighbors(const vertex_t &param)
{
    return param.next_move();
}

BoardGraph::distance_t BoardGraph::distance(const BoardGraph::vertex_t &v1, const BoardGraph::vertex_t &v2)
{
    Q_UNUSED(v1);
    Q_UNUSED(v2);
    return distance_t(1);
}

BoardGraph::distance_t BoardGraph::heuristic(const BoardGraph::vertex_t &v1, const BoardGraph::vertex_t &v2)
{
    //这个函数需要调整，当goal的数值过大的时候，这里的计算成为了瓶颈
    assert(v2.is_done());
    Q_UNUSED(v2);

    const auto size = static_cast<type_size>(v1.goals_size());
    IntMatrix m(size, size, 0);

    auto index = m.szero();
    for (auto box : v1.boxes()) {
        for (auto goal : *v1.goals()) {
            distance_t d = Manhattan_Distance(box, goal);
            Pos p(index/size, index%size);
            m.set(p, d);
            index++;
        }
    }

    auto p = AssignmentProblem::min_assignment<distance_t>(m);
    return p.second;
}

BoardGraph::distance_t BoardGraph::greedy_search(const IntMatrix &m)
{
    //实现了 匈牙利算法,不需要贪婪搜索了
    assert(false);

    assert(m.row_size() == m.col_size());
    const auto size = m.row_size();
    MaskMatrix mask(m.row_size(), m.col_size());

    auto mask_p = [&mask](Pos p) {
        for(auto i=mask.szero(); i<mask.row_size(); ++i) {
            mask.mask(Pos(i, p.col()));
            mask.mask(Pos(p.row(), i));
        }
    };

    auto comp = [&m](const Pos &p1, const Pos &p2) {
        return m.get(p1)>m.get(p2);
    };

    PosVector pq;


    for (auto it=m.range(); it; ++it) {
        pq.push_back(it.pos());
        std::push_heap(begin(pq), end(pq), comp);
    }

    distance_t weight = 0;
    auto n = 1;
    while(n!=size) {
        std::pop_heap(begin(pq), end(pq), comp);
        Pos min = pq.back();
        pq.pop_back();

        if (mask.is_mask(min))
            continue;
        else {
            weight += m.get(min);
            mask_p(min);
            n++;
        }
    }
    return weight;
}

MoveList BoardGraph::trans_to(const BoardGraph::VertexList &ves)
{
    auto first = begin(ves);
    auto second = first;
    ++second;

    MoveList mlst;
    while(second != end(ves)) {
        auto m = to_movelist(*first, *second);
        mlst.check_append(m);
        ++first;
        ++second;
    }
    return mlst;
}

//计算两个 box move 之间的插值 v1 --> v2
MoveList BoardGraph::to_movelist(const BoardGraph::vertex_t &v1, const BoardGraph::vertex_t &v2)
{
    //先找到 box 移动的位置
    auto v2_not_box = [&v2](const Pos &p) { return !v2.is_box(p); };
    auto it1 = std::find_if(begin(v1.boxes()), end(v1.boxes()), v2_not_box);
    assert(it1 != end(v1.boxes()));
    Pos box1 = *it1;

    auto v1_not_box = [&v1](const Pos &p) { return !v1.is_box(p); };
    auto it2 = std::find_if(begin(v2.boxes()), end(v2.boxes()), v1_not_box);
    assert(it2 != end(v2.boxes()));
    Pos box2 = *it2;

    assert(box1 != box2);
    Direction push = pos_to(box1, box2);
    assert(push!=Direction::NotValid);
    Pos man_from = v1.man();
    Pos man_to = pos_move(box1, reverse(push));

    MoveList mlst;
    if (man_from != man_to) {

        typedef Dijkstra<MatrixGraph> G;
        MatrixGraph g(v1.cache_room());

        auto path = G::AStart_path(g, man_from, man_to);
        for (auto p : path.first) {
            auto d = pos_to(man_from, p);
            mlst.check_push(d);
            man_from = p;
        }
    }

    mlst.check_push(add_push(push));
    man_from = box1;

    if (man_from != v2.man()) {
        typedef Dijkstra<MatrixGraph> G;
        MatrixGraph g(v2.cache_room());

        auto path = G::AStart_path(g, man_from, v2.man());
        for (auto p : path.first) {
            auto d = pos_to(man_from, p);
            mlst.check_push(d);
            man_from = p;
        }
    }
    assert(man_from == v2.man());
    return mlst;
}

