#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "algraph.h"
#include "dfa.h"
#include "move.h"
#include "sdfa.h"
#include "driver.cpp"

using namespace std;

int n = 0;

class NFA {
public:
    int state = 0;  // 状态 = n 0 ～ n-1
    set<char> inputChars;
    vector<pair<pair<int, char>, int>> move;
    vector<Move> moves;
    int s0;
    vector<int> F;

    NFA() { inputChars.insert('@'); }
};

NFA nfa;

// NFA 节点
class NfaNode {
public:
    int id;
    map<int, char> edges;  //构成的边 key 是另一个节点的 id value 是边上的值

    NfaNode(int id, map<int, char> edges) {
        this->id = id;
        this->edges = edges;
    }

    NfaNode(int id) { this->id = id; }

    void print() {
        map<int, char>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++) {
            cout << "move(" << id << "," << it->second << ") = " << it->first
                 << endl;
        }
    }
};

vector<NfaNode> graph;

stack<pair<int, int>> oper;
stack<pair<char, int>> label;  // 字符 + 位置

/**
 * 单字符构建 NFA 两个节点
 * O--a-->O--@-->
 */
void createState(char c) {
    map<int, char> map;
    map[n + 1] = c;
    graph.push_back(NfaNode(n++, map));
    graph.push_back(NfaNode(n++));
    oper.push(pair<int, int>(n - 2, n - 1));
}

/**
 * 或运算
 */
void orOperation() {
    if (label.top().first != '|') return;
    label.pop();
    pair<int, int> r = oper.top();
    oper.pop();
    pair<int, int> l = oper.top();
    oper.pop();
    graph.push_back(NfaNode(n++));
    graph.push_back(NfaNode(n++));
    graph[n - 2].edges[l.first] = '@';
    graph[n - 2].edges[r.first] = '@';
    graph[l.second].edges[n - 1] = '@';
    graph[r.second].edges[n - 1] = '@';
    oper.push(pair<int, int>(n - 2, n - 1));
}

/**
 * 或运算
 * O--a-->O--@-->    O--b-->O--@-->
 */
void andOperation() {
    if (oper.size() == 1) return;
    pair<int, int> r = oper.top();
    oper.pop();
    pair<int, int> l = oper.top();
    oper.pop();
    map<int, char> map;
    map[r.first] = '@';
    graph[l.second].edges = map;
    oper.push(pair<int, int>(l.first, r.second));
}

/**
 * 闭包运算
 */
void closureOperation() {
    pair<int, int> l = oper.top();
    oper.pop();
    graph.push_back(NfaNode(n++));
    graph.push_back(NfaNode(n++));
    // O--@-->  ...  --@-->O
    graph[n - 2].edges[l.first] = '@';
    graph[l.second].edges[n - 1] = '@';
    // 闭包 0 次，跳过闭包
    graph[n - 2].edges[n - 1] = '@';
    //闭包 n 次，闭包里面头尾相连
    graph[l.second].edges[l.first] = '@';
    oper.push(pair<int, int>(n - 2, n - 1));
}

/**
 * 右括号运算
 */
void bracketOperation(string &re, int &rightIndex) {
    while (label.top().first != '(') {
        // 运算符栈中，左右括号之间只可能有或运算
        orOperation();
    }
    pair<char, int> leftBracket = label.top();
    label.pop();
    // 后面是闭包 提前计算
    if (rightIndex + 1 < re.size()) {
        //后面字符是否是闭包
        bool flag = false;
        if (re[rightIndex + 1] == '*') {
            closureOperation();
            flag = true;
        }
        if (flag) rightIndex++;
    }
    // 判断运算符栈顶元素位置是否在当前字符前面 如果不在
    // 说明运算符和当前字符之间还有别的字符 需要做与运算
    // 如果当前字符经过转译，则判断算符栈顶元素位置是否和转译字符前一个位置相同
    if (label.empty()) {
        if (!oper.empty()) andOperation();
    } else {
        pair<char, int> o = label.top();
        if (o.second != leftBracket.second - 1) andOperation();
    }
}

/**
 * 字符遍历完之后处理剩下的运算符和 NFA
 */
void handleOther() {
    // 处理或运算
    while (!label.empty()) {
        orOperation();
    }
    while (oper.size() > 1) {
        andOperation();
    }
}

/**
 * 核心方法
 */
void handleRe(string re) {
    for (int i = 0; i < re.size(); i++) {
        char c = re[i];
        if (c == '|') {
            label.push(pair<char, int>(c, i));
        } else if (c == '*') {
            closureOperation();
        } else if (c == '(') {
            label.push(pair<char, int>(c, i));
        } else if (c == ')') {
            bracketOperation(re, i);
        } else {
            if (c == '\\') {
                if (i < re.size())
                    c = re[++i];
                else
                    break;
            }
            nfa.inputChars.insert(c);
            createState(c);
            if (re.size() == 1 || i == 0) continue;
            //后面是闭包或者正闭包 提前计算
            if (i + 1 < re.size()) {
                //后面字符是否是闭包
                bool flag = false;
                if (re[i + 1] == '*') {
                    closureOperation();
                    flag = true;
                }
                if (flag) i++;
            }
            // 判断运算符栈顶元素位置是否在当前字符前面 如果不在
            // 说明运算符和当前字符之间还有别的字符 需要做与运算
            // 如果当前字符经过转译，则判断算符栈顶元素位置是否和转译字符前一个位置相同
            if (label.empty()) {
                if (!oper.empty()) andOperation();
            } else {
                pair<char, int> o = label.top();
                if (re[i - 1] == '\\') {
                    if (o.second != i - 2) andOperation();
                } else if (o.second != i - 1) andOperation();
            }
        }
    }

    // 字符串遍历完之后处理剩下的操作数
    handleOther();
}

//---------------------------------------------------------------------------------------------------------------------------

int m = 0;

DFA dfa;

//子集算法用到的 NFA 图：从第一步的 NFA 的 move 函数转化
ALGraph alGraph;

queue<int> DFAVisited;  // 没计算过乙木希龙-闭包的 DFA 节点

// DFA 节点
class DFANode {
public:
    int id;
    map<int, char> edges;  // 构成的边 key 是另一个节点的 id value 是边上的值
    set<int> states;  // 状态集

    DFANode(int id, map<int, char> edges, set<int> states) {
        this->id = id;
        this->edges = edges;
        this->states = states;
    }

    DFANode(int id) { this->id = id; }

    DFANode(int id, set<int> states) {
        this->id = id;
        this->states = states;
    }

    void print() {
        map<int, char>::iterator it;
        for (it = edges.begin(); it != edges.end(); it++) {
            cout << "move(" << id << "," << it->second << ") = " << it->first
                 << endl;
        }
    }
};

vector<DFANode> DFAGraph;

/**
 * 判断状态集是否重复，重复返回 true
 */
int findDFANode(set<int> &states) {
    for (int i = 0; i < DFAGraph.size(); i++) {
        if (states.size() != DFAGraph[i].states.size()) continue;
        set<int>::iterator it;
        set<int>::iterator it2;
        bool flag = true;
        for (it = states.begin(), it2 = DFAGraph[i].states.begin();
             it != states.end(); it++, it2++) {
            // 不相等
            if (*it != *it2) {
                flag = false;
                break;
            }
        }
        // 集合元素完全相等，重复
        if (flag) {
            return i;
        }
    }
    return -1;
}

/**
 * 递归查找某个点能通过空边到达的节点求乙木希龙-闭包
 */
void findWorklist(set<int> &worklist, int id) {
    vector<bool> visited(alGraph.size(), false);
    queue<int> q;
    q.push(id);
    while (!q.empty()) {
        int i = q.front();
        q.pop();
        if (!visited[i]) {
            for (int k = 0; k < alGraph[i].size(); k++) {
                if (alGraph[i][k].val == '@') {
                    q.push(alGraph[i][k].adjvex);
                }
            }
            visited[i] = true;
            worklist.insert(i);
        }
    }
}

/**
 * 求出一个 DFA 节点经过输入字符能得到的新的 DFA 节点
 */
void findStates(int id) {
    // 先统计能接受输入的字母
    set<char> letters;
    set<int>::iterator it;
    for (it = DFAGraph[id].states.begin(); it != DFAGraph[id].states.end();
         it++) {
        for (int i = 0; i < alGraph[*it].size(); i++) {
            if (alGraph[*it][i].val != '@') {
                letters.insert(alGraph[*it][i].val);
            }
        }
    }

    // 遍历能输入的字母
    set<char>::iterator it2;
    map<char, set<int>> newStates;
    for (it2 = letters.begin(); it2 != letters.end(); it2++) {
        newStates[*it2] = set<int>();
        // 最终的 DFA 的字符集
        dfa.sum.insert(*it2);

        for (it = DFAGraph[id].states.begin(); it != DFAGraph[id].states.end();
             it++) {
            for (int i = 0; i < alGraph[*it].size(); i++) {
                // 由同一个 DFA 节点经过相同字符输入只能得到一个状态集
                // 所以合并状态集
                if (alGraph[*it][i].val == *it2) {
                    findWorklist(newStates[*it2], alGraph[*it][i].adjvex);
                }
            }
        }
    }

    // 遍历当前 DFA 节点经过不同字符输入得到的不同状态集去构建边
    map<char, set<int>>::iterator it3;
    for (it3 = newStates.begin(); it3 != newStates.end(); it3++) {
        // 如果状态集没出现过 构建 DFA 节点
        if (findDFANode(it3->second) == -1) {
            DFAGraph.push_back(DFANode(m++, it3->second));
            DFAGraph[DFAGraph[id].id].edges[m - 1] = it3->first;
            DFAVisited.push(m - 1);
        }
        // 状态集出现过直接构建边
        else {
            // 已存在的 DFA 节点的 id
            int index = findDFANode(it3->second);
            DFAGraph[DFAGraph[id].id].edges[index] = it3->first;
        }
    }
}

/**
 * NFA 转化为 DFA 主方法
 */
void convertDFA() {
    // 先构建出初态集
    DFAGraph.push_back(DFANode(m++));
    DFAVisited.push(m - 1);
    findWorklist(DFAGraph[m - 1].states, nfa.s0);
    while (!DFAVisited.empty()) {
        int id = DFAVisited.front();
        DFAVisited.pop();
        // 遍历状态集中的每个状态
        findStates(id);
    }
}

void buildDFA(DFA &dfa) {
    dfa.s0 = 0;
    for (int i = 0; i < DFAGraph.size(); i++) {
        dfa.states.push_back(
            vector<int>(DFAGraph[i].states.begin(), DFAGraph[i].states.end()));
        map<int, char>::iterator it;
        for (it = DFAGraph[i].edges.begin(); it != DFAGraph[i].edges.end();
             it++) {
            Move move(i, it->first, it->second);
            dfa.moves.push_back(move);
        }

        // 终态集
        set<int>::iterator it2;
        for (it2 = DFAGraph[i].states.begin(); it2 != DFAGraph[i].states.end();
             it2++) {
            if (*it2 == nfa.F[0]) {
                dfa.final.push_back(i);
                break;
            }
        }
    }
}

void nfadfa(string re) {
    dfa = DFA();

    handleRe(re);

    nfa.state = n - 1;
    nfa.s0 = oper.top().first;
    nfa.F.push_back(oper.top().second);
    for (int i = 0; i < graph.size(); i++) {
        map<int, char>::iterator it;
        for (it = graph[i].edges.begin(); it != graph[i].edges.end(); it++) {
            pair<pair<int, char>, int> p(pair<int, char>(i, it->second),
                                         it->first);
            nfa.move.push_back(p);
            nfa.moves.push_back(Move(i, it->first, it->second));
        }
    }

    // 通过转化函数将汤姆森算法求出的 NFA 的 move 函数转化为 NFA 图
    buildngraph(alGraph, nfa.moves);
    convertDFA();
    buildDFA(dfa);

    graph.clear();
    DFAGraph.clear();
    while (!DFAVisited.empty()) {
        DFAVisited.pop();
    }
    alGraph = ALGraph();
    m = 0, n = 0;
    while (!oper.empty()) {
        oper.pop();
    }
    while (!label.empty()) {
        label.pop();
    }
    nfa = NFA();
}

extern DFA type1, type2, type3, type4, type5;

int main() {
    string number_re = "(+|-|@)(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*(@|.(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*)(@|E(+|-|@)(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*)";
    string variable_re = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|_|$|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*)*";
    string relation_re = "<|<=|>|>=|==|!=";
    string keysymbol_re = "+|-|=|\\*|/|\\(|\\)|[|]|{|}|,|;|.|?|'|\"|:|\\||\\|\\||&|&&|->|%|>>|<<|^";
    string keyword_re = "void|if|int|float|short|long|double|bool|else|static|public|private|protected|cin|cout|main";

    nfadfa(number_re);
    type1 = simplifydfa(dfa);

    nfadfa(relation_re);
    type2 = simplifydfa(dfa);

    nfadfa(keysymbol_re);
    type3 = simplifydfa(dfa);

    nfadfa(keyword_re);
    type4 = simplifydfa(dfa);

    nfadfa(variable_re);
    type5 = simplifydfa(dfa);

    startdriver();

    return 0;
}
