#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <chrono>
#include <iostream>
#include <stdio.h>
#include <thread>
#include "ColorStack.h"

using namespace std;

#ifndef PRINT_UTIL
#define PRINT_UTIL

inline void print_animate(ostream &output_p, string line,
                    int (*mills_delayer_p)(int))
{
    for (int i = 0; i < line.length(); i++) {
        output_p << line[i];

        int mills_delay = mills_delayer_p(i);

        if (mills_delay > 0) {
            this_thread::sleep_for(
               chrono::milliseconds(mills_delay));
        }
    }
}

inline string mcolor(string text, LinuxColorCode color_code)
{
    return to_string(color_code) + text + to_string(C_RESET);
}

inline void revert_line(bool clearLine)
{
    cout << "\r";
    if (clearLine) {
        cout << "\e[A";
    }
}

#endif