#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>

#include "algraph.h"
#include "dfa.h"
#include "Exception.h"

using namespace std;

static DFA type1, type2, type3, type4, type5;
static ALGraph graph1, graph2, graph3, graph4, graph5;
static int ln;
static unordered_set<char> ops;

int nexttoken(DFA &dfa, ALGraph &graph, string &s, int k);
string typeofdfa(DFA *dfa);

void initdata() {
    builddgraph(graph1, type1.moves);
    builddgraph(graph2, type2.moves);
    builddgraph(graph3, type3.moves);
    builddgraph(graph4, type4.moves);
    builddgraph(graph5, type5.moves);

    ifstream in("ops.txt");
    char op;
    while (in >> op) {
        ops.insert(op);
    }
    in.close();
}

// 处理单独字符串的方法
void handlestr(string s) {
    int k = 0;
    while (k < s.size()) {
        int oldk = k;

        // 引入对非法变量名的处理
        k = nexttoken(type1, graph1, s, k);
        if (k > oldk && k < s.size()) {
            if (k == s.size() - 1) {
                cout << typeofdfa(&type1) << ": " << s.substr(oldk, k - oldk) << endl;
                continue;
            } else if (k < s.size() - 1 && ops.find(s[k]) != ops.end()) {
                cout << typeofdfa(&type1) << ": " << s.substr(oldk, k - oldk) << endl;
            } else {
                throw Exception(s.substr(oldk, s.size() - oldk), ln);
                break;
            }
        }

        k = nexttoken(type2, graph2, s, k);
        k = nexttoken(type3, graph3, s, k);

        // 选择 KeyWord 与 Variable 中匹配较长的一个
        int k4 = nexttoken(type4, graph4, s, k);
        int k5 = nexttoken(type5, graph5, s, k);
        if (k5 > k4) {
            cout << typeofdfa(&type5) << ": " << s.substr(k, k5 - k) << endl;
            k = k5;
        } else if (k4 > k) {
            cout << typeofdfa(&type4) << ": " << s.substr(k, k4 - k) << endl;
            k = k4;
        }

        // 如果所有规则都无法匹配，抛出自定义异常
        if (k == oldk) {
            throw Exception(s.substr(k, s.size() - k), ln);
            break;
        }
    }
}

bool acceptfinal(DFA &dfa, int state) {
    return find(dfa.final.begin(), dfa.final.end(), state) != dfa.final.end();
}

// 对于 Relation 与 KeySymbol，识别后直接打印
int nexttoken(DFA &dfa, ALGraph &graph, string &s, int k) {
    int state = 0;
    int oldk = k;
    stack<int> stk;

    for (; k < s.size() && state != -1; ++k) {
        char c = s[k];
        if (acceptfinal(dfa, state)) {
            while (!stk.empty()) {
                stk.pop();
            }
        }
        stk.push(state);
        state = targetdgraph(graph, state, c);
    }

    while (!stk.empty() && !acceptfinal(dfa, state)) {
        state = stk.top();
        stk.pop();
        --k;
    }

    if ((&dfa == &type2 || &dfa == &type3) && k > oldk) {
        cout << typeofdfa(&dfa) << ": " << s.substr(oldk, k - oldk) << endl;
    }

    return k;
}

string typeofdfa(DFA *dfa) {
    if (dfa == &type1) {
        return "Number   ";
    }
    if (dfa == &type2) {
        return "Relation ";
    }
    if (dfa == &type3) {
        return "KeySymbol";
    }
    if (dfa == &type4) {
        return "KeyWord  ";
    }
    if (dfa == &type5) {
        return "Variable ";
    }
    return "Unknown  ";
}

// 读入 input.c 文件，并对抛出的异常进行处理
void startdriver() {
    initdata();

    ifstream in("input.c");
    string line, s;

    ln = 1;
    while (getline(in, line)) {
        istringstream is(line);
        while (is >> s) {
            try {
                handlestr(s);
            } catch (Exception &e) {
                e.showMessage();
                exit(1);
            } catch (...) {
                cout << "uncaught unknown exception" << endl;
            }
        }
        ++ln;
    }

    in.close();
}
