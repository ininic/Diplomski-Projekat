
#include <iostream>
#include <thread> 

#include "Network.hpp"
#include "Comunication.hpp"


int main()
{
    Network network;

    network.load_routers("config.txt");
    // network.print_routers_info();
    std::vector<std::thread> threads;

    for (int i = 0; i < network.routers.size(); i++)
    {
        thread t(Comunication::send_recv, ref(network.routers[i]));
        threads.push_back(move(t));
    }

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

}
