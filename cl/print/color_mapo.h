#include <iostream>

class Saturator
{
public:
    Saturator(std::string targetW)
    {
        target  = targetW;
        sat_max = targetW.length();
    }

    bool peek(char c)
    {
        return ((sat_stack + 1) >= sat_max) or (c == target[sat_stack + 1]);
    }

    bool saturated() { return sat_stack >= sat_max; }

    bool feed(char c)
    {
        if (peek(c)) {
            sat_stack++;
            if (saturated()) {
                reset();
                return true;
            }
        } else {
            reset();
        }
        return false;
    }

private:
    void reset() { sat_stack = -1; }

    std::string target;
    int         sat_max;
    int         sat_stack;
};