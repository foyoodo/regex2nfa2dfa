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
};

#endif
