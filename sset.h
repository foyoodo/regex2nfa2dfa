#ifndef __SSET_H__
#define __SSET_H__

#include <iostream>
#include <set>
#include <vector>

using namespace std;

template <typename T>
class sset : public set<T> {
public:
    sset(bool _final = false) : final(_final) {}

    bool final;

    void order() const {
        cout << (final ? "F" : "S") << ": {";
        for (auto it = this->begin(); it != this->end(); ++it) {
            if (it != this->begin()) {
                cout << ", ";
            }
            cout << *it;
        }
        cout << "}" << endl;
    }
};

template <typename T>
class sset<sset<T>> : public vector<sset<T>> {
public:
    void order() const {
        for (auto &st : *this) {
            st.order();
        }
        cout << endl;
    }
};

#endif
