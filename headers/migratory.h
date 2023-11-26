/**
 * @brief Migratory Suboperator
 * the migrating flamingo is compared with the leader flamingo and
 * an array of the differences is returned
 *
 * @param curr pass by value to avoid manipulation, best pass by value to avoid manipulation
 * @return ** void
 */

set<int> getDifferenceSet(Flamingo curr, Flamingo best)
{
    // creating the difference set
    set<int> difference;

    // filling difference set with node differences
    // iterate through the vehicles of the flamingos

    int vehicle = 0;

    for (; vehicle < min(curr.vehicleList.size(), best.vehicleList.size()); vehicle++)
    {
        // get length of shorter and longer vehicle lengths
        int shorterVehicleLength = min(curr.vehicleList[vehicle].size(), best.vehicleList[vehicle].size());
        int longerVehicleLength = max(curr.vehicleList[vehicle].size(), best.vehicleList[vehicle].size());

        int node = 0;

        // iterate through the nodes of the vehicles up to shorter vehicle length
        for (; node < shorterVehicleLength; node++)
        {
            // check if any if the nodes are the different
            if (curr.vehicleList[vehicle][node].nodeIndx != best.vehicleList[vehicle][node].nodeIndx)
            {
                if (curr.vehicleList[vehicle][node].nodeType == 'C')
                    difference.insert(curr.vehicleList[vehicle][node].nodeIndx);
                if (best.vehicleList[vehicle][node].nodeType == 'C')
                    difference.insert(best.vehicleList[vehicle][node].nodeIndx);
            }
        }

        // choose the longer vehicle
        vector<Vehicle> longVehicle = (curr.vehicleList[vehicle].size() == longerVehicleLength) ? curr.vehicleList[vehicle] : best.vehicleList[vehicle];

        // iterate through longer vehicle to insert all remaining customer nodes
        for (vector<Vehicle>::iterator longVehicleIterator = longVehicle.begin() + node; longVehicleIterator != longVehicle.end(); longVehicleIterator++)
        {
            if ((*longVehicleIterator).nodeType == 'C')
                difference.insert((*longVehicleIterator).nodeIndx);
        }
    }

    // selecting the larger flamingo
    vector<vector<Vehicle>> largerFlamingo = (curr.vehicleList.size() == max(curr.vehicleList.size(), best.vehicleList.size())) ? curr.vehicleList : curr.vehicleList;

    // iterating through the vehicles of the larger flamingo
    for (vector<vector<Vehicle>>::iterator largerFlamingoIterator = largerFlamingo.begin() + vehicle; largerFlamingoIterator != largerFlamingo.end(); largerFlamingoIterator++)
    {
        vector<Vehicle> currentVehicle = (*largerFlamingoIterator);
        for (vector<Vehicle>::iterator vehicleIterator = currentVehicle.begin(); vehicleIterator != currentVehicle.end(); vehicleIterator++)
        {
            if ((*vehicleIterator).nodeType == 'C')
                difference.insert((*vehicleIterator).nodeIndx);
        }
    }

    return difference;
}

/**
 * @brief Migratory Operator
 * the migrating flamingo is compared with the leader flamingo and
 * the difference in nodes is replaced accordingly
 *
 * @param curr pass by reference to manipulate, best pass by value to avoid manipulation
 * @return ** void
 */

void migrateFlamingo(Flamingo *curr, Flamingo best)
{
    set<int> difference = getDifferenceSet(*curr, best);

    // get how many vehicles in the flamingo by getting the ceiling of half of the sum of two sizes
    int vehicles = ceil(((*curr).vehicleList.size() + best.vehicleList.size()) / 2);

    Flamingo fl(vehicles, prog_params.num_of_customers, prog_params.num_of_recharge);

    // insert depots to start of vehicle routes
    for (int vehicle = 0; vehicle < vehicles; vehicle++)
    {
        // start every route with a depot
        fl.insertNodetoVehicle(vehicle, getDepot());
    }

    // migrating filling customer nodes
    // iterating through current flamingo's vehicles
    int v = 0;

    for (; v < (*curr).vehicleList.size(); v++)
    {
        int node_iter = 0;
        int current_vehicle_size = (*curr).vehicleList.size();
        // iterating through the current vehicle's nodes
        for (; node_iter < current_vehicle_size; node_iter++)
        {
            // defaults
            char nodeType = 'C';
            int nodeIndx = -1;
            int time = 0;

            int node = -1;

            // if customer node and not in difference set, copy to new flamingo
            if ((*curr).vehicleList[v][node_iter].nodeType == 'C' && difference.find((*curr).vehicleList[v][node_iter].nodeIndx) == difference.end())
            {
                // insert a customer
                node = (*curr).vehicleList[v][node_iter].nodeIndx;
                nodeIndx = fl.getCustomerFromSet(node);
                time = c_nodes[nodeIndx].getServiceTime();

                fl.visitCustomer(nodeIndx);
            }
            // else, insert another node
            else
            {
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
                    if (fl.vehicleList[v][fl.vehicleList[v].size() - 1].nodeType == 'C')
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
            }

            // insert only if the node is valid
            if (nodeIndx != -1)
            {
                Vehicle newNode(nodeIndx, nodeType, time);
                // insert node to the vehicle
                fl.insertNodetoVehicle(v, newNode);
            }
        }
    }
    // filling the remaining vahicles if new flamingo is greater than current flamingo
    while (!fl.unvisitedCustomers.empty())
    {
        // which vehicle to insert the node
        int vehicle = random(v, vehicles - 1);

        // defaults
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
            Vehicle insertedVehicle(nodeIndx, nodeType, time);
            // insert node to the vehicle
            fl.insertNodetoVehicle(vehicle, insertedVehicle);
        }
    }

    // inserting depots at end of routes
    for (int vehicle = 0; vehicle < vehicles; vehicle++)
    {
        // end every route with a depot
        fl.insertNodetoVehicle(vehicle, getDepot());
    }

    // insert replace current flamingo with new flamingo
    *curr = fl;
}
