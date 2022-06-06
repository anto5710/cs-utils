#include "CodeStream.h"
#include "liners/Liner.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

CodeStream::CodeStream(istream &src_input, string src_name)
{
    input_p    = (&src_input);
    finput_p   = nullptr;
    input_name = src_name;
}

CodeStream::CodeStream(string file_name)
{
    finput_p   = streamFile(file_name);
    input_p    = finput_p;
    input_name = file_name;
}

CodeStream::~CodeStream()
{
    if (finput_p != nullptr) {
        finput_p->close();

        delete finput_p;
    }
}

fstream *CodeStream::streamFile(string file_name)
{
    fstream *file_stream_p = new fstream;
    file_stream_p->open(file_name);

    return file_stream_p;
}

bool CodeStream::nextLine()
{
    if (getline((*input_p), cur_line)) {
        loc.nextLine();
        return true;
    }

    return false;
}

bool CodeStream::nextChar()
{
    if (loc.col_no < (int)cur_line.length()) {

        cur_char = cur_line.at(loc.col_no);
        loc.nextChar();

        return true;
    }

    return false;
}

bool CodeStream::isOpen()
{
    return (finput_p == nullptr or finput_p->is_open());
}

void CodeStream::foreach (void (*consumer_p)(Liner *))
{
    for (Liner *liner : liners) {
        consumer_p(liner);
    }
}

void CodeStream::dod()
{
    onBegin();
    
    while (nextLine()) {

        onBeginLine();

        while (nextChar()) {
            onChar();

            if (cur_char == '{') {
                onBeginBlock();

            } else if (cur_char == '}') {
                onEndBlock();
            }
        }
        onEndLine();
    }
    onEnd();
}

void CodeStream::onBegin()
{
    foreach ([](Liner *l) { l->onBegin(); })
        ;
}
void CodeStream::onEnd()
{
    foreach ([](Liner *l) { l->onEnd(); })
        ;
}

void CodeStream::onChar()
{
    foreach ([](Liner *l) { l->onChar(); })
        ;
}

void CodeStream::onBeginLine()
{
    foreach ([](Liner *l) { l->onBeginLine(); })
        ;
}

void CodeStream::onEndLine()
{
    foreach ([](Liner *l) { l->onEndLine(); })
        ;
}

void CodeStream::onBeginStatement()
{
    foreach ([](Liner *l) { l->onBeginStatement(); })
        ;
}

void CodeStream::onEndStatement()
{
    foreach ([](Liner *l) { l->onEndStatement(); })
        ;
}

void CodeStream::onBeginBlock()
{
    foreach ([](Liner *l) { l->onBeginBlock(); })
        ;
}

void CodeStream::onEndBlock()
{
    foreach ([](Liner *l) { l->onEndBlock(); })
        ;
}