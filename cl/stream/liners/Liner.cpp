#include "Liner.h"
#include "../CodeStream.h"

using namespace std;

Liner::Liner(CodeStream &cstream)
{
    cs_p = (&cstream);
}

Liner::Liner()
{
    cs_p = nullptr;
}

void Liner::onChar(){};
void Liner::onBeginLine(){};
void Liner::onEndLine(){};
void Liner::onBeginStatement(){};
void Liner::onEndStatement(){};
void Liner::onBeginBlock(){};
void Liner::onEndBlock(){};

void Liner::onBegin(){};

void Liner::onEnd(){
    // for (string line : err_stack)
    // {
    //     cout << line;
    // }    
};

char Liner::curChar() const
{
    return cs_p->cur_char;
}

string Liner::curLine() const
{
    return cs_p->cur_line;
}

StringLoc Liner::curLoc() const
{
    return cs_p->loc;
}