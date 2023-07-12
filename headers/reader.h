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
params getParams(ifstream &infile);
vector <Customer_Node> getCustomers(ifstream &inFile);

string name;
params prog_params;
vector <Customer_Node> c_nodes;

void readFile(string s){
    ifstream inFile;

    inFile.open(s);
    
    if (inFile) {
        cout << "File openned successfully" << endl;
        
        name = getName(inFile);
        prog_params = getParams(inFile);
        c_nodes = getCustomers(inFile);
        
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

params getParams(ifstream &inFile){
    string line;
    getline(inFile, line);
    vector <int> param_list;
    int x;
    float f0char;
    
    for(x = 0; x < 6; x++)
        getline(inFile, line);
    
    // Vector of string to save tokens
    vector <int> tokens;
     
    // stringstream class check1
    stringstream check1(line);
     
    string intermediate;
     
    // Tokenizing w.r.t. space ' '
    while(getline(check1, intermediate, ' '))
    {
        if(isdigit(intermediate.front())){
            tokens.push_back(stoi(intermediate));
            if(tokens.size() == 10)
                f0char = stof(intermediate);
        }
    }
    
    params p = {tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], tokens[6], tokens[7], tokens[8], f0char, tokens[10]};

    return p;
}

vector <Customer_Node> getCustomers(ifstream &inFile){
    vector <Customer_Node> c;
    int x;
    string line;

    for(x = 0; x < 5; x++){
        getline(inFile, line);
        cout << line << endl;
    }
        

    return c;
}

