#ifndef __DFA_H__
#define __DFA_H__

#include <set>
#include <vector>

#include "move.h"

using namespace std;

struct DFA {
    vector<vector<int>> states;  // 状态集
    set<char> sum;               // 有限字符集
    vector<Move> moves;          // 状态转移函数
    int s0;                      // 初态
    vector<int> final;           // 终态集

    void order() const {
        cout << "----- DFA BEG -----" << endl;
        cout << ">> S:\n" << endl;
        for (int i = 0; i < states.size(); ++i) {
            auto &state = states[i];
            cout << i << ": {";
            for (int j = 0; j < state.size(); ++j) {
                if (j > 0) {
                    cout << ", ";
                }
                cout << state[j];
            }
            cout << "}" << endl;
        }
        cout << endl << ">> CharSet:";
        for (auto c : sum) {
            cout << " " << c;
        }
        cout << endl;
        cout << endl << ">> Move:\n" << endl;
        for (Move m : moves) {
            cout << "move(" << m.si << ", [";
            for (auto it = m.chs->cbegin(); it != m.chs->cend(); ++it) {
                if (it != m.chs->cbegin()) {
                    cout << ", ";
                }
                cout << *it;
            }
            cout << "]) = " << m.sj << endl;
        }
        cout << endl << ">> s0: " << s0 << endl;
        cout << endl << ">> F:";
        for (int v : final) {
            cout << " " << v;
        }
        cout << endl;
        cout << "----- DFA END -----" << endl;
    }
};

#endif
