
#include <iostream>
#include "Network.hpp"



int main()
{
    Network network;

    network.load_routers("config.txt");
    network.print_routers();
}

