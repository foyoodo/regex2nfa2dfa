#include <fstream>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

#include "algraph.h"
#include "dfa.h"

using namespace std;

unordered_set<char> symbols;
DFA dfa, dfa2;
ALGraph graph, graph2;

int nexttoken(string &s, int k);

void initdata() {
    // a(b|c)*
    vector<Move> moves;
    moves.emplace_back(0, 1, new set<char>{'a'});
    moves.emplace_back(1, 1, new set<char>{'b', 'c'});

    dfa.states = vector<vector<int>>(2);
    dfa.sum.insert('a');
    dfa.sum.insert('b');
    dfa.sum.insert('c');
    dfa.moves = std::move(moves);
    dfa.s0 = 0;
    dfa.final = {1};

    builddgraph(graph, dfa.moves);

    // f(ee|ie)
    moves.clear();
    moves.emplace_back(0, 1, new set<char>{'f'});
    moves.emplace_back(1, 2, new set<char>{'e', 'i'});
    moves.emplace_back(2, 3, new set<char>{'e'});

    dfa2.states = vector<vector<int>>(4);
    dfa2.sum.insert('e');
    dfa2.sum.insert('f');
    dfa2.sum.insert('i');
    dfa2.moves = std::move(moves);
    dfa2.s0 = 0;
    dfa2.final = {3};

    builddgraph(graph2, dfa2.moves);

    // Sets data set
    ifstream symbolsin("symbols.txt");
    char symbol;
    while (symbolsin >> symbol) {
        symbols.insert(symbol);
    }
    symbolsin.close();
}

int main(int argc, char const *argv[]) {
    initdata();

    ifstream in("input.txt");
    string line, s;

    while (getline(in, line)) {
        istringstream is(line);
        cout << line << endl;
        while (is >> s) {
            int k = 0;
            while (k < s.size() && k != -1) {
                k = nexttoken(s, k);
            }
        }
        break;
    }

    in.close();
    return 0;
}

bool acceptfinal(DFA &dfa, int state) {
    return find(dfa.final.begin(), dfa.final.end(), state) != dfa.final.end();
}

int nexttoken(string &s, int k) {
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
        cout << "Right : " << s.substr(beg, k - beg) << endl;
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
