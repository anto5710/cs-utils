#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Liner.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <list>
#include <sstream>
#include <thread>
// #include <list>
#include "../CodeStream.h"
#include "ColorStack.h"
#include "PrintUtil.h"

using namespace std;

class ColLiner : public Liner
{
    using Liner::Liner;
    list<int> col_lens;

    // double *col_lens;

    ColorStack   cstack;
    stringstream header;

    string repeat(int c, string s)
    {
        string ss = "";
        for (int i = 0; i < c; i++) {
            ss += s;
        }
        return ss;
    }


    virtual void onBegin()
    {
        // col_lens = new double[cs_p->display_width];
        header << cstack.apply(BG_GRAy);
        header << cstack.apply(C_BLACK) << repeat(cs_p->display_width * 2, "=")
               << cstack.clear();
        // header << cstack.apply(BG_GREEN) << cstack.apply(C_BLACK) <<
        // cs_p->input_name << cstack.undo()  << cstack.undo()+ "\n";
    }

    virtual void onEndLine()
    {
        if (cs_p->cur_line.length() > 80) {
            stringstream ss;
            ColorStack   ccs;
            string leftl = curLine().substr(curLine().find_first_not_of(" \t"),
                                            curLine().length());

            
            string ll =
                ccs.miniColor(
                    ccs.miniColor(cs_p->input_name, ST_UNDERLINE) + ":" +
                        ccs.miniColor(to_string(curLoc().line_no), C_LIGHT_RED),
                    ST_BOLD) +
                ccs.miniColor(" line has over 80 characters", C_WHITE);


            // cout << leftl << endl ;

            int maxd = min(abs((int)(curLine().length()) - 80), 6);

            string fll =
                leftl.empty()
                    ? ""
                    : (leftl.substr(0, cs_p->display_width - maxd - 3) +

                       ccs.miniColor(
                           "..." +
                               leftl.substr(leftl.length() - maxd - 1, maxd),
                           C_RED));

            string fll_un =
                leftl.empty()
                    ? ""
                    : ((leftl.substr(0, cs_p->display_width - maxd - 3) +

                        ("..." +
                         leftl.substr(leftl.length() - maxd - 1, maxd))));

            string coll = " " + to_string(cs_p->cur_line.length()) + " chars ";

            string coll_cl = ccs.miniColor(coll, C_LIGHT_RED);

            string fll_under =
                ccs.miniColor(
                    "└" + repeat((fll_un.length() - 2 - coll.length()) / 2 + 1,
                                 "─"),
                    C_RED) +
                coll_cl +
                ccs.miniColor(
                    repeat((fll_un.length() - 2 - coll.length()) / 2, "─") +
                        "┘",
                    C_RED);
            ccs.apply(C_RESET);

            // cout << fll << endl;
            string pad = "               ";

            ss << ll << endl
               << pad << fll << endl
               << pad << fll_under << endl
               << endl;


            err_stack.push_back(ss.str());

            //    cout << cstack.miniColor("SAUL GOOD
            //    MAN", C_GREEN) << endl;
            // cout << miniColor("GOOD", C_GREEN) <<
            // endl;

            // css << miniColor("Good", C_GREEN) <<
            // endl;
            //      << miniColor(string(curLoc()),
            //      C_YELLOW)
            //      << miniColor(curLine().substr(0,
            //      30), C_BLUE) << endl;
            // cout << "GREEEEEEEEEEEEEEEEEN" << endl;
            // css << css.miniColor("GREEN GUY",
            // C_GREEN) << endl;


            //     cout << colorize(string(curLoc()),
            //     C_YELLOW)
            //          << colorize(curLine().substr(0,
            //          30), C_BLUE)
            //          <<
            //          colorize(curLine().substr(31,
            //          60) + " ... ", C_RED)
            //          << endl;


            //     stringstream ss;

            //     ss << str_color(BG_GRAy) <<
            //     str_color(C_LIGHT_GRAY)
            //        << str_color(ST_UNDERLINE) <<
            //        "This.Good " <<
            //        str_color(C_RESET)
            //        << str_color(BG_GRAy)
            //        <<
            //        colorize("▅▂▃▅▆▇▇▆▅▅▆▅▅▆▆▇▆▅▅▆▅▅▇▆▅▅▆▅▅▆▆▇▇▆▇☑
            //        ", C_GREEN)
            //        << str_color(C_RESET) << endl
            //        << str_color(BG_GRAy) <<
            //        str_color(C_LIGHT_GRAY)
            //        << str_color(ST_UNDERLINE) <<
            //        "This.Good" << str_color(C_RESET)
            //        << " " << str_color(BG_GRAy)
            //        <<
            //        colorize("▅▂▃▅▆▇▇▆▅▅▆▅▅▆▆▇▆▅▅▆▅▅▇▆▅▅▆▅▅▆▆▇▇▆▇☑
            //        ", C_RED)
            //        << str_color(C_RESET) << endl
            //        << str_color(BG_GRAy) <<
            //        str_color(C_LIGHT_GRAY)
            //        << str_color(ST_UNDERLINE) <<
            //        "This.Good " <<
            //        str_color(C_RESET)
            //        << str_color(BG_GRAy)
            //        <<
            //        colorize("▅▂▃▅▆▇▇▆▅▅▆▅▅▆▆▇▆▅▅▆▅▅▇▆▅▅▆▅▅▆▆▇▇▆▇☑
            //        ", C_GREEN)
            //        << str_color(BG_GRAy) <<
            //        str_color(C_RESET) << endl;


            //     double ii = 0;
            //     for (char c : ss.str()) {
            //         if (c == '\n') {
            //             ii = 0;
            //         }
            //         cout << c << flush;
            //         std::this_thread::sleep_for(
            //             std::chrono::milliseconds(max(1,
            //             2 - int(pow(ii++, 0.3)))));
            //         // nanosleep(1, 0);
            //     }
            // }
        }

        col_lens.push_back(curLine().length());
    }

    void onEnd()
    {

        cout << header.str() << endl;
        cout << "\r";

        // print_animate(cout, "▅▂▃▅▆▇▇▆▅▅▆▅▅▆▆▇▆▅▅▆▅▅▇▆▅▅▆▅▅▆▆",
        //               [](int i) { return 1*(12 - int(pow(i, 0.3))); });
        double ii = 0;

        ColorStack css;

        cout << css.apply(C_GREEN);
        cout << css.miniColor(
            css.miniColor(cs_p->input_name, ST_UNDERLINE) + " ☐  ", C_WHITE);


        for (char c :
             "▅▂▃▅▆▅▆▇▇▆▅▅▆▅▅▆▆▇▅▅▂▃▅▆▅▆▇▇▆▅▅▆▅▅▆▆▇▅▅▇▆▅▅▆▅▅▅▇▆▅▅▆▅▅▆▆") {
            if (c == '\n') {
                ii = 0;
            }
            cout << c << flush;

            int d = 1;
            if (to_string(c) == "▃") {
                d = 2;
            } else if (to_string(c) == "▅") {
                d = 2;
            } else if (to_string(c) == "▆") {
                d = 4;
            } else {
                d = 5;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(d));
            ii++;
            if (ii == 37) {

                revert_line(true);

                for (string err : err_stack) {
                    cout << err;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));

                    // print_animate(cout, err, [](int i){return 1;});
                }

                for (string err : cs_p->blanker->err_stack) {
                    cout << err;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    //     print_animate(cout, err, [](int i){return 1;});
                }

                cout << endl;
                // break;
                cout << css.apply(C_RED);
                // cout << "\r";
                cout << css.miniColor(
                    css.miniColor(cs_p->input_name, ST_STRIKE) + " ☒  ", C_RED);
                cout << css.miniColor("▅▂▃▅▆▅▆▇▇▆▅▅▆▅▅▆▆▇▅▅▂▃▅▆▅▆▇▇▆▅▅▆▅▅▆▆",
                                      C_GREEN)
                     << "▅" << css.undo();
            }
            // nanosleep(1, 0);
        }


        cout << css.undo() << endl << endl;
        // cout << endl;
        // string ddd = "▂▃▅▆▇";

        // int cols[cs_p->display_width];
        // int msx = col_lens.size();
        // ;
        // int max_col = 0;

        // for (int i = 0; i < msx; i++) {
        //     cols[i * cs_p->display_width / (msx)] =
        //     (cols[i * cs_p->display_width / (msx)]) * i %
        //     (cs_p->display_width / (msx)) + col_lens.front();

        //     max_col = max(max_col, col_lens.front());
        //     col_lens.pop_front();
        // }

        // for (int i = 0; i < cs_p->display_width; i++) {
        //     cout << cols[i] << "," << endl;
        //     // cout << (5.0 * cols[i] / max_col / (cs_p->display_width *
        //     (msx)))
        //         //  << endl;
        //     // cout << ddd[];
        // }


        // delete [] col_lens;
    }
};