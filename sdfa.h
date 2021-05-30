#ifndef __SDFA_H__
#define __SDFA_H__

#include <algorithm>
#include <iostream>

#include "algraph.h"
#include "dfa.h"
#include "move.h"
#include "sset.h"

using namespace std;

DFA &simplifydfa(DFA &dfa);
sset<sset<int>> &initsets(DFA &dfa);
bool check(ALGraph &graph, sset<sset<int>> &sets, int t, int s, char w);
bool split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets, int k);
void split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets);
ALGraph &rebuilddgraph(ALGraph &graph, sset<sset<int>> &sets);

static int deep;

DFA &simplifydfa(DFA &dfa) {
    DFA *newdfa = new DFA();

    ALGraph graph;
    buildngraph(graph, dfa.moves);

    sset<sset<int>> sets = initsets(dfa);

    deep = 0;
    split(graph, dfa.sum, sets);
    sets.order();

    ALGraph newgraph = rebuilddgraph(graph, sets);

    for (int i = 0; i < sets.size(); ++i) {
        auto &st = sets[i];
        vector<int> state;
        for (int v : st) {
            state.push_back(v);
        }
        if (st.final) {
            newdfa->final.push_back(i);
        }
        newdfa->states.push_back(std::move(state));
    }

    for (int i = 0; i < newgraph.size(); ++i) {
        for (ArcNode &node : newgraph[i]) {
            newdfa->moves.emplace_back(i, node.adjvex, node.vals);
        }
    }

    newdfa->sum = dfa.sum;
    newdfa->s0 = 0;

    return *newdfa;
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
        if (ft == -1 && ft == fs) {
            ret = false;
        } else if (ft == -1 || fs == -1) {
            ret = true;
        } else if (ft != fs) {
            ret = true;
            for (auto &st : sets) {
                if ((st.find(ft) != st.end()) && (st.find(fs) != st.end())) {
                    ret = false;
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
                    st1.insert(*it);
                }
            }
        }
        for (auto it = st.begin(); it != st.end(); ++it) {
            if (it == st.begin()) {
                continue;
            }
            if (st1.find(*it) == st1.end()) {
                st0.insert(*it);
            }
        }
        if (st1.empty()) {
            ret = false;
        } else {
            sets.erase(sets.begin() + k);
            sets.insert(sets.begin() + k, std::move(st1));
            sets.insert(sets.begin() + k, std::move(st0));
        }
    }
    return ret;
}

void split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets) {
    if (++deep > 10) {
        return;
    }
    for (int i = 0; i < sets.size(); ++i) {
        sset<int> &st = sets[i];
        if (split(graph, sum, sets, i)) {
            split(graph, sum, sets);
            break;
        }
    }
}

int indexinsets(sset<sset<int>> &sets, int si) {
    for (int i = 0; i < sets.size(); ++i) {
        sset<int> &st = sets[i];
        if (st.find(si) != st.end()) {
            return i;
        }
    }
    return -1;
}

ALGraph &rebuilddgraph(ALGraph &graph, sset<sset<int>> &sets) {
    ALGraph *newgraph = new ALGraph();

    for (int i = 0; i < sets.size(); ++i) {
        sset<int> &st = sets[i];
        for (int si : st) {
            for (ArcNode &node : graph[si]) {
                addarc(*newgraph, i, indexinsets(sets, node.adjvex), node.val);
            }
        }
    }

    return *newgraph;
}

#endif
