#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <string>
#include <set>

using namespace std;

class Operator {
    public:
        string name;

    Operator (string n) {
        this->name = n;
    }
};

void function1() {
    cout << "\nThis is function one";
}

void function2() {
    cout << "\nThis is function two";
}

int main () {
    Operator op("function1");
    cout << op.name;

    return 0;
}