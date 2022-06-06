#ifndef BLOCK_HEIGHT_LINER
#define BLOCK_HEIGHT_LINER

#include "../CodeStream.h"
#include "Liner.h"
#include "PrintUtil.h"
#include <iostream>

class BlockHeightLiner : public Liner
{
    using Liner::Liner;

    StringLoc last_opener;
    string    begin_line;

    bool hasOpened = false;

public:
    int  depth               = 0;
    bool openedBlockLastLine = false;

    inline void onBeginBlock()
    {
        // depth++;
        openedBlockLastLine = true;
        hasOpened           = (depth > 0);

        if (depth == 0) {
            last_opener = curLoc();
        }
    }

    inline void onChar()
    {
        if (not isspace(curChar()) and openedBlockLastLine) {
            depth++;
            begin_line          = curLine();
            openedBlockLastLine = false;
        }
    }

    inline void onBeginLine()
    {
        if (openedBlockLastLine) {
            depth++;
            begin_line          = curLine();
            openedBlockLastLine = false;
        }
    }

    inline void onEndBlock()
    {
        depth--;
        hasOpened       = (depth > 0);
        StringLoc ender = curLoc();

        // string headerext = ".h";
        // bool   isHeader  = cs_p->input_name.length() >= headerext.length()
        // and
        //                 std::equal(headerext.rbegin(), headerext.rend(),
        //                            cs_p->input_name.rbegin());

        int linediff = last_opener.lineDiff(ender) - 1;

        if (linediff > 30) {

            stringstream ss;
            ColorStack   cstack;
            string leftl = curLine().substr(curLine().find_first_not_of(" \t"),
                                            curLine().length());


            string ll = cstack.miniColor(
                cstack.miniColor(cs_p->input_name, ST_UNDERLINE) + ":" +
                    cstack.miniColor(to_string(last_opener.line_no) + "-" +
                                         to_string(curLoc().line_no),
                                     C_LIGHT_RED) +
                    cstack.miniColor(" function exceeds 30 lines", ST_BOLD),
                ST_BOLD);

            string ll_un =
                cs_p->input_name + ":" + to_string(last_opener.line_no) + "-" +
                to_string(curLoc().line_no) + " function exceeds 30 lines";
            // size_t startpos =
            ss << ll << cstack.miniColor("┌ ", C_RED)
               << cstack.miniColor("line", C_DARK_GRAY)
               << cstack.miniColor(":", C_DARK_GRAY)
               << cstack.miniColor(to_string(last_opener.line_no), ST_BOLD)
               << endl;


            string linediff_str = to_string(linediff) + " lines";

            ss << string(ll_un.length() - linediff_str.length(), ' ')
               << cstack.miniColor(linediff_str, C_LIGHT_RED)
               << cstack.miniColor("┤ ", C_RED) << "     :  " << endl;

            ss << string(ll_un.length(), ' ') << cstack.miniColor("└ ", C_RED)
               << cstack.miniColor("line", C_DARK_GRAY)
               << cstack.miniColor(":", C_DARK_GRAY)
               << cstack.miniColor(to_string(curLoc().line_no), ST_BOLD)
               << endl;
            //    << leftl.substr(0, cs_p->display_width) << endl;


            // << ╭ << │ << ╰
            err_stack.push_back(ss.str());
        }
    }
};


#endif