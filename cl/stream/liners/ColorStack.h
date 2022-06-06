#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifndef COLOR_STRING_STREAM
#define COLOR_STRING_STREAM

#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;

enum LinuxColorCode {
    C_RESET,
    C_BLACK,
    C_RED,
    C_GREEN,
    C_BROWN,
    C_BLUE,
    C_PURPLE,
    C_CYAN,
    C_LIGHT_GRAY,
    C_DARK_GRAY,
    C_LIGHT_RED,
    C_LIGHT_GREEN,
    C_YELLOW,
    C_LIGHT_BLUE,
    C_PINK,
    C_LIGHT_CYAN,
    C_WHITE,
    BG_RED,
    BG_GREEN,
    BG_BROWN,
    BG_BLUE,
    BG_PURPLE,
    BG_CYAN,
    BG_GRAy,
    ST_BOLD,
    ST_ITALIC,
    ST_UNDERLINE,
    ST_BLINK,
    ST_BLACK_OUTLINE,
    ST_REVERSE,
    ST_NON_DISPLAY,
    ST_STRIKE
};

namespace color_codes
{
const string colorstr_values[33] = {
    "\e[m",     "\e[0;30m", "\e[0;31m", "\e[0;32m", "\e[0;33m", "\e[0;34m",
    "\e[0;35m", "\e[0;36m", "\e[0;37m", "\e[1;30m", "\e[1;31m", "\e[1;32m",
    "\e[1;33m", "\e[1;34m", "\e[1;35m", "\e[1;36m", "\e[1;37m", "\e[0;41m",
    "\e[0;42m", "\e[0;43m", "\e[0;44m", "\e[0;45m", "\e[0;46m", "\e[0;47m",
    "\e[1m",    "\e[3m",    "\e[4m",    "\e[5m",    "\e[6m",    "\e[7m",
    "\e[8m",    "\e[9m"};

}

inline string to_string(LinuxColorCode color_code)
{
    return color_codes::colorstr_values[static_cast<int>(color_code)];
}

class ColorStack
{
public:
    operator std::string() { return str(); }

    inline string miniColor(string text, LinuxColorCode color_code)
    {
        return apply(color_code) + text + undo();
    }

    inline string apply(const LinuxColorCode &color_code)
    {
        color_stack.push_back(color_code);
        return to_string(color_code);
    }

    inline string undo()
    {
        color_stack.pop_back();
        return str();
    }

    inline string str()
    {
        stringstream csstack(to_string(C_RESET));

        for (LinuxColorCode code : color_stack) {
            csstack << to_string(code);
        }

        return csstack.str();
    }

    inline string clear()
    {
        color_stack.clear();
        return apply(C_RESET);
    }

private:
    std::vector<LinuxColorCode> color_stack;
};


#endif