#include <iostream>

#include "dfa.h"
#include "move.h"
#include "algraph.h"

using namespace std;

int main(int argc, char const *argv[]) {
    DFA dfa;

    vector<Move> moves;
    moves.emplace_back(0, 1, new set<char>{'a'});
    moves.emplace_back(1, 1, new set<char>{'b', 'c'});

    dfa.states = vector<vector<int>>(2);
    dfa.sum.insert('a');
    dfa.sum.insert('b');
    dfa.sum.insert('c');
    dfa.moves = std::move(moves);
    dfa.s0 = 0;
    dfa.final = {1};

    ALGraph graph;
    builddgraph(graph, dfa.moves);
    orderdgraph(graph);

    cout << "--------------------------" << endl;

    DFA dfa2;

    moves.clear();
    moves.emplace_back(0, 1, new set<char>{'f'});
    moves.emplace_back(1, 2, new set<char>{'e', 'i'});
    moves.emplace_back(2, 3, new set<char>{'e'});

    dfa2.states = vector<vector<int>>(4);
    dfa2.sum.insert('e');
    dfa2.sum.insert('f');
    dfa2.sum.insert('i');
    dfa2.moves = std::move(moves);
    dfa2.s0 = 0;
    dfa2.final = {3};

    ALGraph graph2;
    builddgraph(graph2, dfa2.moves);
    orderdgraph(graph2);

    return 0;
}
