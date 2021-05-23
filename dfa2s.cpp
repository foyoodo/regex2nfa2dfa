#include <iostream>

#include "algraph.h"
#include "dfa.h"

using namespace std;

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
    dfa.final = { 1, 2, 3 };

    ALGraph graph;
    buildngraph(graph, dfa.moves);
    orderngraph(graph);

    cout << "--------------------------" << endl;

    DFA dfa2;

    moves.clear();
    moves.emplace_back(0, 1, 'f');
    moves.emplace_back(1, 2, 'e');
    moves.emplace_back(1, 4, 'i');
    moves.emplace_back(2, 3, 'e');
    moves.emplace_back(4, 5, 'e');

    dfa2.states = vector<vector<int>>(6);
    dfa2.sum.insert('e');
    dfa2.sum.insert('f');
    dfa2.sum.insert('i');
    dfa2.moves = std::move(moves);
    dfa2.s0 = 0;
    dfa2.final = { 3, 5 };

    ALGraph graph2;
    buildngraph(graph2, dfa2.moves);
    orderngraph(graph2);

    return 0;
}
