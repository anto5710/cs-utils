#ifndef LINER
#define LINER

#include "../CodeStream.h"
#include "../StringLoc.h"
#include <list>

class CodeStream;
class Liner
{
public:
    CodeStream  *cs_p;
    list<string> err_stack;

    Liner(CodeStream &cstream);
    Liner();

    virtual void onBegin();

    virtual void onChar();

    virtual void onBeginLine();
    virtual void onEndLine();

    virtual void onBeginStatement();
    virtual void onEndStatement();

    virtual void onBeginBlock();
    virtual void onEndBlock();

    virtual void onEnd();

    char      curChar() const;
    string    curLine() const;
    StringLoc curLoc() const;

    string summary();
};


#endif