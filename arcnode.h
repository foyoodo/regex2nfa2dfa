#ifndef __ARCNODE_H_
#define __ARCNODE_H_

#include <vector>

using namespace std;

struct ArcNode {
    union {
        char val;
        vector<char> *vals;
    };
    int adjvex;

    ArcNode(char _val, int _adjvex) : val(_val), adjvex(_adjvex) {}
    ArcNode(vector<char> *_vals, int _adjvex) : adjvex(_adjvex) {
        vals = new vector<char>(*_vals);
    }
};

#endif
