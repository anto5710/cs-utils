#ifndef CODE_STREAM
#define CODE_STREAM

#include "StringLoc.h"
#include "liners/Liner.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

enum BlockType {
    BLOCK_FUNCTION = 0,
    BLOCK_CLASS    = 1,
    BLOCK_LOOP     = 2,
    BLOCK_IF       = 3,
    BLOCK_OTHER    = 4
};


namespace blocktype_values
{
    const string blockstr_values[5] = {"FUNCTION{}", "CLASS{}",
                                       "LOOP{}"
                                       "IF{}",
                                       "OTHER?{}"};
}

inline string to_string(BlockType btype)
{
    cout << "THIS?" << static_cast<int>(btype);
    return blocktype_values::blockstr_values[static_cast<int>(btype)];
}

class Liner;
class CodeStream
{

public:
    CodeStream(istream &src_input, string src_name);
    CodeStream(string file_name);
    ~CodeStream();

    void dod();

    bool nextLine();
    bool nextChar();
    bool isOpen();

    // private:
    StringLoc loc;
    string    cur_line;
    char      cur_char;

    stringstream statement;
    string       Last_statement;

    string   input_name;
    istream *input_p;
    fstream *finput_p;

    int       block_depth;
    BlockType block_type;

    int display_width = 30;

    fstream        *streamFile(string file_name);
    vector<Liner *> liners;
    Liner          *blanker;

    void foreach (void (*consumer_p)(Liner *));

    virtual void onChar();
    virtual void onBeginLine();
    virtual void onEndLine();
    virtual void onBeginStatement();
    virtual void onEndStatement();
    virtual void onBeginBlock();
    virtual void onEndBlock();
    virtual void onBegin();
    virtual void onEnd();
};

#endif
