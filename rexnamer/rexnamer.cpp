#include <iostream>
#include <regex>

using namespace std;


namespace ANSIX
{
    const string BOLD = "\e[1m", RESET = "\e[m", ITAL = "\e[3m", WEAK = "\e[2m",
                 UNDERLINE = "\e[4m";

    const string F_GREEN = "\e[33m", F_RED = "\e[31m", F_YELLOW = "\e[32m";
};

using namespace ANSIX;


const string USAGE = "Usage: " + BOLD + UNDERLINE + "./renamer" + RESET +
                     " [regex-from] [regex-to]";
void abort_usage() {
    cerr << USAGE << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 33) {
        abort_usage();
    }

    return 0;
}