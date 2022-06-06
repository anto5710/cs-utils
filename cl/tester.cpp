#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <cstdlib>
#include <fstream>
#include <iostream>
// #include <system>
using namespace std;

#include "stream/CodeStream.h"
#include "stream/liners/ColLiner.h"
#include "stream/liners/Liner.h"
#include "stream/liners/IndentLiner.h"
#include "stream/liners/StatementLiner.h"
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void dodd(int argc, char *argv[])
{
    if (argc <= 1) {
        throw runtime_error("NO ARG!");
    }

    CodeStream       s(argv[1]);
    ColLiner         liner(s);
    BlockHeightLiner ss(s);
    IndentLiner      dd(s);
    StatementLiner   st(s);

    s.liners.push_back(&liner);
    s.liners.push_back(&ss);
    s.liners.push_back(&dd);
    s.liners.push_back(&st);

    s.blanker = &ss;

    // cout << (s.isOpen() ? "T" : "F") << endl;
    s.dod();
}

int main(int argc, char *argv[])
{
    // std::thread t1(dodd, argc, argv);
    dodd(argc, argv);
    // cout << "HI>" << endl;
    // t1.join();
    // cout << "END>>" << endl;

    return 0;
}

