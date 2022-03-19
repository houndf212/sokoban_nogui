#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H
#include "types.h"
#include "maskmatrix.h"

class MatrixGraph
{
public:
    typedef Pos vertex_t;
    typedef MaskMatrix::size_type distance_t;
    typedef BasicPosHash<Pos::value_type> hash_t;
    typedef PosVector VertexVector;

    MatrixGraph(const ElementsMatrix &m);

    PosList shortest_path(Pos start, Pos goal) const;

    VertexVector neighbors(vertex_t v1) const;

    static distance_t heuristic(vertex_t v1, vertex_t v2)
    {
        return Manhattan_Distance(v1, v2);
    }

    static constexpr distance_t distance(vertex_t , vertex_t )
    {
        return 1;
    }

private:
    MaskMatrix matrix;
};

#endif // MATRIXGRAPH_H
