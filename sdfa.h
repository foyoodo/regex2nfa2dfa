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

DFA &simplifydfa(DFA &dfa) {
    DFA *newdfa = new DFA();

    ALGraph graph;
    buildngraph(graph, dfa.moves);

    sset<sset<int>> sets = initsets(dfa);

    split(graph, dfa.sum, sets);

    ALGraph newgraph = rebuilddgraph(graph, sets);

    // 设置 newdfa 的状态集与终态集
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

    // 设置 newdfa 的 move 函数
    for (int i = 0; i < newgraph.size(); ++i) {
        for (ArcNode &node : newgraph[i]) {
            newdfa->moves.emplace_back(i, node.adjvex, node.vals);
        }
    }

    // 有限字符集与初态
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

// 对 sets 中的指定集合进行切分，返回 true 代表集合可切并已完成切分
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

// 对 sets 进行递归切分（每次切分成功后从第一个子集开始检验）
void split(ALGraph &graph, set<char> &sum, sset<sset<int>> &sets) {
    for (int i = 0; i < sets.size(); ++i) {
        sset<int> &st = sets[i];
        if (split(graph, sum, sets, i)) {
            split(graph, sum, sets);
            break;
        }
    }
}

// 返回 si 所在子集的索引
int indexinsets(sset<sset<int>> &sets, int si) {
    for (int i = 0; i < sets.size(); ++i) {
        sset<int> &st = sets[i];
        if (st.find(si) != st.end()) {
            return i;
        }
    }
    return -1;
}

// 重建并返回最小化 DFA 对应的的 ALGraph
ALGraph &rebuilddgraph(ALGraph &graph, sset<sset<int>> &sets) {
    ALGraph *newgraph = new ALGraph();

    for (int i = 0; i < sets.size(); ++i) {
        sset<int> &st = sets[i];
        // 只需对子集中第一个状态的出度进行处理
        int si = *st.begin();
        for (ArcNode &node : graph[si]) {
            addarc(*newgraph, i, indexinsets(sets, node.adjvex), node.val);
        }
    }

    return *newgraph;
}

#endif
