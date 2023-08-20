#include "headers/foraging.h"
#include <iomanip>

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

Matrix* distance_matrix = NULL;

/**
 * @brief check feasibility of each vehicle route
 * 
 * @param v vehicle list
 * @return true if feasible
 * @return false if not feasible
 */
bool checkFesaibilityVehicle(vector<Vehicle> v){
    bool checkFeasibility = true;
    double total_distance = 0;
    double total_time = 0, travel_time = 0, service_time = 0, refill_time = 0;
    double total_energy = 0, total_energy_subtrip = 0;
    double total_demand = 0;
    cout << "\t size: " << v.size();
    // first node => first node of the current sub-trip
    for(int first_node = 0; first_node < v.size() - 1 && checkFeasibility;){
        cout << endl << "\tfirst node: " << first_node;
        total_energy_subtrip = 0;
        // second node => current node, it will stop if it will encounter another charging station (end of sub trip)
        for(int second_node = first_node + 1; second_node < v.size() && checkFeasibility; second_node++){
            cout << endl << "\t\tsecond node: " << second_node << " " << v[second_node].nodeType;
            // reset time variables for each node
            travel_time = service_time = refill_time = 0;

            double distance = (*distance_matrix)[getName(v[first_node])][getName(v[second_node])];
            double energy_consumption = calculateEnergyConsumed(distance); //accumulate energy consumption for the sub-trip

            total_distance += distance;
            travel_time = calculateTimeSpent(distance);
            total_energy_subtrip += energy_consumption;

            // cout << endl << "\t\t\tdistance " << distance << " \tenergy " << energy_consumption;

            if(v[second_node].nodeType == 'R'){
                // evaluate energy of subtrip only if the end of the subtrip is found (another charging station)
                if(total_energy_subtrip > prog_params.battery_max){
                    cout << endl << "exceed batt" << endl;
                    checkFeasibility = false;
                    break;
                }

                // calculate time to refill the needed 
                cout << "\ttravel time: " << travel_time << "\tservice time: " << service_time << "\trefill time: " << refill_time << "\tchosen tech: " << r_nodes[v[first_node].nodeIndx].getChosenTech() << "\tname: " << r_nodes[v[first_node].nodeIndx].getName() << "\tspeed: " << tech_list[r_nodes[v[first_node].nodeIndx].getChosenTech()].getSpeed();
                int chosen_speed = tech_list[r_nodes[v[first_node].nodeIndx].getChosenTech()].getSpeed();
                v[first_node].time = refill_time = calculateTimeRecharge(chosen_speed, total_energy_subtrip);
                first_node = second_node;
                total_time += travel_time + refill_time;
                cout << "\ttravel time: " << travel_time << "\tservice time: " << service_time << "\trefill time: " << refill_time << "\tchosen tech: " << r_nodes[v[first_node].nodeIndx].getChosenTech() << "\tname: " << r_nodes[v[first_node].nodeIndx].getName() << "\tspeed: " << tech_list[r_nodes[v[first_node].nodeIndx].getChosenTech()].getSpeed();
                cout << endl << "total time: " << total_time;
                break;
            }else if(v[second_node].nodeType == 'C'){
                // get the demand of the customer and their service time
                total_demand = c_nodes[v[second_node].nodeIndx].getDemand();
                v[second_node].time = service_time = c_nodes[v[second_node].nodeIndx].getServiceTime();
                total_time += travel_time + service_time;
                cout << "\ttravel time: " << travel_time << "\tservice time: " << service_time << "\trefill time: " << refill_time;
                cout << endl << "total time: " << total_time;
            }
        }
    }

    if(total_demand > prog_params.vehicle_capacity || total_time > prog_params.time_max)
        checkFeasibility = false;

    
    cout << endl << "\ttotal demand: " << total_demand << "("<<prog_params.vehicle_capacity<<")" << "\ttotal time: " << total_time << "("<<prog_params.time_max<<")";
    return checkFeasibility;
}

/**
 * @brief check feasibilty of the flamingo population
 * 
 * @return true if feasible
 * @return false if not feasible
 */
bool checkFeasibilityFlamingo(){
    bool checkFeasibility = true;
    cout << f.size();
    // check the feasibility of each flamingo
    for(int flamingo = 0; flamingo < f.size() && checkFeasibility; flamingo++){
        cout << endl << "Flamingo: " << flamingo;
        // evaluate each vehicle in the vehicle list of the flamingo
        for(int vehicle = 0; vehicle < f[flamingo].vehicleList.size() && checkFeasibility; vehicle++){
            cout << endl << "Vehicle: " << vehicle;
            checkFeasibility = checkFesaibilityVehicle(f[flamingo].vehicleList[vehicle]);
        }
    }
    return checkFeasibility;
}

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
            int time = 0;

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
    cout << fixed << setprecision(9);
    distance_matrix = initializeDistanceMatrix(); //create a distance matrix beforehand

    number_of_nodes = prog_params.num_of_customers + prog_params.num_of_recharge + 1;

    populateFlamingo();
    displayFlamingoPopulation(f, "flamingo_population.txt");
    
    for(int flamingo = 0; flamingo < f.size(); flamingo++){
        cout << endl << "Flamingo " << flamingo;
        balanceVehicleRoutes(&f[flamingo]);
    }
cout << "\ndone";
    bool check = checkFeasibilityFlamingo();
    cout << endl << "check is " << check;
}
