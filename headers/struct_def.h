#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <string>

using namespace std;

/**
 * Vehicle
 * nodeIndx - index of node in its corresponding node type array
 * nodeType - node type of current node ['C' => Customer, 'R' => Recharge]
 * time - time spent in current node
 */

class Vehicle{
    public:
        int nodeIndx;  //index of node in its corresponding node type array
        char nodeType; //node type to which current node belongs to 
        int time;
};

/**
 * Flamingo
 * cost - sum of monetary cost of all vehicles in current flamingo
 * vehicList - list of vehicles in current flamingo
 */

class Flamingo{
    public:
        int cost;
        vector<Vehicle> vehicleList;
};

/**
 * location
 * x - x-coordinate
 * y - y-coordinate
 */

typedef struct{
    int x;
    int y;
}location;

/**
 * Customer_Node
 * x - x-coordinate
 * y - y-coordinate
 */

class Customer_Node{
    public:
        Customer_Node(int x, int y, int demand, int service_time) { // Constructor with parameters
            this->coordinates = {x, y};
            this->demand = demand;
            this->service_time = service_time;
        }
    private:
        //id will be the index of node in the c_node_array
        location coordinates;
        int demand;
        int service_time;

};

typedef struct{
    unsigned char hasTech1 : 1;
    unsigned char hasTech2 : 1;
    unsigned char hasTech3 : 1;
}technologies;

class Charging_Node{
    public:
        Charging_Node(int x, int y, int t0char, unsigned char has1, unsigned char has2, unsigned char has3) { // Constructor with parameters
            this->coordinates = {x, y};
            this->t0char = t0char;
            this->tech = {has1, has2, has3};
        }
    private:
        //id will be the index of node in the c_node_array
        location coordinates;
        int t0char;
        technologies tech;
};

typedef struct{
    int index;
    int type;
}node;

typedef struct{
    int nn;
    int nk;
    int nr;
    int ntr;
    int vel;
    int bmax;
    int cpow;
    int timemax;
    int qmax;
    float f0char;
    int f0vehicle;
}params;