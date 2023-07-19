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

class Node{
    public:
        /** CONSTRUCTORS **/
        Node(){
            this->name = -1;
            this->coordinates = {-1, -1};
        }
        
        /** GETTERS **/
        int getName(){
            return this->name;
        }

        location getCoordinates(){
            return this->coordinates;
        }

        int getX(){
            return this->coordinates.x;
        }
        int getY(){
            return this->coordinates.y;
        }
    protected:
        int name;
        location coordinates;
};

/**
 * Customer_Node
 * x - x-coordinate
 * y - y-coordinate
 */

class Customer_Node : public Node{
    public:
        /** CONSTRUCTORS **/
        Customer_Node(int name, int x, int y, int demand, int service_time) { // Constructor with parameters
            this->name = name;
            this->coordinates = {x, y};
            this->demand = demand;
            this->service_time = service_time;
        }
        Customer_Node(){
            this->name = -1;
            this->coordinates = {-1, -1};
            this->demand = -1;
            this->service_time = -1;
        }

        /** DISPLAYERS **/
        void displayNode(){
            cout << this->name << ", " << this->coordinates.x << ", " << this->coordinates.y << " " << this->demand << " " << this->service_time << endl;
        }

        /**GETTERS**/
        int getName(){
            return this->name;
        }

        int getDemand(){
            return this->demand;
        }

        int getServiceTime(){
            return this->service_time;
        }

    private:
        //id will be the index of node in the c_node_array
        int demand;
        int service_time;

};

typedef struct{
    bool hasTech1;
    bool hasTech2;
    bool hasTech3;
}technologies;

class Charging_Node : public Node{
    public:
        /** CONSTRUCTORS **/
        Charging_Node(int name, int x, int y, int t0char, bool has1, bool has2, bool has3) { // Constructor with parameters
            this->name = name;
            this->coordinates = {x, y};
            this->t0char = t0char;
            this->tech = {has1, has2, has3};
        }
        Charging_Node() { // Constructor with parameters
            this->name = -1;
            this->coordinates = {-1, -1};
            this->t0char = -1;
            this->tech = {false, false, false};
        }

        /** DISPLAYERS **/
        void displayNode(){
            cout << this->name << ", " << this->coordinates.x << ", " << this->coordinates.y << " " << this->t0char << " {" << this->tech.hasTech1 << ", " << this->tech.hasTech2 << ", " << this->tech.hasTech3 << "}" << endl;
        }

        /**GETTERS**/
        int getName(){
            return this->name;
        }

        int getT0Char(){
            return this->t0char;
        }

        technologies getTech(){
            return this->tech;
        }

        bool has1(){
            return this->tech.hasTech1;
        }

        bool has2(){
            return this->tech.hasTech2;
        }

        bool has3(){
            return this->tech.hasTech3;
        }

    private:
        //id will be the index of node in the c_node_array
        int t0char;
        technologies tech;
};

class Technology{
    public:
        /** CONSTRUCTORS **/
        Technology(int speed, float cost){
            this->speed = speed;
            this->cost = cost;
        }
        Technology(){
            this->speed = 0;
            this->cost = 0;
        }

        /** DISPLAYER **/
        void displayTech(){
            cout << this->speed << " " << this->cost << endl;
        }

        /** GETTERS **/
        int getSpeed(){
            return this->speed;
        }

        float getCost(){
            return this->cost;
        }

    private:
        int speed;
        float cost;
};

typedef struct{
    int index;
    int type;
}node;

typedef struct{
    int nn; // number of nodes
    int nk; // number of vehicles available
    int nr; // number of recharge stations (excluding depot)
    int ntr; //
    int vel; // velocity or average speed 25km/h
    int bmax; // battery capacity 20KWh
    int cpow; //energy consumption 0.125 KWh/km
    int timemax; //maximum route duration 8 hours => 480 minutes
    int qmax; // vehicle capacity 2300kg
    float f0char; // fixed cost of recharge 2.270 euro/cycle
    int f0vehicle; //
}params;