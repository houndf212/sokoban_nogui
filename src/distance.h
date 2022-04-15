#ifndef DISTANCE_H
#define DISTANCE_H

#include "board.h"

void set_distances(const board &b);

extern FixedMatix<int, MAX_INNER, MAX_INNER> distance_from_to;
extern FixedVector<bool, MAX_INNER> impossible_place;

#endif // DISTANCE_H
