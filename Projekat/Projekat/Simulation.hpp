#ifndef SIMULATION_DEF
#define SIMULATION_DEF

#include <thread> 

#include "Network.hpp"
#include "Comunication.hpp"
using namespace std;

class Simulation
{
	public: 
      
		static void run_simulation(Network& network)
		{
            vector<thread> threads;

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

};




#endif SIMULATION_DEF