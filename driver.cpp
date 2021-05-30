#include <fstream>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

#include "algraph.h"
#include "dfa.h"

using namespace std;

static unordered_set<char> symbols;
static DFA type1, type2;
static ALGraph graph1, graph2;

int nexttoken(DFA &dfa, ALGraph &graph, string &s, int k);

void initdata() {
    builddgraph(graph1, type1.moves);
    builddgraph(graph2, type2.moves);

    // Sets data set
    ifstream symbolsin("symbols.txt");
    char symbol;
    while (symbolsin >> symbol) {
        symbols.insert(symbol);
    }
    symbolsin.close();
}

void handlestr(string s) {
    int k = 0;
    while (k < s.size() && k != -1) {
        int oldk = k;
        k = nexttoken(type1, graph1, s, k);
        if (k == -1) {
            k = nexttoken(type2, graph2, s, oldk);
        } else if (k < s.size()) {
            k = nexttoken(type2, graph2, s, k);
        }
    }
}

void driver_start() {
    initdata();

    ifstream in("input.txt");
    string line, s;

    while (getline(in, line)) {
        istringstream is(line);
        cout << line << endl;
        while (is >> s) {
            handlestr(s);
        }
        break;
    }

    in.close();
}

bool acceptfinal(DFA &dfa, int state) {
    return find(dfa.final.begin(), dfa.final.end(), state) != dfa.final.end();
}

string typeofdfa(DFA *dfa) {
    if (dfa == &type1) {
        return "Number";
    }
    if (dfa == &type2) {
        return "Variable";
    }
    return "unknown";
}

int nexttoken(DFA &dfa, ALGraph &graph, string &s, int k) {
    int state = 0;
    stack<int> stk;

    int beg = k;

    for (int i = k; i < s.size() && state != -1; ++i, ++k) {
        char c = s[i];
        if (symbols.find(c) != symbols.end()) {
            if (i == beg) {
                cout << "Symbol: " << c << endl;
                return i + 1;
            } else {
                break;
            }
        }
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

    if (k == s.size() || (k < s.size() && symbols.find(s[k]) != symbols.end())) {
        cout << typeofdfa(&dfa) << " : " << s.substr(beg, k - beg) << endl;
        return k;
    } else {
        if (k + 1 < s.size() && symbols.find(s[k + 1]) != symbols.end()) {
            cout << "Wrong : " << s.substr(beg, k - beg + 1) << endl;
            return k + 1;
        }
        cout << "Wrong : " << s.substr(beg, s.size() - beg) << endl;
        return -1;
    }
}
