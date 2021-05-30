#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

#include "algraph.h"
#include "dfa.h"
#include "Exception.h"

using namespace std;

static DFA type1, type2, type3, type4, type5;
static ALGraph graph1, graph2, graph3, graph4, graph5;
static int ln;

void nexttoken(DFA &dfa, ALGraph &graph, string &s, int &k);
string typeofdfa(DFA *dfa);

void initdata() {
    builddgraph(graph1, type1.moves);
    builddgraph(graph2, type2.moves);
    builddgraph(graph3, type3.moves);
    builddgraph(graph4, type4.moves);
    builddgraph(graph5, type5.moves);
}

void handlestr(string s) {
    int k = 0;
    while (k < s.size()) {
        int oldk = k;
        nexttoken(type1, graph1, s, k);
        nexttoken(type2, graph2, s, k);
        nexttoken(type3, graph3, s, k);
        nexttoken(type4, graph4, s, k);
        nexttoken(type5, graph5, s, k);
        if (k == oldk) {
            throw Exception(s.substr(k, s.size() - k), ln);
            break;
        }
    }
}

bool acceptfinal(DFA &dfa, int state) {
    return find(dfa.final.begin(), dfa.final.end(), state) != dfa.final.end();
}

void nexttoken(DFA &dfa, ALGraph &graph, string &s, int &k) {
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

    if (k > oldk) {
        cout << typeofdfa(&dfa) << ": " << s.substr(oldk, k - oldk) << endl;
    }
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
