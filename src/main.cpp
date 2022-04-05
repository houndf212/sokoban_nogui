#include "xsb.h"
#include "boardparam.h"
#include "boardgraph.h"
#include <iostream>
#include <string.h>
#include <chrono>
#include "debug_print.h"
#include "preprocess.h"
#include "board.h"

using namespace std;
using namespace XSB;

bool check()
{
    extern bool check_matrix_iterator();
    assert(check_matrix_iterator());
    return true;
}

void test()
{
    extern void test_print();
    extern void test_console_board();
    extern void test_boxmover();
    extern void test_assignmentproblem();
    extern void test_baordgraph();
    extern void test_matrixgraph();
    extern void test_munkres();
    extern void test_HungarianAlg();
    extern void test_matrix_iterator();
    extern void test_matrix_resize();

//    test_matrix_resize();
}


void readXSokobanFile(const char* pFile)
{
    std::list<XsokobanLevelInfo> lst;
    parseLevelFile(lst, pFile);
    cout << "read xsokoban level size: " << lst.size() <<endl;

    for (const XsokobanLevelInfo &info : lst)
    {
        cout << info.level <<endl;
        const auto &m = info.xsbMat;
        for (auto row=m.szero(); row<m.row_size(); ++row) {
            for (auto &e : m.row_range(row)) {
                cout<<XSB::e_to_char(e);
                //cout<<" ";
            }
            cout<<endl;
        }
        cout << endl;
        cout << info.title <<endl;
        cout << info.author <<endl;
        cout << endl;
        cout << endl;
    }
}

void readSolutionFile(const char* pFile)
{
    list<XSBSolutionInfo> lst;
    parseSolutionFile(lst, pFile);
    cout << "read solution size: " << lst.size() <<endl;

    for (const XSBSolutionInfo &info : lst)
    {
        BoardParam param;
        param.set_matrix(info.xsbMat);
        if (param.can_solve(info.solution))
        {
            cout << "solved: " << info.level <<endl;
        }
        else
        {
            if (!info.solution.empty())
                cout << "**";

            cout << "unsolved: " << info.level <<endl;
        }
    }
}

void tryOne(const char *pFile)
{
    ElementsMatrix em = from_file(pFile);
    if (em.row_size() == 0 || em.col_size() == 0)
    {
        cout << "format error!" <<endl;
        return;
    }

    PreProcess::preprocess_level(em);
    boardUtil::copy_board(boardUtil::initial_board, em);

    BoardParam param;
    param.set_matrix(em);

    auto t1 = chrono::system_clock::now();

    auto ml = BoardGraph::solve(param);

    auto t2 = chrono::system_clock::now();

    auto ms = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    if (param.can_solve(ml))
    {
        cout << "Level X\n\n";
        print(em);
        cout << "Solution\n";
        print(ml);
        cout << "\n";
        cout << "Solver time: " << ms << "ms" <<endl;
    }
    else
    {
        cout << "Error!\n";
    }
}

void preprocessLevelFile(const char *pFile)
{
    std::list<XsokobanLevelInfo> lst;
    parseLevelFile(lst, pFile);
    cout << "read xsokoban level size: " << lst.size() <<endl;

    for (const XsokobanLevelInfo &info : lst)
    {
        cout << info.level <<endl;
        board m = info.xsbMat;
        PreProcess::preprocess_level(m);
        boardUtil::copy_board(boardUtil::initial_board, m);
        for (auto row=m.szero(); row<m.row_size(); ++row) {
            for (auto &e : m.row_range(row)) {
                cout<<XSB::e_to_char(e);
                //cout<<" ";
            }
            cout<<endl;
        }
        cout << endl;
        cout << info.title <<endl;
        cout << info.author <<endl;
        cout << endl;
        cout << endl;
    }
}

void printUsge()
{
    cout << "sokoban -check solution.sok" <<endl;
    cout << "sokoban -read  level.sok" <<endl;
    cout << "sokoban -tryone one.sok" <<endl;
}

int main(int argc, char *argv[])
{
//    assert(check());
//    test();
//    return 0;

    if (argc != 3)
    {
        printUsge();
        return -1;
    }
    const char *mode = argv[1];
    const char *pFile = argv[2];

    if (0 == strcmp(mode, "-check"))
        readSolutionFile(pFile);
    else if (0 == strcmp(mode, "-read"))
        readXSokobanFile(pFile);
    else if (0 == strcmp(mode, "-try"))
        tryOne(pFile);
    else if (0 == strcmp(mode, "-pre"))
        preprocessLevelFile(pFile);

    return 0;
}

