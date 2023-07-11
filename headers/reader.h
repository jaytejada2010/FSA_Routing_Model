#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <string>

#include "struct_def.h"

using namespace std;

string getName(ifstream &infile);

string name;
vector <Customer_Node> c_nodes;

void readFile(string s){
    ifstream inFile;

    inFile.open(s);
    
    if (inFile) {
        cout << "File openned successfully" << endl;
        
        name = getName(inFile);
        //c_nodes = getCustomers(inFile);
        
    }else{
        cout << "Unable to open file datafile.txt" << endl;
    }
}

string getName(ifstream &inFile){
    string line;
    getline(inFile, line);

    // Vector of string to save tokens
    vector <string> tokens;
     
    // stringstream class check1
    stringstream check1(line);
     
    string intermediate;
     
    // Tokenizing w.r.t. space ' '
    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    return tokens[1];
}

