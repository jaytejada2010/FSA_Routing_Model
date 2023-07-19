#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include <string>

#include "reader.h"

double calculateDistance(int x1, int y1, int x2, int y2){
    return pow(x2 - x1, 2) + pow(y2 - y1, 2); //using euclidean distance between 2 points
}

double calculateEnergyConsumed(double distance){
    return distance / prog_params.energy_consumption;
}

double calculateTimeSpent(double distance){
    return distance / prog_params.velocity;
}