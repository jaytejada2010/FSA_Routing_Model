    distance_matrix = initializeDistanceMatrix(); //create a distance matrix beforehand

    number_of_nodes = prog_params.num_of_customers + prog_params.num_of_recharge + 1;

    populateFlamingo();
    displayFlamingoPopulation(f, "flamingo_population.txt");

    bool check = checkFesaibilityVehicle(f[0].vehicleList[0]);