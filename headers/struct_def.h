#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <string>
#include <set>

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

        /** CONSTRUCTORS **/
        Vehicle(){
            this->nodeIndx = -1;
            this->nodeType = '\0';
            this->time = -1;
        }

        Vehicle(int nodeIndx, char nodeType, int time){
            this->nodeIndx = nodeIndx;
            this->nodeType = nodeType;
            this->time = time;
        }
};

/**
 * Flamingo
 * cost - sum of monetary cost of all vehicles in current flamingo
 * vehicList - list of vehicles in current flamingo
 */

class Flamingo{
    public:
        double cost;
        vector<vector<Vehicle>> vehicleList;  // multi-dimensional 
        set<int> unvisitedCustomers; // set that holds the index of customers that are not yet visited
        set<int> unvisitedRecharge; // set that holds the index of recharge stations that are not yet visited

        /** CONSTRUCTORS **/
        Flamingo(){
            this->cost = -1;
            this->vehicleList = vector<vector<Vehicle>>();
        }

        Flamingo(int size, int customers, int recharge){
            this->cost = 0;
            this->vehicleList = vector<vector<Vehicle>>(size);
            setUnvisitedCustomers(customers);
            setUnvisitedRecharge(recharge);
        }

        /** SETTERS **/
        void setCost(double cost){
            this->cost = cost;
        }

        void setUnvisitedCustomers(int customers){
            for(int x = 0; x < customers; x++){
                this->unvisitedCustomers.insert(x);
            }
        }

        void setUnvisitedRecharge(int recharge){
            for(int x = 0; x < recharge; x++){
                this->unvisitedRecharge.insert(x);
            }
        }

        /** OTHER OPERATIONS **/
        void insertNodetoVehicle(int vehicle, Vehicle node){
            if(vehicle >= 0 && vehicle < vehicleList.size()){
                this->vehicleList[vehicle].push_back(node);
            }else{
                cout << "Invalid vehicle index at " << vehicle << endl;
                exit(0);
            }
        }

        void visitCustomer(int customer){
            // visit a customer by removing them from the UNVISITED set
            this->unvisitedCustomers.erase(customer);
        }

        void visitRecharge(int recharge){
            // visit a recharge station by removing them from the UNVISITED set
            this->unvisitedRecharge.erase(recharge);
        }

        int getCustomerFromSet(int index){
            set<int>::iterator it = unvisitedCustomers.begin();
            advance(it, index);

            if(it != unvisitedCustomers.end()){
                return *it;
            }else{
                cout << "Invalid customer index at " << index << endl;
                exit(0);
            }
        }

        int getRechargeFromSet(int index){
            set<int>::iterator it = unvisitedRecharge.begin();
            advance(it, index);

            if(it != unvisitedRecharge.end()){
                return *it;
            }else{
                cout << "Invalid recharge index at " << index << endl;
                exit(0);
            }
        }
};

/**
 * @brief Operator class that can be used for Foragin and Migrating Operators
 * 
 * scores => array of scores of each operator for the behavior
 * 
 */
class Operators{
    public:
        vector<int> scores;

    Operators(int num_operators){
        // initialize all the scores of the operators(foraging/migrating) to 0
        for (int x = 0; x < num_operators; x++){
            scores.push_back(0);
        }
    }

    // GETTERS
    int getScore(int index){
        return this->scores[index];
    }

    int getRandomOperator(){
        int totalScore = 0;

        // get the total score of all operators
        for(int x = 0; x < scores.size(); x++){
            totalScore += this->scores[x];
        }
        // Generate a random number between start and end (inclusive)
        int randomValue = rand() % (totalScore - 1);

        for(int x = 0; x < scores.size(); x++){
            randomValue -= this->scores[x];
            if(randomValue < 0)
                return x;
        }

        return 0;
    }

    // SETTERS
    void setScore(int index, int score){
        this->scores[index] += score;
    }
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

            if(has1){
                setChosenTech(0);
            }else if(has2){
                setChosenTech(1);
            }else if(has3){
                setChosenTech(2);
            }
        }
        Charging_Node() { // Constructor with parameters
            this->name = -1;
            this->coordinates = {-1, -1};
            this->t0char = -1;
            this->tech = {false, false, false};
            setChosenTech(-1);
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

        int getChosenTech(){
            return this->chosen_tech;
        }

        /**SETTERS**/
        void setChosenTech(int chosen_tech){
            this->chosen_tech = chosen_tech;
        }

    private:
        //id will be the index of node in the c_node_array
        int t0char;
        int chosen_tech;
        technologies tech;
};

class Technology{
    public:
        /** CONSTRUCTORS **/
        Technology(int speed, double cost){
            this->speed = speed;     // KWH/hour
            this->cost = cost;       // Euro/KWH
        }
        Technology(){
            this->speed = 0;
            this->cost = 0;
        }

        /** DISPLAYER **/
        void displayTech(){
            cout << endl << this->speed << " " << this->cost << endl;
        }

        /** GETTERS **/
        int getSpeed(){
            return this->speed;
        }

        double getCost(){
            return this->cost;
        }

    private:
        int speed;
        double cost;
};

typedef struct{
    int index;
    int type;
}node;

typedef struct{
    int num_of_customers; // number of customers
    int num_of_vehicles; // number of vehicles available
    int num_of_recharge; // number of recharge stations (excluding depot)
    int num_of_tech; //  number of technologies (0 - 2) => 3 available tech
    int velocity; // velocity or average speed 25km/h
    int battery_max; // battery capacity 20KWh
    int energy_consumption; //energy consumption 0.125 KWh/km
    int time_max; //maximum route duration 8 hours => 480 minutes
    int vehicle_capacity; // vehicle capacity 2300kg
    float fixed_cost_recharge; // fixed cost of recharge 2.270 euro/cycle
    int f0vehicle; //
}params;