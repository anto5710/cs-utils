
#include <iostream>
#include <map>

using namespace std;

typedef void (*VOID_FUNC)(void);
const string TERMINATOR = ";";

struct CommandNode {
    string                     key;
    VOID_FUNC                  action;
    map<string, CommandNode *> children;

    CommandNode() : CommandNode(""){};
    CommandNode(string key) : CommandNode(key, nullptr){};
    CommandNode(string key, VOID_FUNC action)
    {
        this->key    = key;
        this->action = action;
    }

    void addSequence(string *args, VOID_FUNC action)
    {
        string head = args[0];

        if (head == TERMINATOR) {
            this->action = action;

        } else {
            getChild(head)->addSequence(args + 1, action);
        }
    }

    CommandNode *getChild(string key)
    {
        if (not hasKey(key)) {
            addChild(new CommandNode(key));
        }

        return children[key];
    }

    void addChild(CommandNode *child) { children[child->key] = child; }

    bool hasKey(string key) { return children.count(key) > 0; }

    void run(string *cmds)
    {
        string head = cmds[0];

        if (head == TERMINATOR) {
            action();

        } else if (hasKey(head)) {
            children[head]->run(cmds + 1);

        } else {
            cerr << "Invalid subcommand: " << key << "::" << head << endl;
        }
    }

    void printChildren()
    {
        for (const auto &myPair : children) {
            cout << myPair.first << " ";
        }
        
        cout << endl;
    }
};

void printHello()
{
    cout << "A: Hello!" << endl;
}

void printDod()
{
    cout << "Dod::Do: Hello!" << endl;
}

int main()
{
    CommandNode master;

    string seq[] = {"A", ";"};
    master.addSequence(seq, &printHello);

    string seq2[] = {"Dod", "Do", ";"};
    master.addSequence(seq2, &printDod);

    string seq3[] = {"Dod", "do", ";"};
    master.addSequence(seq3, &printDod);

    string arg;
    string buckets[10];
    int    i = 0;

    while (cin >> arg) {
        buckets[i++] = arg;

        if (arg == TERMINATOR) {
            master.run(buckets);
            i = 0;

        } else if (arg == "?") {
            master.printChildren();
            i--;
        }
    }

    return 0;
}
