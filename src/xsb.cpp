#include "xsb.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace XSB;
using namespace std;


char XSB::e_to_char(Elements_t e)
{
    switch (e) {
    case Elements::man:
        return '@';
    case Elements::man_goal:
        return '+';
    case Elements::box:
        return '$';
    case Elements::box_goal:
        return '*';
    case Elements::wall:
        return '#';
    case Elements::goal:
        return '.';
    case Elements::floor1:
        return '-';
    }
    assert(false);
    return '-';
}

Elements XSB::char_to_e(char c)
{
    switch (c) {
    case '@':
        return Elements::man;
    case '+':
        return Elements::man_goal;
    case '$':
        return Elements::box;
    case '*':
        return Elements::box_goal;
    case '#':
        return Elements::wall;
    case '.':
        return Elements::goal;
    case ' ':
    case '-':
    case '_':
        return Elements::floor1;
    default:
        assert(false);
        break;
    }
    return Elements::floor1;
}

ElementsMatrix XSB::from_file(const char *filestr)
{
    ifstream in(filestr);
    if (!in)
    {
        cout << "cannot open file: " << filestr <<endl;
        return ElementsMatrix();
    }

    list<string> lst;
    string line;
    while (getline(in, line))
        lst.push_back(line);

    return from_stringList(lst);
}


char XSB::d_to_char(Direction d)
{
    assert(d!=Direction::NotValid);
    switch (d) {
    case Direction::push:
    case Direction::NotValid:
        break;

    case Direction::up:
        return 'u';
    case Direction::push_up:
        return 'U';

    case Direction::down:
        return 'd';
    case Direction::push_down:
        return 'D';

    case Direction::left:
        return 'l';
    case Direction::push_left:
        return 'L';

    case Direction::right:
        return 'r';
    case Direction::push_right:
        return 'R';
    }

    assert(false);
    return '*';
}

Direction XSB::char_to_d(char c)
{
    switch (c) {
    case 'u':
        return Direction::up;
    case 'U':
        return Direction::push_up;

    case 'd':
        return Direction::down;
    case 'D':
        return Direction::push_down;

    case 'l':
        return Direction::left;
    case 'L':
        return Direction::push_left;

    case 'r':
        return Direction::right;
    case 'R':
        return Direction::push_right;
    default:
        break;
    }
    assert(false);
    return Direction::NotValid;
}

std::string XSB::to_string(const ElementsMatrix &m)
{
    std::string str;
    for (auto row=m.szero(); row<m.row_size(); ++row) {
        std::transform(m.row_begin(row), m.row_end(row), std::back_inserter(str), e_to_char);
        str.push_back('\n');
    }
    return str;
}

template <class R>
static void test_set_range(const R &range)
{
    for (auto &v : range) {
        if (v != Elements::wall) {
            //这里说明room不是被墙包裹的
            assert(v == Elements::floor1);
            v = Elements::wall;
        }
        else
            break;
    }
}

void XSB::replace_outer_floor(ElementsMatrix &m)
{
    //保证所有的room都是被wall包裹的
    //四个方向 从上到下从下倒上，从左到右，从右到左
    for (auto row=m.szero(); row<m.row_size(); ++row) {
        test_set_range(m.row_range(row));
        test_set_range(m.row_reverse_range(row));
    }

    for (auto col=m.szero(); col<m.col_size(); ++col) {
        test_set_range(m.col_range(col));
        test_set_range(m.col_reverse_range(col));
    }
}

template <class R>
static bool is_wall_range(const R &range)
{
    auto equal_wall = [](Elements_t e) { return e == Elements::wall; };
    return std::all_of(range.begin(), range.end(), equal_wall);
}

static ElementsMatrix remove_row(type_size row, const ElementsMatrix &m)
{
    ElementsMatrix em(m.row_size()-1, m.col_size(), Elements::floor1);

    for (auto r=m.szero(); r<m.row_size(); ++r) {
        if (r == row) continue;
        auto r_to = r<row ? r : r-1;
        std::copy(m.row_begin(r), m.row_end(r), em.row_begin(r_to));
    }
    return em;
}

static ElementsMatrix remove_col(type_size col, const ElementsMatrix &m)
{
    ElementsMatrix em(m.row_size(), m.col_size()-1, Elements::floor1);

    for (auto c=m.szero(); c<m.col_size(); ++c) {
        if (c == col) continue;
        auto c_to = c<col ? c : c-1;
        std::copy(m.col_begin(c), m.col_end(c), em.col_begin(c_to));
    }
    return em;
}

ElementsMatrix XSB::remove_outer_wall(ElementsMatrix m)
{
    for (auto row=m.szero(); row<m.row_size()-1;) {
        if (is_wall_range(m.row_range(row)) && is_wall_range(m.row_range(row+1))) {
            m = remove_row(row, m);
        }
        else
            row++;
    }

    for (auto col=m.szero(); col<m.col_size()-1;) {
        if (is_wall_range(m.col_range(col)) && is_wall_range(m.col_range(col+1))) {
            m = remove_col(col, m);
        }
        else
            col++;
    }

    return m;
}

static void trim_end(std::string &s)
{
    while (!s.empty())
    {
        if (std::isspace(s.back()))
            s.pop_back();
        else
            return;
    }
}

ElementsMatrix XSB::from_stringList(std::list<std::string> lst)
{
    if (lst.empty())
        return ElementsMatrix(0, 0);

    type_size col_size = 0;
    for (auto &line : lst)
    {
        trim_end(line);
        col_size = std::max(col_size, type_size(line.size()));
    }
    type_size row_size = lst.size();

    ElementsMatrix em(row_size, col_size, Elements::floor1);

    int row = 0;
    for (const auto &line : lst)
    {
        std::transform(line.begin(), line.end(), em.row_begin(row), char_to_e);
        ++row;
    }
#if 0
    replace_outer_floor(em);
    return remove_outer_wall(em);
#endif
    return em;
}
static const std::string prefix_level2 ="Sasquatch";
static const std::string prefix_level  = "Level";
static const std::string prefix_title  = "Title:";
static const std::string prefix_author = "Author:";


inline bool
starts_with(const string &str, const string &s)
{
    return str.size() >= s.size() && str.compare(0, s.size(), s) == 0;
}

inline bool _isHeaderLine(const string &line)
{
    return starts_with(line, prefix_level) ||
            starts_with(line, prefix_level2);
}

void XSB::parseLevelFile(std::list<XsokobanLevelInfo> &lst, const char* pFile)
{
    std::ifstream infile(pFile);
    if (!infile)
        return;

    string line;
    while (getline(infile, line))
    {
        if (_isHeaderLine(line))
            break;
    }

    while (_isHeaderLine(line))
    {
        XsokobanLevelInfo info;
        info.level = line;
        list<string> xsbList;
        while (getline(infile, line))
        {
            trim_end(line);
            if (line.empty())
                continue;

            if (_isHeaderLine(line))
            {
                break;
            }

            if (starts_with(line, prefix_title))
            {
                info.title = line;
                continue;
            }

            if (starts_with(line, prefix_author))
            {
                info.author = line;
                continue;
            }

            if (is_sokoban_line(line))
            {
                xsbList.push_back(line);
                continue;
            }

            cout << "WARNING: unknow line: " << line <<endl;
        }

        if (!xsbList.empty())
        {
            info.xsbMat = from_stringList(xsbList);
            if (info.xsbMat.row_size()!=0 && info.xsbMat.col_size()!=0)
                lst.push_back(info);
        }
    }
}

bool XSB::is_sokoban_line(const std::string &line)
{
    for (char c : line)
    {
        switch (c)
        {
            case '@':
            case '+':
            case '$':
            case '*':
            case '#':
            case '.':
            case ' ':
            case '-':
            case '_':
                break;
            default:
                return false;
        }
    }
    return true;
}
bool XSB::is_solution_line(const string &line)
{
    for (char c : line)
    {
        switch (c)
        {
        case 'u':
        case 'U':
        case 'd':
        case 'D':
        case 'l':
        case 'L':
        case 'r':
        case 'R':
            continue;
        default:
            return false;
        }
    }
    return !line.empty();
}

static const std::string prefix_solution  = "Solution";
static const std::string prefix_solver    = "Solver:";
static const std::string prefix_time      = "Solver time:";
static const std::string prefix_date    = "Solver date:";

void XSB::parseSolutionFile(std::list<XSBSolutionInfo> &lst, const char *pFile)
{
    std::ifstream infile(pFile);
    if (!infile)
        return;

    string line;
    while (getline(infile, line))
    {
        if (_isHeaderLine(line))
            break;
    }

    while (_isHeaderLine(line))
    {
        XSBSolutionInfo info;
        info.level = line;
        list<string> xsbList;
        while (getline(infile, line))
        {
            trim_end(line);
            if (line.empty())
                continue;

            if (_isHeaderLine(line))
            {
                break;
            }

            if (starts_with(line, prefix_solver))
            {
                info.solver = line;
                continue;
            }

            if (starts_with(line, prefix_time))
            {
                info.time = line;
                continue;
            }

            if (starts_with(line, prefix_date))
            {
                info.date = line;
                continue;
            }

            if (starts_with(line, prefix_solution))
            {
                continue;
            }

            if (is_solution_line(line))
            {
                info.solutionText += line;
                continue;
            }

            if (is_sokoban_line(line))
            {
                xsbList.push_back(line);
                continue;
            }

            cout << "WARNING: unknow line: " << line <<endl;
        }

        if (!info.solutionText.empty())
        {
            for (char c : info.solutionText)
                info.solution.check_push(char_to_d(c));
        }

        if (!xsbList.empty())
        {
            info.xsbMat = from_stringList(xsbList);
            if (info.xsbMat.row_size()!=0 && info.xsbMat.col_size()!=0)
                lst.push_back(info);
        }
    }
}


