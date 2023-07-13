#include "headers/reader.h"

/**
 * THIS IS A PROGRAM THAT AIMS TO SOLVE THE GREEN VEHICLE ROUTING PROBLEM
 * USING THE FLAMINGO SEARCH ALGORITHM
 * 
 * GENERAL FLOW OF THE PROGRAM
 * 1 => Preprocess the data into the program and store into their
 *      respective data structures
 * 2 => Populate Flamingo population by generating initial feasible solutions
 * 3 => Calculate cost of each flamingo
 * 4 => Assign a leader flamingo with the least cost
 * 5 => Divide the flamingos into migrating and foragin flamingos based on group ratio
 * 6.1 => Update migrating flamingos
 * 6.2 => Update foraging flamingos
 * 7 => Check feasibility of each flamingo
 * 8 => If convergence criterion is met, go to 9
 *      if not, go back to 3
 * 9 => return last Leader Flamingo
 */

// global parameters
int group_ratio = 2; // ratio when diving flamingos into migrating and foraging
float convergence_criterion = 0.9; // to check the if all flamingos has passed the optimal solution

int main(){
    readFile("data/100/datos-10-N100.txt");
}