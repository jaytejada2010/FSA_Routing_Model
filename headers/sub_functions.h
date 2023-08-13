#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <string>
#include <random>

#include "reader.h"

#define CONSTANT_SIZE 500

typedef double Matrix[CONSTANT_SIZE][CONSTANT_SIZE];

/**
 * @brief calculate the distance between node 1 and node 2
 * 
 * @param x1 x-coordinate of node 1
 * @param y1 y-coordinate of node 1
 * @param x2 x-coordinate of node 2
 * @param y2 y-coordinate of node 2
 * @return ** double distance expressed in kilometers(km)
 */
double calculateDistance(int x1, int y1, int x2, int y2){
    return (pow(x2 - x1, 2) + pow(y2 - y1, 2)) / 1000; //using euclidean distance between 2 points
    // division of 1000 is to convert m => km
}

/**
 * @brief calculate the energy consumed during the travel between 2 nodes
 * 
 * @param distance distance between 2 nodes expressed in kilometers(km)
 * @return ** double energy consumed experessed in KWh
 */
double calculateEnergyConsumed(double distance){
    return distance / prog_params.energy_consumption;
}

/**
 * @brief calculate the time spent during the travel between 2 nodes
 * 
 * @param distance distance between 2 nodes expressed in kilometers(km)
 * @return ** double time spent expressed in minutes
 */
double calculateTimeSpent(double distance){
    return (distance / prog_params.velocity) * 60; // convert to minutes
}

/**
 * @brief calculate the time to recharge
 * 
 * @param speed speed of chosen technology expressed in KWh/h
 * @param energy_refill total energy refill of sub-trip expressed in KWh
 * @return ** double total time it takes to charge expressed in minutes
 */
double calculateTimeRecharge(int speed, double energy_refill){
    cout << endl << "calculate speed: " << speed << " energy: " << energy_refill << " answer: " << (energy_refill / speed) * 60 << endl;
    return (energy_refill / speed) * 60; // convert to minutes
}

/**
 * @brief calculate the energy to refill based on the speed of the technology and time 
 * 
 * @param speed speed of the technology expressed in Kw
 * @param time time to refill expressed in minutes
 * @return ** double energy refill expressed in KWh
 */
double calculateEnergyRefill(int speed, double time){
    return (speed * time) / 60;
}

/**
 * @brief calculate the cost to recharge
 * 
 * @param cost cost of chosen technology expressed in Euro
 * @param energy_refill total energy refill of sub-trip expressed in KWh
 * @return ** double total cost it takes to charge expressed in Euro
 */
double calculateEnergyCost(double cost, double energy_refill){
    return energy_refill * cost;
}

/**
 * @brief Get the name attritbute
 * 
 * @param node Vehicle node
 * @return ** int 
 */
int getName(Vehicle node){
    int name = -1;

    switch (node.nodeType){
        case 'C':
            name = c_nodes[node.nodeIndx].getName();
            break;
        case 'R' : 
            name = r_nodes[node.nodeIndx].getName();
            break;
        default:
            break;
    }
    return name;
}

/**
 * @brief Set the Next Available Tech of the Recharge Station
 * 
 * @param station Recharge Station Node
 * @return ** void 
 */
void setNextAvailableTech(Charging_Node *station){
    int chosen_tech = (*station).getChosenTech();
    // cout << endl << (*station).getName() << "\told tech: " << chosen_tech;
    if(chosen_tech == 1 && (*station).has3()){
        (*station).setChosenTech(chosen_tech + 1);
        // cout << "\t new tech: " << (*station).getChosenTech();
    }
} 

Matrix *initializeDistanceMatrix(){
    Matrix *distance = (Matrix*)calloc(1, sizeof(Matrix)); //storage allocation is in heap
    int num_of_nodes = prog_params.num_of_customers + prog_params.num_of_recharge + 1; // +1 is the depot
    // nodes naming
    // 0 => depot
    // 1 - num_of_customers => customers
    // num_of_customers + 1 - num_of_recharge => recharge stations

    bool customer_done = false; // check if we are done traversing customer
    
    // traversal:
    // start with customer node pairing it with other customer nodes then pair it with the recharge stations
    int first_node = 0, second_node = 0;

    // traverse customer array first
    for (first_node = 0; first_node < c_nodes.size(); first_node++){
        Customer_Node x = c_nodes[first_node];
        second_node = first_node + 1;

        // pair customer node with another customer node
        for (; second_node < c_nodes.size(); second_node++){
            Customer_Node y = c_nodes[second_node];

            // get distance
            (*distance)[x.getName()][y.getName()] = (*distance)[y.getName()][x.getName()] = calculateDistance(x.getX(), x.getY(), y.getX(), y.getY());
        }

        // pair customer node with recharge stations
        for (second_node = 0; second_node < r_nodes.size(); second_node++){
            Charging_Node y = r_nodes[second_node];
           
            // get distance
            (*distance)[x.getName()][y.getName()] = (*distance)[y.getName()][x.getName()] = calculateDistance(x.getX(), x.getY(), y.getX(), y.getY());
        }
    }

    // traverse the recharge nodes
    for (first_node = 0; first_node < r_nodes.size() - 1; first_node++){
        // pair recharge nodes with the rest of recharge nodes
        Charging_Node x = r_nodes[first_node];

        for (second_node = first_node + 1; second_node < r_nodes.size(); second_node++){
            Charging_Node y = r_nodes[second_node];
  
            // get distance
            (*distance)[x.getName()][y.getName()] = (*distance)[y.getName()][x.getName()] = calculateDistance(x.getX(), x.getY(), y.getX(), y.getY());
        }
    }

    // cout << "\n\n" << "Distance Matrix: " << "\n";
    // for(int x = 0; x < num_of_nodes; x++){
    //     for(int y = 0; y < num_of_nodes; y++){
    //         cout << (*distance)[x][y] << "\t";
    //     }
    //     cout << "\n\n";
    // }

    return distance;
}

/**
 * @brief generate a random number given a range inclusive
 * 
 * @param start 
 * @param end 
 * @return ** int 
 */
int random(int start, int end){
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dis(start, end);

    return dis(gen);
}

Vehicle getDepot(){
    Vehicle depot(0, 'R', 0);

    return depot;
}

void displayFlamingoPopulation(vector<Flamingo> fl, string filename){
    ofstream file(filename);

    for(int flamingo = 0; flamingo < fl.size(); flamingo++){
        file << endl << "Flamingo " << flamingo << endl;
        file << "cost: " << fl[flamingo].cost << endl;
        file << "Vehicle List: " << endl;

        for(int vehicle = 0; vehicle < fl[flamingo].vehicleList.size(); vehicle++){
            file << "\tVehicle #" << vehicle << endl;

            for(int node = 0; node < fl[flamingo].vehicleList[vehicle].size(); node++){
                Vehicle v = fl[flamingo].vehicleList[vehicle][node];
                file << "\t\tindex: " << node << "\tnodeIndx: " << v.nodeIndx << "\tnodeType: " << v.nodeType << "\ttime: " << v.time << endl;
            }
        }
    }

    file.close();
}