#include "sub_functions.h"

/**
 * @brief Flamingo Operator
 * for every recharge station in the flamingo, choose the next available technology,
 * this will cost more but time will be reduced
 * 
 * @param fl pass by reference to manipulate
 * @return ** void 
 */
void chooseDifferentTechnology(Flamingo *fl) {
    // Traverse the whole vehicle list of the flamingo
    for (int vehicle = 0; vehicle < (*fl).vehicleList.size(); vehicle++) {
        for (int node = 1; node < (*fl).vehicleList[vehicle].size() - 1; node++) {
            // If recharge station is found, choose the next available technology
            if ((*fl).vehicleList[vehicle][node].nodeType == 'R') {
                int nodeIndx = (*fl).vehicleList[vehicle][node].nodeIndx;
                setNextAvailableTech(&r_nodes[nodeIndx]);
            }
        }
    }
}

/**
 * @brief Flamingo Operator
 * given a flamingo, find the shortest and longest route and balance them where their new sizes difference should not 
 * be bigger than 1
 * 
 * ex. 
 * V1 => size of 6
 * V2 => size of 2
 * 
 * we move 2 nodes from V1 to V2 so that
 * V1 => 6 - 2 = 4
 * V2 => 2 + 2 = 4 
 * 
 * @param fl 
 * @return ** void 
 */
void balanceVehicleRoutes(Flamingo *fl){
    int shortest_ndx = 0, longest_ndx = 0;

    // find the shortest and longest vehicle route
    for(int vehicle = 0; vehicle < (*fl).vehicleList.size(); vehicle++){
        cout << endl << "\t vehicle size: " << (*fl).vehicleList[vehicle].size();
        // find the shortest vehicle route
        if((*fl).vehicleList[vehicle].size() < (*fl).vehicleList[shortest_ndx].size()){
            shortest_ndx = vehicle;
        }

        // find the longest vehicle route
        if((*fl).vehicleList[vehicle].size() >= (*fl).vehicleList[longest_ndx].size()){
            longest_ndx = vehicle;
        }
    }

    // find the balance needed for the longest and shortest route
    int difference = getDifferenceInSize((*fl).vehicleList[shortest_ndx].size(), (*fl).vehicleList[longest_ndx].size());

    for(int x = 0; x < difference;){
        // get random node from the longest route
        int nodeIndx = random(1, (*fl).vehicleList[longest_ndx].size() - 2);
        // insert randomly to the shortest route
        int insertIndx = random(1, ((*fl).vehicleList[shortest_ndx].size() > 2) ? (*fl).vehicleList[shortest_ndx].size() - 2 : 1); //if shortest route size > 2 choose randomly, otherwise choose the first node
        if((*fl).vehicleList[longest_ndx][nodeIndx].nodeType == 'C' || ((*fl).vehicleList[shortest_ndx][insertIndx].nodeType != 'R' && (*fl).vehicleList[shortest_ndx][insertIndx - 1].nodeType != 'R')){
            (*fl).vehicleList[shortest_ndx].insert((*fl).vehicleList[shortest_ndx].begin() + insertIndx, (*fl).vehicleList[longest_ndx][nodeIndx]); // insert to shortest route
            (*fl).vehicleList[longest_ndx].erase((*fl).vehicleList[longest_ndx].begin() + nodeIndx); // remove from longest route
            x++;
        }
    }
}

void balanceChargingStations(Flamingo *fl){
    for(int vehicle = 0; vehicle < (*fl).vehicleList.size(); vehicle++){
        // initialize starting and end points of the shortest and longest sub-trip
        int short_start = 0, short_end = (*fl).vehicleList[vehicle].size() - 1; 
        int long_start = 0, long_end = 0;

        // find the longest and shortest sub-trip
        for(int first = 0; first < (*fl).vehicleList[vehicle].size() - 1; ){
            int second;
            // find the end of the subtrip
            for(second = first + 1; (*fl).vehicleList[vehicle][second].nodeType != 'R'; second++){}
            // find the shortest sub-trip
            if(short_end - short_start > second - first){
                short_start = first;
                short_end = second;
            }
            // find the longest sub-trip
            if(long_end - long_start < second - first){
                long_start = first;
                long_end = second;
            }
            first = second;
        }

        int ndxToMove;
        if (short_start == 0 && short_end == (*fl).vehicleList[vehicle].size() - 1){
            ndxToMove = -1; // if the route has no charging station in the middle
        }else if (short_start != 0 && short_end == (*fl).vehicleList[vehicle].size() - 1){
            ndxToMove = short_start; // if the ending node is the end of the route
        } else {
            ndxToMove = short_end; // the default
        }

        if(ndxToMove != -1){
            // move the node in the middle of the longest route
            Vehicle node = (*fl).vehicleList[vehicle][ndxToMove];
            int ndxToInsert = ceil((long_end + long_start) / 2); // find the middle index of the longest route
            (*fl).vehicleList[vehicle].insert((*fl).vehicleList[vehicle].begin() + ndxToInsert, node);
            (*fl).vehicleList[vehicle].erase((*fl).vehicleList[vehicle].begin() + ndxToMove);
        }
    }
}