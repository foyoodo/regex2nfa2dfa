#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct Move {
    int si, sj;
    char ch;
};

struct NFA {
    vector<int> states;
    vector<char> sum;
    vector<Move> moves;
    int s0;
    vector<int> final;
};

struct NFANode {
    int left, right;

    NFANode(int _left, int _right) : left(_left), right(_right) {}
};

struct ArcNode {
    char val;
    int adjvex;

    ArcNode(char _val, int _adjvex) : val(_val), adjvex(_adjvex) {}
};

typedef vector<vector<ArcNode *>> ALGraph;

int cur = -1;
stack<char> ops;
stack<NFANode *> nfas;

int getnumber(ALGraph &graph) {
    graph.push_back(vector<ArcNode *>());
    return ++cur;
}

void newarc(ALGraph &graph, int l, int r) {
    graph[l].push_back(new ArcNode('$', r));
}

void newarc(ALGraph &graph, int l, int r, char c) {
    graph[l].push_back(new ArcNode(c, r));
}

void matcharc(ALGraph &graph, const string &s, int k, bool &flag) {
    int l, r;
    char c = s[k];
    if (k < s.size() - 1 && s[k + 1] != '*') {
        if (nfas.empty() || (!ops.empty() && (ops.top() == '(' || ops.top() == '|') && flag)) {
            flag = false;
            l = getnumber(graph);
            r = getnumber(graph);
            newarc(graph, l, r, c);
            nfas.push(new NFANode(l, r));
        } else {
            NFANode *node = nfas.top();
            l = node->left;
            r = getnumber(graph);
            newarc(graph, node->right, r, c);
            node->right = r;
        }
    } else if (k < s.size() - 1 && s[k + 1] == '*') {
        l = getnumber(graph);
        r = getnumber(graph);
        newarc(graph, l, r, c);
        nfas.push(new NFANode(l, r));
    } else if (nfas.empty() || (!ops.empty() && (ops.top() == '(' || ops.top() == '|') && flag)) {
        flag = false;
        l = getnumber(graph);
        r = getnumber(graph);
        newarc(graph, l, r, c);
        nfas.push(new NFANode(l, r));
    } else {
        NFANode *node = nfas.top();
        l = node->left;
        r = getnumber(graph);
        newarc(graph, node->right, r, c);
        node->right = r;
    }
}

void matchadd(ALGraph &graph) {
    int l, r;
    if (!nfas.empty()) {
        NFANode *node = nfas.top();
        newarc(graph, node->right, node->left);
        l = getnumber(graph);
        r = getnumber(graph);
        newarc(graph, l, node->left);
        newarc(graph, node->right, r);
        node->left = l;
        node->right = r;
    }
}

void matchstar(ALGraph &graph) {
    int l, r;
    if (!nfas.empty()) {
        NFANode *node = nfas.top();
        newarc(graph, node->right, node->left);
        l = getnumber(graph);
        r = getnumber(graph);
        newarc(graph, l, node->left);
        newarc(graph, node->right, r);
        newarc(graph, l, r);
        node->left = l;
        node->right = r;
    }
}

void matchor(ALGraph &graph) {
    int l, r;
    if (nfas.size() >= 2) {
        NFANode *node1 = nfas.top();
        nfas.pop();
        NFANode *node2 = nfas.top();
        l = getnumber(graph);
        r = getnumber(graph);
        newarc(graph, l, node1->left);
        newarc(graph, l, node2->left);
        newarc(graph, node1->right, r);
        newarc(graph, node2->right, r);
        node2->left = l;
        node2->right = r;

        delete node1;
    }
}

void matchbracket(ALGraph &graph) {
    while (!ops.empty()) {
        if (ops.top() == '|') {
            matchor(graph);
            ops.pop();
        } else if (ops.top() == '(') {
            ops.pop();
            break;
        }
    }
}

void matchand(ALGraph &graph) {
    while (nfas.size() >= 2) {
        NFANode *node1 = nfas.top();
        nfas.pop();
        NFANode *node2 = nfas.top();
        newarc(graph, node2->right, node1->left);
        node2->right = node1->right;

        delete node1;
    }
}

NFA *createnfa(ALGraph &graph) {
    NFA *nfa = new NFA();
    return nfa;
}

NFA *build(ALGraph &graph, const string &s) {
    bool flag = false;
    for (int i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '(') {
            ops.push(c);
            flag = true;
        } else if (c == ')') {
            matchbracket(graph);
        } else if (c == '|') {
            ops.push(c);
            flag = true;
        } else if (c == '*') {
            matchstar(graph);
            if (ops.empty() || (ops.top() == '|' && (nfas.size() > ops.size() + 1))) {
                matchand(graph);
            }
        } else if (c == '+') {
            matchadd(graph);
        } else {
            matcharc(graph, s, i, flag);
        }
    }
    matchbracket(graph);
    matchand(graph);

    NFA *nfa = createnfa(graph);

    cur = -1;
    while (!ops.empty()) {
        ops.pop();
    }
    while (!nfas.empty()) {
        NFANode *node = nfas.top();
        nfas.pop();
        delete node;
    }

    return nfa;
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
    ALGraph graph, graph2, graph3;
    build(graph, "(a|b)*abb");
    order(graph);
    cout << "---------------------------" << endl;
    build(graph2, "(a|b)*a|bcd");
    order(graph2);
    return 0;
}