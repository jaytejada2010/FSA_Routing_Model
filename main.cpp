#include "headers/sub_functions.h"

/**
 * THIS IS A PROGRAM THAT AIMS TO SOLVE THE GREEN VEHICLE ROUTING PROBLEM
 * USING THE FLAMINGO SEARCH ALGORITHM
 * 
 * GENERAL FLOW OF THE PROGRAM
 * 1 => Preprocess the data into the program and store into their                                  [done]
 *      respective data structures
 * 2 => Populate Flamingo population by generating initial feasible solutions                      [not yet started]
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



int main(){
    readFile("data/100/datos-10-N100.txt");
}