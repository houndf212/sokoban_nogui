#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H
#include <iostream>
#include "fixedvector.h"
using std::cout;
using std::endl;

template<typename T>
void print(const T &);

template<FIXED_SIZE_TYPE _R, FIXED_SIZE_TYPE _C>
void print(const FixedMatix<bool, _R, _C> &m, int h, int w)
{
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
            cout << m.at(i, j) ;

        cout <<endl;
    }
    cout <<endl;
}

#endif // DEBUG_PRINT_H
