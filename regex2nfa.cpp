#include <numeric>
#include <iostream>
#include <stack>
#include <vector>
#include <set>

using namespace std;

struct Move {
    int si, sj;
    char ch;

    Move(int _si, int _sj, char _ch) : si(_si), sj(_sj), ch(_ch) {}
};

struct NFA {
    vector<int> states;
    set<char> sum;
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
        matchstar(graph);
        NFANode *node = nfas.top();
        nfas.pop();
        if (!nfas.empty()) {
            newarc(graph, nfas.top()->right, node->left);
            nfas.top()->right = r;
            delete node;
        } else {
            nfas.push(node);
        }
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

NFA *createnfa(ALGraph &graph) {
    NFA *nfa = new NFA();
    nfa->states.resize(cur + 1);
    vector<bool> visited(cur + 1);
    iota(nfa->states.begin(), nfa->states.end(), 0);
    for (int i = 0; i < graph.size(); ++i) {
        for (ArcNode *node : graph[i]) {
            nfa->sum.insert(node->val);
            nfa->moves.emplace_back(i, node->adjvex, node->val);
            visited[node->adjvex] = true;
        }
        if (graph[i].empty()) {
            nfa->final.push_back(i);
        }
    }
    for (int i = 0; i < visited.size(); ++i) {
        if (!visited[i]) {
            nfa->s0 = i;
            break;
        }
    }
    return nfa;
}

void ordernfa(NFA *nfa) {
    cout << "----- NFA BEG -----" << endl;
    cout << ">> S:";
    for (int s : nfa->states) {
        cout << " " << s;
    }
    cout << endl << ">> CharSet:";
    for (auto it = nfa->sum.begin(); it != nfa->sum.end(); ++it) {
        cout << " " << *it;
    }
    cout << endl << ">> Move";
    for (Move &m : nfa->moves) {
        printf("\nmove(%d, %c) = %d", m.si, m.ch, m.sj);
    }
    cout << endl << ">> s0: " << nfa->s0;
    cout << endl << ">> F:";
    for (int v : nfa->final) {
        cout << " " << v;
    }
    cout << endl << "----- NFA END -----" << endl;
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
            if (i > 0 && s[i - 1] == ')') {
                matchstar(graph);
            }
        } else if (c == '+') {
            matchadd(graph);
        } else {
            matcharc(graph, s, i, flag);
        }
    }
    matchbracket(graph);

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
    NFA *nfa = build(graph, "(a|b)*abb");
    order(graph);
    ordernfa(nfa);
    cout << "---------------------------" << endl;
    NFA *nfa2 = build(graph2, "(a|b)*a|bcd");
    order(graph2);
    ordernfa(nfa2);
    cout << "---------------------------" << endl;
    NFA *nfa3 = build(graph3, "ab|cd*");
    order(graph3);
    ordernfa(nfa3);
    return 0;
}