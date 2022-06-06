#ifndef INDENT_LINER
#define INDENT_LINER

#include "BlockHeightLiner.h"
#include "Liner.h"
#include <iostream>

class IndentLiner : public Liner
{
    using Liner::Liner;

    float average_blank_l = 0;
    int   i               = 1;
    int   left_blank_l    = 0;
    bool  blank_done      = false;

    int counter = 0;

    inline void onBeginLine()
    {
        left_blank_l = 0;
        blank_done   = false;
    }

    inline void onChar()
    {
        blank_done = blank_done || not isspace(curChar());
        if (not blank_done) {
            left_blank_l++;
        }
    }

    inline void onEndLine()
    {
        int depth = (((BlockHeightLiner *)(cs_p->blanker))->depth);

        if (not curLine().empty() and
            (left_blank_l < (int)curLine().length()) && depth > 0

            && (curLine().rfind("public:", 0) != 0 &&
                curLine().rfind("private:", 0) != 0)) {

            average_blank_l =
                (average_blank_l * i + (((float)left_blank_l / (float)depth))) /
                (i + 1);

            i++;
        }
    }
};

#endif
