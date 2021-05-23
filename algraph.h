#ifndef __ALGRAPH_H__
#define __ALGRAPH_H__

#include <algorithm>
#include <vector>

#include "arcnode.h"
#include "move.h"

using namespace std;

typedef vector<vector<ArcNode>> ALGraph;

inline void increase(ALGraph &graph, int k) {
    for (int i = graph.size(); i <= k; ++i) {
        graph.push_back(vector<ArcNode>());
    }
}

void newarc(ALGraph &graph, int l, int r) {
    graph[l].emplace_back('$', r);
}

void newarc(ALGraph &graph, int l, int r, char c) {
    increase(graph, max(l, r));
    graph[l].emplace_back(c, r);
}

void newarc(ALGraph &graph, int l, int r, vector<char> *chs) {
    increase(graph, max(l, r));
    graph[l].emplace_back(chs, r);
}

void buildngraph(ALGraph &graph, vector<Move> &moves) {
    for (Move m : moves) {
        newarc(graph, m.si, m.sj, m.ch);
    }
}

void builddgraph(ALGraph &graph, vector<Move> &moves) {
    for (Move m : moves) {
        newarc(graph, m.si, m.sj, m.chs);
    }
}

void orderngraph(ALGraph &graph) {
    for (int i = 0; i < graph.size(); ++i) {
        printf("%d", i);
        for (ArcNode &node : graph[i]) {
            printf(" -> (%c, %d)", node.val, node.adjvex);
        }
        printf("\n");
    }
}

void orderdgraph(ALGraph &graph) {
    for (int i = 0; i < graph.size(); ++i) {
        printf("%d", i);
        for (ArcNode &node : graph[i]) {
            printf(" -> ([");
            for (int j = 0; j < node.vals->size(); ++j) {
                if (j > 0) {
                    printf(", ");
                }
                printf("%c", (*node.vals)[j]);
            }
            printf("], %d)", node.adjvex);
        }
        printf("\n");
    }
}

int targetdgraph(ALGraph &graph, int si, char ch) {
    int sj = -1;
    if (si < graph.size()) {
        for (ArcNode &arc : graph[si]) {
            vector<char>::iterator it = find(arc.vals->begin(), arc.vals->end(), ch);
            if (it != arc.vals->end()) {
                sj = arc.adjvex;
                break;
            }
        }
    }
    return sj;
}

#endif
