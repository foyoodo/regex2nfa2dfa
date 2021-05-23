#ifndef __MOVE_H__
#define __MOVE_H__

#include <vector>

using namespace std;

struct Move {
    int si, sj;
    union {
        char ch;
        vector<char> *chs;
    };

    Move(int _si, int _sj, char _ch) : si(_si), sj(_sj), ch(_ch) {}
};

#endif