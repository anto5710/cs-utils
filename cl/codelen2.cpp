#include <fstream>
#include <iostream>

using namespace std;

class CodeStream
{
public:
    CodeStream(istream &input);
    CodeStream(string filename);
    ~CodeStream();

private:
    istream *input;
};

CodeStream::CodeStream(istream &input)
{
    this->input = &input;
}

fstream *streamFile(string filename)
{
    fstream *finput_p = new fstream;
    finput_p->open(filename);

    if (){
        if (true)
            cout << "d";
        else
            cout << "D";
    } else if (sd) {

    } else {} for (int i = 0; i < 2; i++) {
                 sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss111111111112
    }


























    return finput_p;
}


int main()
{
    CodeStream s("tester.cpp");

    return 0;
}