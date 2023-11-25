#include "sub_functions.h"
#include "struct_def.h"

void migrateFlamingo(Flamingo *curr, Flamingo best);

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
    // get how many vehicles for this flamingo randomly
    int vehicles = ceil(((*curr).vehicleList.size() + best.vehicleList.size()) / 2);

    Flamingo fl(vehicles, prog_params.num_of_customers, prog_params.num_of_recharge);

    for (int vehicle = 0; vehicle < vehicles; vehicle++)
    {
        // start every route with a depot
        fl.insertNodetoVehicle(vehicle, getDepot());
    }

    while (!fl.unvisitedCustomers.empty())
    {
        // which vehicle to insert the node
        int vehicle = random(0, vehicles - 1);

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

    // insert replace current flamingo with new flamingo
    *curr = fl;
}

/**
 * @brief Migratory Suboperator
 * the migrating flamingo is compared with the leader flamingo and
 * an array of the differences is returned
 *
 * @param curr pass by value to avoid manipulation, best pass by value to avoid manipulation
 * @return ** void
 */

void getDifferenceArray(Flamingo curr, Flamingo best, int something)
{
    // creating the difference array
    int difference[100] = {0};

    // setting value of difference array as -1
    for (int i = 0; i < sizeof(difference); i++)
    {
        // elements with value -1 need not change
        difference[i] = -1;
    }

    // filling differenceArray with node differences
    for (int vehicle = 0; vehicle < curr.vehicleList.size(); vehicle++)
    {
        for (int node = 0; node < max(curr.vehicleList[vehicle].size(), best.vehicleList[vehicle].size()); node++)
        {
            if (curr.vehicleList[vehicle][node].nodeIndx != best.vehicleList[vehicle][node].nodeIndx)
            {
                int differenceIndex1 = curr.vehicleList[vehicle][node].nodeIndx;
                int differenceIndex2 = best.vehicleList[vehicle][node].nodeIndx;

                difference[differenceIndex1] = curr.vehicleList[vehicle][node].nodeIndx;
            }
        }
    }
}
