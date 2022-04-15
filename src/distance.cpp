#include "distance.h"
#include "graph.h"

FixedMatix<int, MAX_INNER, MAX_INNER> distance_from_to;
FixedVector<bool, MAX_INNER> impossible_place;

FixedVector<bool, MAX_INNER> can_get_to_dest;
FixedVector<bool, MAX_INNER> can_get_from_start;


// 记录 inner 任意一点是否能到到target【利用计算好的distance矩阵】
void compute_can_get_to_dest(const board &b)
{
    can_get_to_dest.setSize(index_num);
    for (int i = 0; i < index_num; i++)
    {
        can_get_to_dest.at(i) = 0;

        for (int k=0; k<index_num; ++k)
        {
            Pos p;
            index_to_y_x(k, p);
            if (b.at(p) & TARGET)
            {
                if (distance_from_to.at(i, k) < MAX_DISTANCE)
                    can_get_to_dest.at(i) = true;
            }
        }
    }
}

//反过来， box的位置能否到达某点
void compute_can_from_start(const board &b)
{
    can_get_from_start.setSize(index_num);
    for (int i = 0; i < index_num; i++)
    {
        can_get_from_start.at(i) = false;
        for (int k=0; k<index_num; ++k)
        {
            Pos p;
            index_to_y_x(k, p);
            if (b.at(p) & BOX)
            {
                if (distance_from_to.at(k, i) < MAX_DISTANCE)
                    can_get_from_start.at(i) = true;
            }
        }
    }
}


//标记那些地方不能去，也就是下面的if， 如果 box去不了，并且某个位置 也去不了target这种位置就没用
void set_impossible_places()
{
    impossible_place.setSize(index_num);
    for (int i = 0; i < index_num; i++)
        impossible_place.at(i) = true;

    for (int i = 0; i < index_num; i++)
        if (can_get_from_start.at(i) && can_get_to_dest.at(i))
            impossible_place.at(i) = false;

}

void set_distances(const board &b)
{
    board board_without_boxes;
    clear_boxes(b, board_without_boxes);

    build_graph(board_without_boxes);

    for (int from_index = 0; from_index < index_num; from_index++)
    {
        set_graph_weights_to_infinity();
        clear_weight_around_cell(from_index);
        do_graph_iterations();

        for (int to_index = 0; to_index < index_num; to_index++)
        {
            int dist = get_weight_around_cell(to_index);
            distance_from_to.at(from_index, to_index) = dist;
        }
    }

    compute_can_get_to_dest(b);
    compute_can_from_start(b);
    set_impossible_places();
}
