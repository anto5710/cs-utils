/*
 *  codelen.cpp
 *  Author: Chris Ahn (dahn01)
 *    Date: Feb. 2, 2022
 *
 *
 * For given text input, asserts maxColumn <= 80, and {}blockline <= 30
 */


#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>

using namespace std;

class CodeStream
{
public:
    CodeStream(istream &input_ref, string name);

    bool      nextLine();
    bool      assertLine();
    bool      assertAll();
    int       output_mode;
    const int MAX_COLUMNS      = 80;
    const int MAX_BLOCK_HEIGHT = 30;

private:
    const int     BLOCKS_CLEARED = -1;
    const int     PRINT_WIDTH    = 44;
    queue<string> error_stack;

    string line;
    int    line_no, block_depth, latest_opener_line;
    int    column_failcnt, height_failcnt;

    istream *input_p;
    string   input_name;

    bool   assertColumn();
    bool   assertHeight();
    bool   assertBlock();
    void   onBlockOpen();
    bool   onBlockClose();
    void   printHeader();
    void   printSummary();
    string center(string text, int width);
};

bool assertSource(istream &input, string input_name);
bool assertValidLine(int line_no, string line, string input_name);
bool assertValidBlock(int *last_opener_p, int line_no, string line,
                      string input_name);


const string SILENT_SWITCH = "-s";
const int    SILENT_MODE   = -1;
const int    COMPACT_MODE  = 0;

int main(int argc, char *argv[])
{
    if (argc == 3 and argv[1] == SILENT_SWITCH) {}
    if (argc == 2) {
        if (argv[1] == SILENT_SWITCH) {

        } else {

            ifstream file_input;
            string   file_name = argv[1];
            file_input.open(file_name);

            CodeStream(file_input, file_name).assertAll();


            file_input.close();
        }

    } else if (argc == 1) {
        CodeStream(cin, "cin").assertAll();

    } else {
        cerr << "coldelen: Usage: ./codelen (-s) <FILE_NAME>" << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}

CodeStream::CodeStream(istream &input_ref, string name)
{
    if (not input_ref.good()) {
        throw runtime_error("CodeStream: invalid input stream: " + input_name);
    }

    line_no     = 0;
    block_depth = 0;

    column_failcnt = 0;
    height_failcnt = 0;

    input_name = name;
    input_p    = &input_ref;
}


const size_t TAB_SIZE = 8;

bool CodeStream::assertColumn()
{

    size_t len = line.length();
    for (char c : line) {
        if (c == '\t') {
            len += (TAB_SIZE - 1);
        }
    }

    if (len > MAX_COLUMNS) {

        size_t starti       = line.find_first_not_of(" \n\r\t\f\v");
        string trimmed_line = (starti == std::string::npos)
                                  ? ""
                                  : line.substr(starti, starti + PRINT_WIDTH);

        ostringstream outs;
        outs << " \e[1mline:" << line_no << "\e[22;39;2m: "
             << "\t(\e[22;1;31m" << len << "\e[22;39m cols):\e[2m\t"
             << trimmed_line << "[...]"
             << "\e[22m" << endl;

        error_stack.push(outs.str());

        column_failcnt++;
        return false;
    }

    return true;
}

bool CodeStream::assertHeight()
{
    stringstream sstream(line);
    bool         result = true;

    char c;
    while (sstream >> c) {
        if (c == '{') {
            onBlockOpen();

        } else if (c == '}') {
            result &= onBlockClose();
        }
    }

    if (not result and block_depth == 0) {
        height_failcnt++;
    }

    return result;
}

void CodeStream::onBlockOpen()
{
    block_depth++;

    if (block_depth == 1) {
        latest_opener_line = line_no;
    }
}

bool CodeStream::onBlockClose()
{
    block_depth--;
    bool result = true;

    if (block_depth == 0) {
        result             = assertBlock();
        latest_opener_line = BLOCKS_CLEARED;
    }

    return result;
}

bool CodeStream::assertBlock()
{
    int block_height = (line_no - latest_opener_line - 1);

    if ((latest_opener_line != BLOCKS_CLEARED) and
        (block_height > MAX_BLOCK_HEIGHT)) {

        ostringstream outs;

        outs << " \e[1mline"
             << ":" << (latest_opener_line + 1) << "\e[22m-\e[1m"
             << (line_no - 1) << "\e[22m: (\e[33;1m" << block_height
             << "\e[39;22m lines)" << endl;

        error_stack.push(outs.str());

        return false;
    }

    return true;
}

#include <stdlib.h>

void CodeStream::printHeader()
{
    // string hline = string(PRINT_WIDTH, '=');
}

void CodeStream::printSummary()
{
    // string hline  = string(PRINT_WIDTH, '=');
    string hhline = string(PRINT_WIDTH, '-');

    cerr << setw(10);

    // if (output_mode != SILENT_MODE) {
    if (height_failcnt > 0 or column_failcnt > 0) {
        string warn_color = column_failcnt > 0 ? "31" : "33";

        cerr << endl
             << ("\e[22;1;4;" + warn_color + "m" + input_name +
                 "\e[24m ☒ \e[39;22m")
             << endl;
        // cout << "\e[31m";
        cerr << hhline << endl;

        while (not error_stack.empty()) {
            cerr << error_stack.front();
            error_stack.pop();
        }

        if (column_failcnt > 0) {
            string column_report =
                (column_failcnt == 0
                     ? "SAFE"
                     : (to_string(column_failcnt) + " FAILED!"));

            cerr << "\e[1;"
                 << "m"
                 << " COLUMN WIDTH (<= " << MAX_COLUMNS
                 << " cols): " << column_report << "\e[22;39m" << endl;
            cout << "[FAIL_COLUMN]";
        }


        if (height_failcnt > 0) {
            string height_report =
                (height_failcnt == 0
                     ? "SAFE"
                     : (to_string(height_failcnt) + " FAILED!"));

            cerr << "\e[1;"
                 << "m"
                 << " BLOCK HEIGHT (<= " << MAX_BLOCK_HEIGHT
                 << " lines): " << height_report << "\e[22;39m" << endl;
            cout << "[FAIL_HEIGHT]";
        }
    } else {
        // string msg = "[SAFE]";
        cout << ("\e[4;1;32m" + input_name + "\e[24m ☑ \e[39;22m") << "\t";

        // cout << center(msg, PRINT_WIDTH) << endl;
    }
}

string CodeStream::center(string text, int width)
{
    int    text_length    = text.length();
    int    left_pad_width = max(0, (width - text_length) / 2);
    string left_pad       = string(left_pad_width, ' ');

    return (left_pad + text);
}

bool CodeStream::nextLine()
{
    getline(*input_p, line);
    line_no++;
    return input_p->good();
}

bool CodeStream::assertLine()
{
    return assertColumn() and assertHeight();
}

bool CodeStream::assertAll()
{
    printHeader();

    while (nextLine()) {
        assertLine();
    }

    printSummary();

    return height_failcnt == 0 and column_failcnt == 0;
}