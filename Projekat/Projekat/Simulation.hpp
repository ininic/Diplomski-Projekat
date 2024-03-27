#ifndef SIMULATION_DEF
#define SIMULATION_DEF

#include <thread> 
#include <filesystem>

#include "Network.hpp"
#include "Comunication.hpp"

using namespace std;

class Simulation
{
	public: 
      
		static void run_simulation(Network& network)
		{
            vector<thread> threads;

            remove_files(network);

            for (int i = 0; i < network.routers.size(); i++)
            {
                if (i == 5)
                {
                    thread t(Comunication::send_recv, ref(network.routers[i]), network.routers[i].devices[0].ip_addr, network.routers[8].devices[0].ip_addr);
                    threads.push_back(move(t));
                }
                else
                {
                    thread t(Comunication::send_recv, ref(network.routers[i]), 0, 0);
                    threads.push_back(move(t));
                }
               
            }

            for (int i = 0; i < threads.size(); i++)
            {
                threads[i].join();
            }
        
		}

        static void remove_files(Network& network)
        {
            for (int i = 0; i < network.routers.size(); i++)
            {
                string fname = to_string(network.routers[i].router_id) + ".txt";
                remove(fname.c_str());
            }

        }

};




#endif SIMULATION_DEF