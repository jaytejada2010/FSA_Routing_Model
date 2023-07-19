#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <string>

#include "reader.h"

#define CONSTANT_SIZE 500

typedef double Matrix[CONSTANT_SIZE][CONSTANT_SIZE];

double calculateDistance(int x1, int y1, int x2, int y2){
    return pow(x2 - x1, 2) + pow(y2 - y1, 2); //using euclidean distance between 2 points
}

double calculateEnergyConsumed(double distance){
    return distance / prog_params.energy_consumption;
}

double calculateTimeSpent(double distance){
    return distance / prog_params.velocity;
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
        cout << "x: " << x.getName() << " second: " << second_node << " c size: " << c_nodes.size() << "\n";

        // pair customer node with another customer node
        for (; second_node < c_nodes.size(); second_node++){
            Customer_Node y = c_nodes[second_node];
            cout << " yc: " << y.getName();
            // get distance
            (*distance)[x.getName()][y.getName()] = (*distance)[y.getName()][x.getName()] = calculateDistance(x.getX(), x.getY(), y.getX(), y.getY());
        }

        // pair customer node with recharge stations
        for (second_node = 0; second_node < r_nodes.size(); second_node++){
            Charging_Node y = r_nodes[second_node];
            cout << " yr: " << y.getName();
            // get distance
            (*distance)[x.getName()][y.getName()] = (*distance)[y.getName()][x.getName()] = calculateDistance(x.getX(), x.getY(), y.getX(), y.getY());
        }
        cout << "\n\n";
    }

    // traverse the recharge nodes
    for (first_node = 0; first_node < r_nodes.size() - 1; first_node++){
        // pair recharge nodes with the rest of recharge nodes
        Charging_Node x = r_nodes[first_node];
        cout << "x: " << x.getName();
        for (second_node = first_node + 1; second_node < r_nodes.size(); second_node++){
            Charging_Node y = r_nodes[second_node];
            cout << " yr: " << y.getName();
            // get distance
            (*distance)[x.getName()][y.getName()] = (*distance)[y.getName()][x.getName()] = calculateDistance(x.getX(), x.getY(), y.getX(), y.getY());
        }
        cout << "\n\n";
    }


    cout << "\n\n" << "Distance Matrix: " << "\n";
    for(int x = 0; x < num_of_nodes; x++){
        for(int y = 0; y < num_of_nodes; y++){
            cout << (*distance)[x][y] << "\t";
        }
        cout << "\n\n";
    }

    return distance;
}