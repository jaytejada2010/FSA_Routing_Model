#include "headers/sub_functions.h"

/**
 * THIS IS A PROGRAM THAT AIMS TO SOLVE THE GREEN VEHICLE ROUTING PROBLEM
 * USING THE FLAMINGO SEARCH ALGORITHM
 * 
 * GENERAL FLOW OF THE PROGRAM
 * 1 => Preprocess the data into the program and store into their                                  [done]
 *      respective data structures
 * 2 => Populate Flamingo population                                                               [done]
 * 3 => Calculate cost of each flamingo                                                            [not yet started]
 * 4 => Assign a leader flamingo with the least cost                                               [not yet started]
 * 5 => Divide the flamingos into migrating and foragin flamingos based on group ratio             [not yet started]
 * 6.1 => Update migrating flamingos                                                               [not yet started]
 * 6.2 => Update foraging flamingos                                                                [not yet started]
 * 7 => Check feasibility of each flamingo                                                         [not yet started]
 * 8 => If convergence criterion is met, go to 9                                                   [not yet started]
 *      if not, go back to 3 
 * 9 => return last Leader Flamingo                                                                [not yet started]
 */

// global parameters
int group_ratio = 2; // ratio when diving flamingos into migrating and foraging
float convergence_criterion = 0.9; // to check the if all flamingos has passed the optimal solution

vector<Flamingo> f;
int number_of_nodes = 0;

/**
 * @brief generate initial population of flamingo
 * 
 * customers are all visited atleast once
 * recharge stations (except depot) are visited at most once
 * 
 * @return ** void 
 */
void populateFlamingo(){
    int flamingos = number_of_nodes;

    for(int flamingo = 0; flamingo < flamingos; flamingo++){
        // get how many vehicles for this flamingo randomly
        int vehicles = random(1, prog_params.num_of_vehicles);

        Flamingo fl(vehicles, prog_params.num_of_customers, prog_params.num_of_recharge);

        for(int vehicle = 0; vehicle < vehicles; vehicle++){
            // start every route with a depot
            fl.insertNodetoVehicle(vehicle, getDepot());
        }

        while(!fl.unvisitedCustomers.empty()){
            // which vehicle to insert the node
            int vehicle = random(0, vehicles - 1);

            char nodeType = 'C';
            int nodeIndx = -1;
            int time = 0; // not sure what time is yet

            int node = -1;

            if(fl.unvisitedRecharge.size() > 1){
                nodeType = (random(0, 1) == 0) ? 'C' : 'R';
            }

            if(nodeType == 'C'){
                // insert a customer
                node = random(0, fl.unvisitedCustomers.size() - 1);
                nodeIndx = fl.getCustomerFromSet(node);
                time = c_nodes[nodeIndx].getServiceTime();

                fl.visitCustomer(nodeIndx);
            }else if(nodeType == 'R'){
                // make sure no charging stations are consecutive of each other
                if(fl.vehicleList[vehicle][fl.vehicleList[vehicle].size() - 1].nodeType == 'C'){
                    // insert a recharge station but not the depot
                    node = random(1, fl.unvisitedRecharge.size() - 1);
                    nodeIndx = fl.getRechargeFromSet(node);

                    fl.visitRecharge(nodeIndx);
                }
            }else {
                cout << "Invalid node type at " << nodeType << endl;
                exit(0);
            }

            // insert only if the node is valid
            if (nodeIndx != -1){
                Vehicle v(nodeIndx, nodeType, time);
                // insert node to the vehicle
                fl.insertNodetoVehicle(vehicle, v);
            }
        }

        for(int vehicle = 0; vehicle < vehicles; vehicle++){
            // end every route with a depot
            fl.insertNodetoVehicle(vehicle, getDepot());
        }

        // insert flamingo to the population
        f.push_back(fl);
    }
}

int main(){
    readFile("data/100/datos-10-N100.txt");
    initializeDistanceMatrix(); //create a distance matrix beforehand

    number_of_nodes = prog_params.num_of_customers + prog_params.num_of_recharge + 1;

    populateFlamingo();
    displayFlamingoPopulation(f, "flamingo_population.txt");
}
