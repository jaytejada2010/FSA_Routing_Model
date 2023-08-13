#include "sub_functions.h"

/**
 * @brief Flamingo Operator
 * for every recharge station in the flamingo, choose the next available technology,
 * this will cost more but time will be reduced
 * 
 * @param fl 
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