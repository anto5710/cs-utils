#include <iostream>
#include <map>
#include <sstream>


using namespace std;

namespace rpn_opreator
{
    typedef int (*BinaryIntOperator)(int, int);

    enum OperatorType { UNIMPLEMENTED = -1, ADD = 0, MULTIPLY, DIVIDE, MOD };

    const map<string, OperatorType> operator_map = {
        {"+", ADD}, {"*", MULTIPLY}, {"/", DIVIDE}, {"mod", MOD}};

    OperatorType getOperator(string token)
    {
        return (operator_map.count(token) > 0 ? operator_map.at(token)
                                              : UNIMPLEMENTED);
    }


} // namespace rpn_opreator


map<string, rpn_opreator::BinaryIntOperator> op_map;

void switcher(string token)
{
    switch (rpn_opreator::getOperator(token)) {

    case rpn_opreator::ADD: {
        cout << "THIS IS ADD" << endl;
        break;
    }

    case rpn_opreator::MULTIPLY: {
        cout << "IT'S MULTIPLYING!" << endl;
        break;
    }

    case rpn_opreator::DIVIDE: {
        cout << "DIVIDE CONQUER GLUE SUBJUGATE" << endl;
        break;
    }

    case rpn_opreator::MOD: {
        cout << "MOD IS HERE" << endl;
        break;
    }

    case rpn_opreator::UNIMPLEMENTED:
    default: {
        cout << "FUC!" << endl;
        break;
    }
    };
}

int main()
{
    op_map["+"] = [](int a, int b) { return a + b; };
    op_map["/"] = [](int a, int b) { return a / b; };
    op_map["*"] = [](int a, int b) { return a * b; };
    op_map["%"] = [](int a, int b) { return a % b; };

    string token;
    while (cin >> token) {
        printf("MAP[%s] == enum[%d]\n", token.c_str(),
               rpn_opreator::getOperator(token));

        cout << "1 " << token << " 3 = " << op_map[token](1, 3) << endl;
    }

    return 0;
}