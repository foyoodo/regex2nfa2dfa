#include <algorithm>
#include <iostream>

#include "algraph.h"
#include "dfa.h"
#include "move.h"
#include "sset.h"

using namespace std;

DFA &simplifydfa(DFA &dfa);
sset<sset<int>> &initsets(DFA &dfa);

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

DFA &simplifydfa(DFA &dfa) {
    DFA *newdfa = new DFA();

    ALGraph graph;
    buildngraph(graph, dfa.moves);

    sset<sset<int>> sets = initsets(dfa);

    return *newdfa;
}

// 初始化 sets，按（非）终态集进行切分
sset<sset<int>> &initsets(DFA &dfa) {
    sset<sset<int>> *sets = new sset<sset<int>>;
    sset<int> start, final(true);
    for (int &v : dfa.final) {
        final.push_back(v);
    }
    for (int i = 0; i < dfa.states.size(); ++i) {
        if (find(final.begin(), final.end(), i) == final.end()) {
            start.push_back(i);
        }
    }
    sets->push_back(std::move(start));
    sets->push_back(std::move(final));
    return *sets;
}
