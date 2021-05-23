#include <iostream>
#include <set>

#include "algraph.h"
#include "dfa.h"

using namespace std;

void split(DFA &dfa, ALGraph &graph, vector<set<int>> &sets);
bool cansplit(DFA &dfa, ALGraph &graph, set<int> &st);
void initsets(DFA &dfa, vector<set<int>> &sets);
bool outofset(ALGraph &graph, set<int> &st, int si, char ch);

DFA *simplifydfa(DFA &dfa) {
    DFA *newdfa = new DFA();

    ALGraph graph;
    buildngraph(graph, dfa.moves);
    // orderngraph(graph);

    vector<set<int>> sets;
    initsets(dfa, sets);
    split(dfa, graph, sets);

    // for (int v : sets[0]) {
    //     for (auto it = dfa.sum.begin(); it != dfa.sum.end(); ++it) {
    //         if (outofset(graph, sets[0], v, *it)) {
    //             cout << v << endl;
    //         }
    //     }
    // }

    return newdfa;
}

void initsets(DFA &dfa, vector<set<int>> &sets) {
    set<int> start, final;  // 非终态集，终态集
    for (int &v : dfa.final) {
        final.insert(v);
    }
    for (int i = 0; i < dfa.states.size(); ++i) {
        if (final.find(i) == final.end()) {
            start.insert(i);
        }
    }
    sets.push_back(std::move(start));
    sets.push_back(std::move(final));
}

void orderset(set<int> &st) {
    for (auto it = st.begin(); it != st.end(); ++it) {
        if (it != st.begin()) {
            cout << " ";
        }
        cout << *it;
    }
    cout << endl;
}

void ordersets(vector<set<int>> &sets) {
    for (auto &st : sets) {
        orderset(st);
    }
}

void split(DFA &dfa, ALGraph &graph, vector<set<int>> &sets) {
    for (int i = 0; i < sets.size(); ++i) {
        set<int> &st = sets[i];
        if (cansplit(dfa, graph, st)) {
            set<int> st0, st1;

            for (int si : st) {
                for (auto it = dfa.sum.begin(); it != dfa.sum.end(); ++it) {
                    if (outofset(graph, st, si, *it)) {
                        st1.insert(si);
                    }
                }
            }

            for (int si : st) {
                if (st1.find(si) == st1.end()) {
                    st0.insert(si);
                }
            }

            sets.erase(sets.begin() + i);
            sets.insert(sets.begin() + i, std::move(st1));
            sets.insert(sets.begin() + i, std::move(st0));
            // ordersets(sets);
            split(dfa, graph, sets);
            break;
        }
    }
    // ordersets(sets);
}

bool cansplit(DFA &dfa, ALGraph &graph, set<int> &st) {
    if (st.size() <= 1) {
        cout << "<= 1" << endl;
        return false;
    }
    for (int v : st) {
        for (auto it = dfa.sum.begin(); it != dfa.sum.end(); ++it) {
            if (outofset(graph, st, v, *it)) {
                return true;
            }
        }
    }
    orderset(st);
    return false;
}

bool outofset(ALGraph &graph, set<int> &st, int si, char ch) {
    int sj = -1;
    if (si < graph.size()) {
        for (ArcNode &arc : graph[si]) {
            if (arc.val == ch) {
                sj = arc.adjvex;
                break;
            }
        }
    }
    return sj >= 0 && st.find(sj) == st.end();
}

int main(int argc, char const *argv[]) {
    DFA dfa;

    vector<Move> moves;
    moves.emplace_back(0, 1, 'a');
    moves.emplace_back(1, 2, 'b');
    moves.emplace_back(1, 3, 'c');
    moves.emplace_back(2, 2, 'b');
    moves.emplace_back(2, 3, 'c');
    moves.emplace_back(3, 2, 'b');
    moves.emplace_back(3, 3, 'c');

    dfa.states = vector<vector<int>>(4);
    dfa.sum.insert('a');
    dfa.sum.insert('b');
    dfa.sum.insert('c');
    dfa.moves = std::move(moves);
    dfa.s0 = 0;
    dfa.final = {1, 2, 3};

    ALGraph graph;
    buildngraph(graph, dfa.moves);
    // orderngraph(graph);

    // cout << "--------------------------" << endl;

    DFA dfa2;

    moves.clear();
    moves.emplace_back(0, 1, 'f');
    moves.emplace_back(1, 2, 'e');
    moves.emplace_back(1, 4, 'i');
    moves.emplace_back(2, 3, 'e');
    moves.emplace_back(4, 5, 'e');

    dfa2.states = vector<vector<int>>(6);
    dfa2.sum.insert('e');
    dfa2.sum.insert('f');
    dfa2.sum.insert('i');
    dfa2.moves = std::move(moves);
    dfa2.s0 = 0;
    dfa2.final = {3, 5};

    simplifydfa(dfa2);

    return 0;
}
