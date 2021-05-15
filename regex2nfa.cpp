#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct NFA {
    vector<int> states;
    vector<char> set;
    int s0;
    vector<int> final;
};

struct ArcNode {
    char val;
    int adjvex;

    ArcNode(char _val, int _adjvex) : val(_val), adjvex(_adjvex) {}
};

typedef vector<vector<ArcNode *>> ALGraph;

int cur = 0;
stack<char> ops, vals;

void dealor(ALGraph &graph, int l, int r, char v) {
    int a = cur++;
    int b = cur++;

    for (int i = graph.size(); i <= b; ++i) {
        graph.push_back(vector<ArcNode *>());
    }

    graph[l].push_back(new ArcNode('$', a));
    graph[a].push_back(new ArcNode(v, b));
    graph[b].push_back(new ArcNode('$', r));
}

int dealand(ALGraph &graph, int l, char v) {
    int a = cur++;

    for (int i = graph.size(); i <= a; ++i) {
        graph.push_back(vector<ArcNode *>());
    }

    graph[a].push_back(new ArcNode(v, l));

    return a;
}

void dealstar(ALGraph &graph, int l, int r) {
    graph[r].push_back(new ArcNode('$', l));
}

void dealbracket(ALGraph &graph, int l, int r) {

}

void deal(ALGraph &graph, int l, int r) {
    if (ops.empty()) {
        int left = cur++;
        for (int i = graph.size(); i <= left; ++i) {
            graph.push_back(vector<ArcNode *>());
        }
        while (!vals.empty()) {
            char v = vals.top();
            vals.pop();
            left = dealand(graph, v, left);
        }
        graph[r].push_back(new ArcNode('$', left));
    } else {

    }
}

bool build(ALGraph &graph, const string &s) {
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(' || s[i] == '|') {
            ops.push(s[i]);
        } else {
            if (s[i] == ')') {

            } else if (s[i] == '*') {

            } else {
                if (!vals.empty()) {
                    char val = vals.top();
                    vals.pop();
                }
                vals.push(s[i]);
            }
        }
    }
    return true;
}

void order(ALGraph &graph) {
    for (int i = 0; i < graph.size(); ++i) {
        printf("%d", i);
        for (ArcNode *node : graph[i]) {
            printf(" -> (%c, %d)", node->val, node->adjvex);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[]) {
    ALGraph graph;
    bool ret = build(graph, "(a|b)*abb");
    order(graph);
    return 0;
}