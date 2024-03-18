
#include <iostream>


#include "Network.hpp"
#include "Comunication.hpp"
#include "Simulation.hpp"

int main()
{

    Network network;

    network.load_routers("config.txt");
    // network.print_routers_info();
    Simulation::run_simulation(network);

    network.print_routers_info();

}
