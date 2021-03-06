#include <iostream>
#include <set>

#include "algraph.h"
#include "dfa.h"

using namespace std;

int outofset(ALGraph &graph, set<int> &st, int si, char ch);
bool cansplit(DFA &dfa, ALGraph &graph, set<int> &st);
void initsets(DFA &dfa, vector<set<int>> &sets);
void orderset(set<int> &st);
void ordersets(vector<set<int>> &sets);
void split(DFA &dfa, ALGraph &graph, vector<set<int>> &sets);

// 递归深度
int deep = 0;

DFA &simplifydfa(DFA &dfa) {
    DFA *newdfa = new DFA();

    ALGraph graph;
    buildngraph(graph, dfa.moves);

    vector<set<int>> sets;
    initsets(dfa, sets);
    split(dfa, graph, sets);
    deep = 0;

    cout << ">>> ordersets" << endl;
    ordersets(sets);
    cout << ">>> cansplit" << endl;
    for (auto &st : sets) {
        if (cansplit(dfa, graph, st)) {
            orderset(st);
        }
    }

    // for (auto it = sets.begin(); it != sets.end(); ++it) {
    //     vector<int> state;
    //     for (int v : *it) {
    //         state.push_back(v);
    //     }
    //     newdfa->states.push_back(std::move(state));
    // }

    // newdfa->sum = dfa.sum;
    // newdfa->s0 = 0;

    return *newdfa;
}

// 初始化 sets，按（非）终态集进行切分
void initsets(DFA &dfa, vector<set<int>> &sets) {
    set<int> start, final;
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

// 遍历 set
void orderset(set<int> &st) {
    cout << "{ ";
    for (auto it = st.begin(); it != st.end(); ++it) {
        if (it != st.begin()) {
            cout << ", ";
        }
        cout << *it;
    }
    cout << " }" << endl;
}

// 遍历 sets
void ordersets(vector<set<int>> &sets) {
    for (auto &st : sets) {
        orderset(st);
    }
}

void split(DFA &dfa, ALGraph &graph, vector<set<int>> &sets) {
    if (++deep >= 10) {
        return;
    }
    for (int i = 0; i < sets.size(); ++i) {
        set<int> &st = sets[i];
        if (cansplit(dfa, graph, st)) {
            set<int> st0, st1;
            for (int si : st) {
                for (auto it = dfa.sum.begin(); it != dfa.sum.end(); ++it) {
                    int ret = outofset(graph, st, si, *it);
                    if (ret == 1 || ret == -2) {
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
            split(dfa, graph, sets);
            break;
        }
    }
}

bool cansplit(DFA &dfa, ALGraph &graph, set<int> &st) {
    if (st.size() <= 1) {
        return false;
    }
    set<char> cst;
    bool hasoutofset = false, hasinofset = false;
    int cnt = 0;
    for (int si : st) {
        bool flag = true;
        for (auto it = dfa.sum.begin(); it != dfa.sum.end(); ++it) {
            int ret = outofset(graph, st, si, *it);
            if (ret == 1) {
                hasoutofset = true;
                cst.insert(*it);
                if (flag) {
                    flag = false;
                    ++cnt;
                }
            } else if (ret == 0) {
                hasinofset = true;
                if (flag) {
                    flag = false;
                    ++cnt;
                }
            }
        }
    }
    if (cnt > 0 && cnt < st.size()) {
        return true;
    }
    if (!hasoutofset) {
        return false;
    }
    if (hasoutofset && hasinofset) {
        return true;
    }
    return cst.size() > 1;
}

//  1 : out of set
//  0 : in the set
// -1 : not right edge
// -2 : no edge
int outofset(ALGraph &graph, set<int> &st, int si, char ch) {
    int sj = -1;
    if (si < graph.size()) {
        if (graph[si].size() == 0) {
            return -2;
        }
        for (ArcNode &arc : graph[si]) {
            if (arc.val == ch) {
                sj = arc.adjvex;
                break;
            }
        }
    }
    if (sj >= 0 && st.find(sj) == st.end()) {
        return 1;
    }
    if (sj < 0) {
        return -1;
    }
    return 0;
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

    simplifydfa(dfa);

    cout << "--------------------------" << endl;

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

    cout << "--------------------------" << endl;

    DFA dfa3;

    moves.clear();
    moves.emplace_back(0, 1, 'a');
    moves.emplace_back(0, 2, 'b');
    moves.emplace_back(2, 3, 'a');
    moves.emplace_back(2, 4, 'b');
    moves.emplace_back(3, 3, 'a');
    moves.emplace_back(3, 4, 'b');
    moves.emplace_back(4, 3, 'a');
    moves.emplace_back(4, 4, 'b');

    dfa3.states = vector<vector<int>>(5);
    dfa3.sum.insert('a');
    dfa3.sum.insert('b');
    dfa3.moves = std::move(moves);
    dfa3.s0 = 0;
    dfa3.final = {1, 2, 3, 4};

    simplifydfa(dfa3);

    cout << "--------------------------" << endl;

    DFA dfa4;

    moves.clear();
    moves.emplace_back(0, 1, 'a');
    moves.emplace_back(0, 2, 'b');
    moves.emplace_back(1, 1, 'a');
    moves.emplace_back(1, 3, 'b');
    moves.emplace_back(2, 1, 'a');
    moves.emplace_back(2, 2, 'b');
    moves.emplace_back(3, 1, 'a');
    moves.emplace_back(3, 4, 'b');
    moves.emplace_back(4, 1, 'a');
    moves.emplace_back(4, 2, 'b');

    dfa4.states = vector<vector<int>>(5);
    dfa4.sum.insert('a');
    dfa4.sum.insert('b');
    dfa4.moves = std::move(moves);
    dfa4.s0 = 0;
    dfa4.final = {4};

    simplifydfa(dfa4);

    return 0;
}
