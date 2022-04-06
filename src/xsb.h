#ifndef XSB_H
#define XSB_H
#include "types.h"
#include <string>
#include "movelist.h"

//描述 xsb格式

namespace XSB
{
using std::string;

char e_to_char(Elements_t e);
Elements_t char_to_e(char c);

char d_to_char(Direction d);
Direction char_to_d(char c);

//给外围无用的floor设置为wall
void replace_outer_floor(ElementsMatrix &m);

//去掉多余的墙
ElementsMatrix remove_outer_wall(ElementsMatrix m);

ElementsMatrix from_file(const char *filestr);
void to_file(const ElementsMatrix &m, const char *filestr);
std::string to_string(const ElementsMatrix &m);

bool is_sokoban_line(const std::string &line);
bool is_solution_line(const string &line);
ElementsMatrix from_stringList(std::list<std::string> lst);

struct XsokobanLevelInfo
{
    std::string level;
    std::string title;
    std::string author;
    ElementsMatrix xsbMat;
};

struct XSBSolutionInfo
{
    string level;
    string solutionText;
    string solver;
    string time;
    string date;
    MoveList solution;
    ElementsMatrix xsbMat;
};

void parseLevelFile(std::list<XsokobanLevelInfo> &lst, const char* pFile);
void parseSolutionFile(std::list<XSBSolutionInfo> &lst, const char* pFile);

}

#endif // XSB_H
