#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
int main(int argc, char const *argv[]) {
    ifstream in("input.c");
    string line, str;

    while (getline(in, line)) {
        istringstream is(line);
        while (is >> str) {
            cout << str << endl;
        }
    }

    in.close();
    return 0;
}
