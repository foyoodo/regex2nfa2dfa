#ifndef __SSET_H__
#define __SSET_H__

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class sset : public vector<T> {
public:
    sset(bool _final = false) : final(_final) {}

    bool final;

    void order() {
        cout << (final ? "F" : "S") << ": {";
        for (int i = 0; i < this->size(); ++i) {
            if (i > 0) {
                cout << ", ";
            }
            cout << (*this)[i];
        }
        cout << "}" << endl;
    }
};

template <typename T>
class sset<sset<T>> : public vector<sset<T>> {
public:
    void order() {
        for (auto &set : *this) {
            set.order();
        }
    }
};

#endif
