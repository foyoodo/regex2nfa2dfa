#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

#include "algraph.h"
#include "dfa.h"

using namespace std;

bool nexttoken(DFA &dfa, string &s, int &k);

int main(int argc, char const *argv[]) {
    DFA dfa;

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

    ifstream in("input.txt");
    string line, s;

    while (getline(in, line)) {
        istringstream is(line);
        cout << line << endl;
        while (is >> s) {
            int k = 0;
            while (k < s.size()) {
                int beg = k;
                if (nexttoken(dfa, s, k)) {
                    cout << s.substr(beg, k - beg) << endl;
                }
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

bool nexttoken(DFA &dfa, string &s, int &k) {
    ALGraph graph;
    builddgraph(graph, dfa.moves);

    int oldk = k;

    int state = 0;
    stack<int> stk;

    do {
        if (k >= s.size()) {
            break;
        }
        char c = s[k++];
        if (acceptfinal(dfa, state)) {
            while (!stk.empty()) {
                stk.pop();
            }
        }
        stk.push(state);
        state = targetdgraph(graph, state, c);
    } while (state != -1);

    while (!stk.empty() && !acceptfinal(dfa, state)) {
        state = stk.top();
        stk.pop();
        --k;
    }

    if (oldk == k) {
        ++k;
        return false;
    }
    return true;
}
