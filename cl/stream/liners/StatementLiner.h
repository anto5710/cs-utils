
#include "../CodeStream.h"
#include "../StringLoc.h"
#include "Liner.h"
#include "PrintUtil.h"
#include <iostream>
#include <regex>
#include <sstream>

class StatementLiner : public Liner
{
    using Liner::Liner;


    void onBeginStatement() {}
    void onEndStatement()
    {
        cs_p->Last_statement = cs_p->statement.str();
        cs_p->statement.str(string());

        if (not cs_p->Last_statement.empty()) {
            // cout << mcolor(" :: ", C_BROWN) << cs_p->Last_statement << endl;
        }
    }

    const string REGEXP_CLASS = "class\\s+\\w+";
    const string REGEX_IF     = "(else|(else)?\\s*if\\s*\\([^;]+)";
    const string REGEX_LOOP   = "(while|for)\\s*\\s*\\([^;]+";

    BlockType classifyBlock(string last_line)
    {
        smatch matches;
        // cout << "matching: " << last_line << "to" << (REGEXP_CLASS) << endl;
        if (regex_search(last_line, matches, regex(REGEXP_CLASS))) {
            return BLOCK_CLASS;

        } else if (regex_search(last_line, matches, regex(REGEX_IF))) {
            return BLOCK_IF;

        } else if (regex_search(last_line, matches, regex(REGEX_LOOP))) {
            return BLOCK_LOOP;
        }

        return BLOCK_OTHER;
    }


    void onEndBlock()
    {
        cs_p->statement.str(string());
        onEndStatement();
    }

    void onBeginBlock()
    {
        onEndStatement();

        cs_p->block_type = classifyBlock(cs_p->Last_statement);
        // cout << mcolor("?:: ", C_GREEN) << classifyBlock(cs_p->Last_statement)
        //      << endl;
    }

    void onChar()
    {
        if (curChar() == ';') {
            onEndStatement();

        } else if (curChar() != '{') {

            bool empty_yet = (cs_p->statement.rdbuf()->in_avail() == 0);
            cs_p->statement << curChar();

            if (empty_yet) {
                onBeginStatement();
            }
        }
    }
};