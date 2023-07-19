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
vector <Charging_Node> getChargers(ifstream &inFile);
vector <Technology> getTechnologies(ifstream &inFile);

string name;
params prog_params;
vector <Customer_Node> c_nodes;
vector <Charging_Node> r_nodes;
vector <Technology> tech_list;

int num_of_nodes = 1; // by default is 1 which is the depot

void readFile(string s){
    ifstream inFile;
    inFile.open(s);
    
    if (inFile) {
        cout << "File openned successfully" << endl;
        
        name = getName(inFile);
        prog_params = getParams(inFile);
        c_nodes = getCustomers(inFile);
        r_nodes = getChargers(inFile);
        tech_list = getTechnologies(inFile);
        
        // update total number of nodes 
        num_of_nodes += prog_params.num_of_customers + prog_params.num_of_recharge;
        
    }else{
        cout << "Unable to open file " << s << endl;
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

    /** reposition file pointer to first data row **/
    for(x = 0; x < 4; x++)
        getline(inFile, line);

    // Vector of string to save tokens
    vector <int> tokens;
    stringstream check1;
    string intermediate;
    
    for(x = 0; x < prog_params.num_of_customers; x++){
        getline(inFile, line);
        check1 = stringstream(line);

        // Tokenizing w.r.t. space ' '
        while(getline(check1, intermediate, ' ')){
            if(isdigit(intermediate.front())){
                tokens.push_back(stoi(intermediate));
            }
        }
        c.push_back(Customer_Node(tokens[1], tokens[2], tokens[3], tokens[4]));
        tokens.clear();
    }

    return c;
}

vector <Charging_Node> getChargers(ifstream &inFile){
    vector <Charging_Node> r;
    int x, i;
    string line;

    /** reposition file pointer to first data row **/
    for(x = 0; x < 4; x++)
        getline(inFile, line);

    // Vector of string to save tokens
    vector <int> tokens;
    stringstream check1;
    string intermediate;
    
    for(x = 0, i = prog_params.num_of_recharge + 1; x < i; x++){
        getline(inFile, line);
        cout << line << endl;
        check1 = stringstream(line);

        // Tokenizing w.r.t. space ' '
        while(getline(check1, intermediate, ' ')){
            if(isdigit(intermediate.front())){
                tokens.push_back(stoi(intermediate));
            }
        }
        r.push_back(Charging_Node(tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], tokens[6]));
        tokens.clear();
    }

    return r;
}

vector <Technology> getTechnologies(ifstream &inFile){
    vector <Technology> t;
    int x;
    float cost;
    string line;

    for(x = 0; x < 4; x++)
        getline(inFile, line);

    // Vector of string to save tokens
    vector <int> tokens;
    stringstream check1;
    string intermediate;
    
    for(x = 0; x < 3; x++){
        getline(inFile, line);
        check1 = stringstream(line);

        // Tokenizing w.r.t. space ' '
        while(getline(check1, intermediate, ' ')){
            if(isdigit(intermediate.front())){
                tokens.push_back(stoi(intermediate));
                if(tokens.size() == 3)
                    cost = stof(intermediate);
            }
        }
        t.push_back(Technology(tokens[1], cost));
        tokens.clear();
    }

    return t;
}

