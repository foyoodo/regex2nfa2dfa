#ifndef __MOVE_H__
#define __MOVE_H__

#include <set>

using namespace std;

struct Move {
    int si, sj;
    union {
        char ch;
        set<char> *chs;
    };

    Move(int _si, int _sj, char _ch) : si(_si), sj(_sj), ch(_ch) {}
    Move(int _si, int _sj, set<char> *_chs) : si(_si), sj(_sj), chs(_chs) {}
};

#endif
