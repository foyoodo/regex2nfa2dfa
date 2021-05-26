#ifndef __ARCNODE_H_
#define __ARCNODE_H_

#include <set>

using namespace std;

struct ArcNode {
    union {
        char val;
        set<char> *vals;
    };
    int adjvex;

    ArcNode(char _val, int _adjvex) : val(_val), adjvex(_adjvex) {}
    ArcNode(set<char> *_vals, int _adjvex) : vals(_vals), adjvex(_adjvex) {}
};

#endif
