#include <iostream>
#include <vector>

#include "algraph.h"

using namespace std;

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
    buildngraph(graph, moves);
    orderngraph(graph);
    return 0;
}