#include <iostream>
#include <vector>

using namespace std;

struct Move {
    int si, sj;
    char ch;

    Move(int _si, int _sj, char _ch) : si(_si), sj(_sj), ch(_ch) {}
};

struct ArcNode {
    char val;
    int adjvex;

    ArcNode(char _val, int _adjvex) : val(_val), adjvex(_adjvex) {}
};

typedef vector<vector<ArcNode>> ALGraph;

inline void increase(ALGraph &graph, int k) {
    for (int i = graph.size(); i <= k; ++i) {
        graph.push_back(vector<ArcNode>());
    }
}

void newarc(ALGraph &graph, int l, int r, char c) {
    increase(graph, max(l, r));
    graph[l].emplace_back(c, r);
}

void buildgraph(ALGraph &graph, vector<Move> &moves) {
    for (Move m : moves) {
        newarc(graph, m.si, m.sj, m.ch);
    }
}

void ordergraph(ALGraph &graph) {
    for (int i = 0; i < graph.size(); ++i) {
        printf("%d", i);
        for (ArcNode &node : graph[i]) {
            printf(" -> (%c, %d)", node.val, node.adjvex);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[]) {
    ALGraph graph;
    vector<Move> moves;
    moves.emplace_back(0, 1, 'a');
    moves.emplace_back(1, 5, '$');
    moves.emplace_back(2, 3, 'b');
    moves.emplace_back(3, 5, '$');
    moves.emplace_back(4, 2, '$');
    moves.emplace_back(4, 0, '$');
    moves.emplace_back(5, 4, '$');
    moves.emplace_back(5, 7, '$');
    moves.emplace_back(6, 4, '$');
    moves.emplace_back(6, 7, '$');
    moves.emplace_back(7, 8, 'a');
    moves.emplace_back(8, 9, 'b');
    moves.emplace_back(9, 10, 'b');
    buildgraph(graph, moves);
    ordergraph(graph);
    return 0;
}