#include "matrixgraph.h"
#include "dijkstra.h"




MatrixGraph::MatrixGraph(const ElementsMatrix &m)
    :matrix(m.row_size(), m.col_size())
{
    for (auto it=m.range(); it; ++it) {
        //mask表示可行路径
        if (*it == Elements::floor)
            matrix.mask(it.pos());
    }
}

PosList MatrixGraph::shortest_path(Pos start, Pos goal) const
{
    typedef Dijkstra<MatrixGraph> G;
    auto path = G::AStart_path(*this, start, goal);
    return path.first;
}

MatrixGraph::VertexVector MatrixGraph::neighbors(MatrixGraph::vertex_t v1) const
{
    VertexVector vec;
    auto func = [&] (Pos p) {
        if (matrix.isInMatrix(p) && matrix.is_mask(p))
            vec.push_back(p);
    };
    func(Pos(v1.row()+1, v1.col()));
    func(Pos(v1.row()-1, v1.col()));
    func(Pos(v1.row(), v1.col()+1));
    func(Pos(v1.row(), v1.col()-1));
    return vec;
}
