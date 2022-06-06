
#include <iostream>
#include <list>
#include <sstream>

using namespace std;


class BufferedTokenTree
{
public:
    list<BufferedTokenTree *> children;
    char                      data;

    inline BufferedTokenTree(char val) { data = val; }

    inline ~BufferedTokenTree()
    {
        for (BufferedTokenTree *child : children) {
            delete child;
        }
    }


    inline BufferedTokenTree &child(char c)
    {
        for (BufferedTokenTree *child : children) {
            if (charis(child, c)) {
                return *child;
            }
        }

        BufferedTokenTree *newchild = new BufferedTokenTree(c);
        children.push_back(newchild);

        return *newchild;
    }

    inline void add(string vals)
    {
        if (not vals.empty()) {
            child(vals.front()).add(vals.substr(1));
        }
    }

    inline bool charis(BufferedTokenTree *node, char c)
    {
        return node != nullptr and node->data == c;
    }

    inline string succread(istream &in, BufferedTokenTree *node)
    {
        if (node == nullptr || in.peek() != node->data) {
            return "";
        }

        char c;
        in >> c;
        string maxer;

        for (BufferedTokenTree *child : children) {
            string temp = succread(in, child);

            if (child == nullptr || temp != "") {

            }
            // if (temp.length() > maxer.length()) {
            //     maxer = string(1, c) + temp;
            // }
        }

        return maxer;
    }
};

string toString(BufferedTokenTree *node)
{
    if (node == nullptr) {
        return "0";
    }

    string sumstr = string(1, node->data) + "-> {";

    int i = 0;

    for (BufferedTokenTree *child : node->children) {
        sumstr += toString(child);
        if (i < node->children.size() - 1) {
            sumstr += ", ";
        }
        i++;
    }

    return sumstr + "}";
}


// BufferedTokenTree *tokener(std::string vals)
// {
//     if (vals.empty()) {
//         return nullptr;
//     }

//     BufferedTokenTree *subroot = new BufferedTokenTree(vals.front());
//     subroot->add(vals.substr(1));

//     return subroot;
// }

int main()
{
    BufferedTokenTree *d = new BufferedTokenTree('a');

    d->add("bc");
    d->add("cd");
    d->add("cc");
    d->add("bcc");
    d->add("bccc");

    
    cout << toString(d) << endl;
    istringstream ins("asdds");
    string cs ="d";

    string inp;
    while (cin >> inp) {
        ins.str(inp);
        cout << "READ?: " << d->succread(ins, d) << endl;
    }

    delete d;
    return 0;
}
