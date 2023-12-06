#include "headers/foraging.h"
#include "headers/migratory.h"
#include <iomanip>

// Scores
#define GOLD 5
#define SILVER 3
#define BRONZE 1

/**
 * THIS IS A PROGRAM THAT AIMS TO SOLVE THE GREEN VEHICLE ROUTING PROBLEM
 * USING THE FLAMINGO SEARCH ALGORITHM
 *
 * GENERAL FLOW OF THE PROGRAM
 * 1 => Preprocess the data into the program and store into their                                  [done]
 *      respective data structures
 * 2 => Populate Flamingo population                                                               [done]
 * 3 => Calculate cost of each flamingo                                                            [done]
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
double group_ratio = 0.8;          // ratio when diving flamingos into migrating and foraging
float convergence_criterion = 0.9; // to check the if all flamingos has passed the optimal solution

// division of flamingo into foraging and migratory
int middle = 0;
int f_start = 0, f_end = 0;
int m_start = 0, m_end = 0;

vector<Flamingo> f, prevRank;
vector<double> maxDiff;
vector<double> leader_cost;
int number_of_nodes = 0;

Matrix *distance_matrix = NULL;

/**
 * @brief check feasibility of each vehicle route
 *
 * @param v vehicle list
 * @return true if feasible
 * @return false if not feasible
 */

bool checkFesaibilityVehicle(vector<Vehicle> v, double *cost)
{
    bool checkFeasibility = true;
    double total_distance = 0;
    double total_time = 0, travel_time = 0, service_time = 0, refill_time = 0;
    double total_energy = 0, total_energy_subtrip = 0;
    double total_demand = 0;
    // cout << "\t size: " << v.size();
    // first node => first node of the current sub-trip
    for (int first_node = 0; first_node < v.size() - 1 && checkFeasibility;)
    {
        // cout << endl
        //      << "\tfirst node: " << first_node;
        total_energy_subtrip = 0;
        // second node => current node, it will stop if it will encounter another charging station (end of sub trip)
        for (int second_node = first_node + 1; second_node < v.size() && checkFeasibility; second_node++)
        {
            // cout << endl
            //      << "\t\tsecond node: " << second_node << " " << v[second_node].nodeType;
            // reset time variables for each node
            travel_time = service_time = refill_time = 0;

            double distance = (*distance_matrix)[getName(v[first_node])][getName(v[second_node])];
            double energy_consumption = calculateEnergyConsumed(distance); // accumulate energy consumption for the sub-trip

            total_distance += distance;
            travel_time = calculateTimeSpent(distance);
            total_energy_subtrip += energy_consumption;

            // cout << endl << "\t\t\tdistance " << distance << " \tenergy " << energy_consumption;

            if (v[second_node].nodeType == 'R')
            {
                // evaluate energy of subtrip only if the end of the subtrip is found (another charging station)
                if (total_energy_subtrip > prog_params.battery_max)
                {
                    cout << endl
                         << "exceed batt" << endl;
                    checkFeasibility = false;
                    break;
                }

                // calculate time to refill the needed
                // cout << "\ttravel time: " << travel_time << "\tservice time: " << service_time << "\trefill time: " << refill_time << "\tchosen tech: " << r_nodes[v[first_node].nodeIndx].getChosenTech() << "\tname: " << r_nodes[v[first_node].nodeIndx].getName() << "\tspeed: " << tech_list[r_nodes[v[first_node].nodeIndx].getChosenTech()].getSpeed();
                int chosen_speed = tech_list[r_nodes[v[first_node].nodeIndx].getChosenTech()].getSpeed();
                v[first_node].time = refill_time = calculateTimeRecharge(chosen_speed, total_energy_subtrip);
                first_node = second_node;
                total_time += travel_time + refill_time;
                // cout << "\ttravel time: " << travel_time << "\tservice time: " << service_time << "\trefill time: " << refill_time << "\tchosen tech: " << r_nodes[v[first_node].nodeIndx].getChosenTech() << "\tname: " << r_nodes[v[first_node].nodeIndx].getName() << "\tspeed: " << tech_list[r_nodes[v[first_node].nodeIndx].getChosenTech()].getSpeed();
                // cout << endl
                //      << "total time: " << total_time;

                // calculate the cost of the refill
                double chosen_cost = tech_list[r_nodes[v[first_node].nodeIndx].getChosenTech()].getCost();
                (*cost) += calculateEnergyCost(chosen_cost, total_energy_subtrip);
                break;
            }
            else if (v[second_node].nodeType == 'C')
            {
                // get the demand of the customer and their service time
                total_demand = c_nodes[v[second_node].nodeIndx].getDemand();
                v[second_node].time = service_time = c_nodes[v[second_node].nodeIndx].getServiceTime();
                total_time += travel_time + service_time;
                // cout << "\ttravel time: " << travel_time << "\tservice time: " << service_time << "\trefill time: " << refill_time;
                // cout << endl
                //      << "total time: " << total_time;
            }
        }
    }

    if (total_demand > prog_params.vehicle_capacity || total_time > prog_params.time_max)
    {
        checkFeasibility = false;
        if (total_demand > prog_params.vehicle_capacity)
            cout << "Demand exceeded capacity. " << total_demand << endl;
        if (total_time > prog_params.time_max)
            cout << "Travel time exceeded max. " << total_time << endl;
    }

    // cout << endl
    //      << "\ttotal demand: " << total_demand << "(" << prog_params.vehicle_capacity << ")"
    //      << "\ttotal time: " << total_time << "(" << prog_params.time_max << ")";
    return checkFeasibility;
}

/**
 * @brief check feasibilty of the given flamingo
 *
 * @return true if feasible
 * @return false if not feasible
 */
bool checkFeasibilityEachFlamingo(Flamingo *fl)
{
    bool checkFeasibility = true;
    // initialize with the fixed cost
    double cost = prog_params.fixed_cost_recharge * (*fl).vehicleList.size();
    // cout << " size " << f.size();
    // check the feasibility of each flamingo
    for (int vehicle = 0; vehicle < (*fl).vehicleList.size() && checkFeasibility; vehicle++)
    {
        // cout << endl
        //      << "Vehicle: " << vehicle;
        checkFeasibility = checkFesaibilityVehicle((*fl).vehicleList[vehicle], &cost);
    }

    // update cost
    (*fl).cost = cost;
    // cout << endl
    //      << "total cost " << (*fl).cost << endl;

    return checkFeasibility;
}

/**
 * @brief check feasibilty of the flamingo population
 *
 * @return true if feasible
 * @return false if not feasible
 */
bool checkFeasibilityFlamingo()
{
    bool checkFeasibility = true;
    // cout << f.size();
    // check the feasibility of each flamingo
    for (int flamingo = 0; flamingo < f.size() && checkFeasibility; flamingo++)
    {
        // cout << endl
        //      << "Flamingo: " << flamingo;
        checkFeasibilityEachFlamingo(&f[flamingo]);
    }
    return checkFeasibility;
}

/**
 * @brief get the node index of the nearest node to the previous node
 *
 * @param customer_list list of customer nodes in the vehicle
 * @param previousNode name / index on the distance matrix of previous node
 * @return ** int node of nearest customer
 */
void arrangeNodes(vector<vector<Vehicle>> *v_list)
{

    // iterate through vehicleList
    for (int x = 0; x < (*v_list).size(); x++)
    {
        // get list of customer nodes visited by vehicle
        set<int> customer_list;
        for (int ctr = 0; ctr < (*v_list)[x].size(); ctr++)
        {
            if ((*v_list)[x][ctr].nodeType == 'C')
                customer_list.insert((*v_list)[x][ctr].nodeIndx);
        }

        // iterate through vehicle
        for (vector<Vehicle>::iterator node = (*v_list)[x].begin() + 1; node != (*v_list)[x].end(); node++)
        {
            if ((*node).nodeType == 'C')
            {
                Vehicle nearest_customer = (*node);
                Vehicle previousNode = (*(next(node, -1)));
                set<int>::iterator x = customer_list.begin();

                // iterate through the customer_list to look for the nearest node
                for (; x != customer_list.end(); x++)
                {
                    int current_node = (*x);
                    if ((*distance_matrix)[getName(previousNode)][c_nodes[current_node].getName()] < (*distance_matrix)[getName(previousNode)][getName(nearest_customer)])
                        current_node = nearest_customer.nodeIndx;
                }

                (*node).nodeIndx = nearest_customer.nodeIndx;
                (*node).time = c_nodes[nearest_customer.nodeIndx].getServiceTime();
                customer_list.erase(nearest_customer.nodeIndx);
            }
        }
    }
}

/**
 * @brief generate initial population of flamingo
 *
 * customers are all visited atleast once
 * recharge stations (except depot) are visited at most once
 *
 * @return ** void
 */
void populateFlamingo()
{
    int flamingos = prog_params.num_of_vehicles;

    for (int flamingo = 0; flamingo < flamingos; flamingo++)
    {
        bool feasibility = false;
        Flamingo fl;
        do
        {
            // get how many vehicles for this flamingo randomly
            int vehicles = prog_params.num_of_vehicles;
            // int vehicles = random(1, prog_params.num_of_vehicles);
            // int vehicles = 25;

            fl = Flamingo(vehicles, prog_params.num_of_customers, prog_params.num_of_recharge);

            for (int vehicle = 0; vehicle < vehicles; vehicle++)
            {
                // start every route with a depot
                fl.insertNodetoVehicle(vehicle, getDepot());
            }

            while (!fl.unvisitedCustomers.empty())
            {
                // which vehicle to insert the node
                int vehicle = random(0, vehicles - 1);

                char nodeType = 'C';
                int nodeIndx = -1;
                int time = 0;

                int node = -1;

                if (fl.unvisitedRecharge.size() > 1)
                {
                    nodeType = (random(0, 1) == 0) ? 'C' : 'R';
                }

                if (nodeType == 'C')
                {
                    // insert a customer
                    node = random(0, fl.unvisitedCustomers.size() - 1);
                    nodeIndx = fl.getCustomerFromSet(node);
                    time = c_nodes[nodeIndx].getServiceTime();

                    fl.visitCustomer(nodeIndx);
                }
                else if (nodeType == 'R')
                {
                    // make sure no charging stations are consecutive of each other
                    if (fl.vehicleList[vehicle][fl.vehicleList[vehicle].size() - 1].nodeType == 'C')
                    {
                        // insert a recharge station but not the depot
                        node = random(1, fl.unvisitedRecharge.size() - 1);
                        nodeIndx = fl.getRechargeFromSet(node);

                        fl.visitRecharge(nodeIndx);
                    }
                }
                else
                {
                    cout << "Invalid node type at " << nodeType << endl;
                    exit(0);
                }

                // insert only if the node is valid
                if (nodeIndx != -1)
                {
                    Vehicle v(nodeIndx, nodeType, time);
                    // insert node to the vehicle
                    fl.insertNodetoVehicle(vehicle, v);
                }
            }

            for (int vehicle = 0; vehicle < vehicles; vehicle++)
            {
                // end every route with a depot
                fl.insertNodetoVehicle(vehicle, getDepot());
            }
            arrangeNodes(&(fl.vehicleList));
            feasibility = checkFeasibilityEachFlamingo(&fl);
        } while (!feasibility);

        // insert flamingo to the population
        f.push_back(fl);
    }
}

/**
 * @brief comparator function for ranking flamingos from least cost to greatest cost
 * @param f1 FLamingo; first flamingo
 * @param f2 Flamingo; second flamingo
 *
 * @return ** bool
 */
bool rankFlamingos(Flamingo f1, Flamingo f2)
{
    return (f1.cost < f2.cost);
}

void flamingoSearchAlgorithm(vector<Flamingo> *f)
{
    // initilize scoring

    // get ranking
    prevRank = (*f);
    (*f).clear();

    // make foraging operator history
    vector<int> ops;

    // divide the flamingo into 2 groups
    middle = ceil(prevRank.size() * group_ratio);
    // cout << endl
    //      << "middle " << middle;
    // foraging group
    f_start = 0, f_end = middle;
    // cout << endl
    //      << "foraging: " << f_start << " " << f_end;
    // migrating group
    m_start = middle, m_end = prevRank.size();
    // cout << endl
    //      << "migrating: " << m_start << " " << m_end;
    // update foraging group
    for (int x = f_start; x < f_end; x++)
    {
        bool feasibility = false;
        int op = 0;
        double current_cost = prevRank[x].cost;
        Flamingo temp; // temporary flamingo, only change the current flamingo if it is feasible

        // cout << endl
        //      << " Foraging: " << x;
        int count = 0;
        // pick a random operator and update
        // if not feasible, update again until it becomes feasible

        do
        {
            // the temporary flamingo will always start at the current flamingo
            // if it is not feasible, it will go back to the current flamingo
            // ex.
            // current: 0 3 4 2 0 6 5 0
            // temp after the update: 0 3 4 0 2 6 5 0 (it will not update from here, if this is not feasible)
            // the temp is not feasible
            // temp will go back to: 0 3 4 2 0 6 5 0 and update again
            temp = prevRank[x];
            op = callForagingOperator(&temp);
            ops.push_back(op);

            // cout << endl
            //      << "foraging count " << count;
            count++;
            // check feasibilty, calculating the cost is inside the checkFeasibilityEachFlamingo()
            feasibility = checkFeasibilityEachFlamingo(&temp);

            // go back to previous rank if it reaches 1000 iterations
            if (count == 25)
            {
                temp = prevRank[x];
                break;
            }

        } while (!feasibility || temp.cost > prevRank[x].cost); //

        // update current flamingo
        (*f).push_back(temp);

        // cout << endl
        //      << "feasiblity " << feasibility;
        int score = BRONZE;
        // evaluate and score the operator
        if (prevRank[0].cost > temp.cost)
        {
            score = GOLD; // if the new cost is better than the leader flamingo
        }
        else if (current_cost > temp.cost)
        {
            score = SILVER; // if the new cost is better than the current flamingo
        }
        // cout << endl
        //      << "score " << score;
        // update score of flamingo operator
        foraging.setScore(op, score);
        // foraging.displayScores();
    }

    // update migrating group
    for (int x = m_start; x < m_end; x++)
    {
        bool feasibility = false;
        Flamingo currentFlamingo, bestFlamingo; // temporary flamingo, only change the current flamingo if it is feasible

        int count = 0;

        // if not feasible, update again until it becomes feasible
        do
        {
            // the temporary flamingo will always start at the current flamingo
            // if it is not feasible, it will go back to the current flamingo
            // ex.
            // current: 0 3 4 2 0 6 5 0
            // temp after the update: 0 3 4 0 2 6 5 0 (it will not update from here, if this is not feasible)
            // the temp is not feasible
            // temp will go back to: 0 3 4 2 0 6 5 0 and update again
            currentFlamingo = prevRank[x];
            bestFlamingo = prevRank[0];

            migrateFlamingo(&currentFlamingo, bestFlamingo);
            arrangeNodes(&(currentFlamingo.vehicleList));

            // cout << endl
            //      << x << " migratory count " << count;
            count++;
            // check feasibilty, calculating the cost is inside the checkFeasibilityEachFlamingo()
            feasibility = checkFeasibilityEachFlamingo(&currentFlamingo);

            // go back to previous rank if it reaches 1000 iterations
            if (count == 25)
            {
                currentFlamingo = prevRank[x];
                break;
            }
        } while (!feasibility || currentFlamingo.cost > prevRank[x].cost);

        // update current flamingo
        (*f).push_back(currentFlamingo);

        // cout << endl
        //      << "feasiblity " << feasibility;
    }

    // for (int ctr = 0; ctr < (*f).size(); ctr++)
    // {
    //     string f_type = (ctr < m_start) ? "foraging" : "migrating";
    //     cout << endl
    //          << f_type << " flamingo [" << ctr << "]"
    //          << " previous (" << prevRank[ctr].vehicleList.size() << ") cost: " << prevRank[ctr].cost << " current (" << (*f)[ctr].vehicleList.size() << ") cost: " << (*f)[ctr].cost << " Delta: " << ((*f)[ctr].cost - (*f)[0].cost) / (*f)[ctr].cost;

    //     if (ctr < m_start)
    //         cout << "opeartor: " << ops[ctr];
    // }
    // foraging.displayScores();
}

bool isConverged(int *iterations)
{
    // bool isConvergent = false;

    // float flamingo_delta = f[0].cost - prevRank[0];
    // cout << " " << prevRank.size() << " " << prevRank[prevRank.size()].cost << endl;
    (*iterations)++;
    // cout << endl
    //      << "iteration: " << (*iterations);
    set<double> diff_set;
    double diff_indiv, sum = 0;
    for (int x = 0; x < f.size(); x++)
    {
        diff_indiv = (prevRank[x].cost - f[x].cost) / prevRank[x].cost;
        sum += diff_indiv;
        diff_set.insert(diff_indiv);
        // cout << prevRank[x].cost << " " << f[x].cost << " " << prevRank[x].cost - f[x].cost << endl;
    }

    // for (set<double>::iterator d = diff_set.begin(); d != diff_set.end(); d++)
    // {
    //     cout << endl
    //          << "Max: " << *d;
    // }
    maxDiff.push_back(*diff_set.rbegin());

    // cout << "Average: " << sum / f.size() << endl;

    return (*diff_set.rbegin() > 0) ? true : false;
    // return (f[m_start].cost - f[0].cost) / f[0].cost > 0.15 ? true : false;
    // return (*iterations) < 50 ? true : false;
}

void printCosts(string filename)
{
    ofstream file;
    file.open(filename, ios::app);

    int ctr = 0;
    for (vector<double>::iterator l = leader_cost.begin(); l != leader_cost.end(); l++, ctr++)
    {
        file << endl
             << "Iteration " << ctr << " Leader Cost: " << *l;
    }

    file.close();
}

void readFiles(string filename)
{
    maxDiff.clear();
    leader_cost.clear();
    clock_t begin = clock();
    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    cout << "Opening " << filename << endl;
    readFile(filename);

    cout << fixed << setprecision(9);
    distance_matrix = initializeDistanceMatrix(); // create a distance matrix beforehand

    number_of_nodes = prog_params.num_of_customers + prog_params.num_of_recharge + 1;

    populateFlamingo();

    // ranking the flamingo
    sort(f.begin(), f.end(), rankFlamingos);
    leader_cost.push_back(f[0].cost);

    string output_file = filename.erase(filename.find(".txt"), 4) + "_flamingo_population.txt";
    cout << "print on file " << output_file.erase(0, 9) << endl;

    int iterations = 0;
    do
    {
        flamingoSearchAlgorithm(&f);
        sort(f.begin(), f.end(), rankFlamingos);
        leader_cost.push_back(f[0].cost);
        // displayFlamingoPopulation(f, output_file, iterations);
    } while (isConverged(&iterations));

    /* here, do your time-consuming job */
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << endl
         << filename << " time spent: " << time_spent << " seconds";
    printCosts(output_file);
    printTime(maxDiff, output_file, time_spent);

    cout << "done flamingo" << endl;
}

int main()
{
    string data[] = {"200"};
    for (string n_count : data)
    {
        for (int x = 20; x < 30; x++)
        {
            string filename = "data/" + n_count + "/datos-" + to_string(x) + "-N" + n_count + ".txt";
            readFiles(filename);
        }
    }

    // readFiles("data/100/datos-10-N100.txt");

    // bool check = checkFeasibilityFlamingo();
    //     cout << endl << "check is " << check;
    //     cout << endl << endl << endl;
    //     for(int flamingo = 0; flamingo < f.size(); flamingo++){
    //         cout << endl << "Flamingo " << flamingo;
    //         balanceChargingStations(&f[flamingo]);
    //     }
    // cout << "\ndone";
    //      check = checkFeasibilityFlamingo();
    //     cout << endl << "check is " << check;
}
