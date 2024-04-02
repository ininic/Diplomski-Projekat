#ifndef SIMULATION_DEF
#define SIMULATION_DEF

#include <thread> 
#include <filesystem>

#include "Network.hpp"
#include "Comunication.hpp"

#define SOURCE_IP_ADDR "223.1.4.1"
#define SOURCE_ROUTER_ID 104
#define DESTINATION_IP_ADDR "223.1.1.1"


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
                if (network.routers[i].router_id == SOURCE_ROUTER_ID)
                {
                    thread t(Comunication::send_recv, ref(network.routers[i]), Conversions::convert_string_tp_ipv4_decimal(SOURCE_IP_ADDR), Conversions::convert_string_tp_ipv4_decimal(DESTINATION_IP_ADDR));
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

            remove ("packet_path.txt");

        }

};




#endif SIMULATION_DEF