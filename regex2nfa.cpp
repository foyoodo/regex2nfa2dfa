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

inline void increase(ALGraph &graph, int k) {
    for (int i = graph.size(); i <= k; ++i) {
        graph.push_back(vector<ArcNode *>());
    }
}

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

    graph[l].push_back(new ArcNode(v, a));

    return a;
}

void dealstar(ALGraph &graph, int &l, int &r) {
    if (l == r) {
        r = cur++;
        increase(graph, r);
    }
    graph[l].push_back(new ArcNode('a', r));
    graph[r].push_back(new ArcNode('$', l));
    l = cur++;
    r = cur++;
    increase(graph, r);
    graph[l].push_back(new ArcNode('$', r));
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

bool isOperator(const char ch) {
    if (ch == '(' || ch == ')' || ch == '|' || ch == '.' || ch == '*') {
        return true;
    }
    return false;
}

bool build(ALGraph &graph, const string &s) {
    int left = cur++;
    int right = left;

    increase(graph, left);

    for (int i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '(');
        else if (c == '|') ops.push(c);
        else if (c == '_') ops.push(c);
        else if (c == '*') {

        } else if (c == ')') {

        } else vals.push(c);

        if (s[i] == '(' || s[i] == '|') {
            ops.push(s[i]);
        } else {
            if (s[i] == ')') {

            } else if (s[i] == '*') {

            } else if (s[i] == '|') {
                ops.push(s[i]);
            } else {  // s[i] -> value
                if (vals.empty()) {
                    vals.push(s[i]);
                } else {
                    if (ops.empty() || ops.top() == '(') { // ab
                        char val = vals.top();
                        vals.pop();
                        vals.push(s[i]);
                        right = dealand(graph, right, val);
                    } else {  // ops has '|' => a|b , don't calculate
                        vals.push(s[i]);
                    }
                }
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
    bool ret = build(graph, "(a|b)*_a_b_b");
    order(graph);
    return 0;
}