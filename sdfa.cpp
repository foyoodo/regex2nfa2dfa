#include <algorithm>
#include <iostream>
#include <set>

#include "algraph.h"
#include "dfa.h"
#include "move.h"
#include "sset.h"

using namespace std;

DFA &simplifydfa(DFA &dfa);
sset<sset<int>> &initsets(DFA &dfa);
bool split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets, int k);
void split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets);

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

    return 0;
}

static int deep;

DFA &simplifydfa(DFA &dfa) {
    DFA *newdfa = new DFA();

    ALGraph graph;
    buildngraph(graph, dfa.moves);

    sset<sset<int>> sets = initsets(dfa);

    deep = 0;
    split(graph, dfa.sum, sets);

    return *newdfa;
}

// 判断 t 和 s 是否可区分，true 代表可区分，false 代表不可区分
bool check(ALGraph &graph, sset<sset<int>> &sets, int t, int s, char w) {
    bool ret = false;
    if (max(t, s) < graph.size()) {
        int ft = -1, fs = ft;
        for (auto &node : graph[t]) {
            if (node.val == w) {
                ft = node.adjvex;
                break;
            }
        }
        for (auto &node : graph[s]) {
            if (node.val == w) {
                fs = node.adjvex;
                break;
            }
        }
        if (ft != fs) {
            for (auto &st : sets) {
                if ((st.find(ft) != st.end()) && (st.find(fs) != st.end())) {
                    ret = true;
                    break;
                }
            }
        }
    }
    return ret;
}

bool split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets, int k) {
    bool ret = true;
    sset<int> &st = sets[k];
    if (st.size() <= 1) {
        ret = false;
    } else {
        sset<int> st0(st.final), st1(st.final);
        int first = *st.begin();
        st0.insert(first);
        for (auto it = st.begin(); it != st.end(); ++it) {
            if (it == st.begin()) {
                continue;
            }
            for (char w : sum) {
                if (check(graph, sets, first, *it, w)) {
                    st0.insert(*it);
                } else {
                    st1.insert(*it);
                }
            }
        }
        if (st0.size() == st.size()) {
            ret = false;
        } else {
            auto it = sets.begin() + k;
            sets.erase(it);
            sets.insert(it, std::move(st1));
            sets.insert(it, std::move(st0));
        }
    }
    return ret;
}

void split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets) {
    if (++deep > 5) {
        return;
    }
    for (int i = 0; i < sets.size(); ++i) {
        sset<int> &st = sets[i];
        if (split(graph, sum, sets, i)) {
            sets.order();
            split(graph, sum, sets);
            break;
        }
    }
}

// 初始化 sets，按（非）终态集进行切分
sset<sset<int>> &initsets(DFA &dfa) {
    sset<sset<int>> *sets = new sset<sset<int>>;
    sset<int> start, final(true);
    for (int &v : dfa.final) {
        final.insert(v);
    }
    for (int i = 0; i < dfa.states.size(); ++i) {
        if (final.find(i) == final.end()) {
            start.insert(i);
        }
    }
    sets->push_back(std::move(start));
    sets->push_back(std::move(final));
    return *sets;
}
