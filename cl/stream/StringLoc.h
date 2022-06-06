#ifndef STRING_LOC
#define STRING_LOC


#include "CodeStream.h"
#include <iostream>

using namespace std;

class CodeStream;
class StringLoc
{
public:
    int line_no;
    int col_no;

    StringLoc() { StringLoc(0, 0); }

    StringLoc(int line_number, int col_number)
    {
        line_no = line_number;
        col_no  = col_number;
    }

    int lineDiff(StringLoc &other) { return abs(other.line_no - line_no); }

    int colDiff(StringLoc &other) { return abs(other.col_no - col_no); }

    void nextChar() { col_no++; }

    void nextLine()
    {
        line_no++;
        col_no = 0;
    }

    operator std::string()
    {
        return to_string(line_no) + ":" + to_string(col_no);
    }
};


#endif