#ifndef SIMULATION_DEF
#define SIMULATION_DEF

#include <thread> 
#include <filesystem>

#include "Network.hpp"
#include "Comunication.hpp"

#define SOURCE_IP_ADDR "106.1.1.2"
#define SOURCE_ROUTER_ID 106
#define DESTINATION_IP_ADDR "108.1.1.2"


using namespace std;

class Simulation
{
	public: 
      
		static void run_simulation(Network& network)
		{
            fstream f("src_dest.txt");
            char buff[MAX_BUFF] = { 0 };
            f.getline(buff, 159, '\n');
            string source_ip_str(buff);
            f.getline(buff, 159, '\n');
            string destination_ip_str(buff);
            f.close();
            regex r("(\\d+).(\\d+).(\\d+).(\\d+)");
            smatch m;
            int source_router_id = 106;
            if (regex_search(source_ip_str, m, r))
            {
                source_router_id = atoi(m[1].str().c_str());            
            }      

            vector<thread> threads;
            remove_files(network);

            for (int i = 0; i < network.routers.size(); i++)
            {
                if (network.routers[i].router_id == source_router_id)
                {
                    thread t(Comunication::send_recv, ref(network.routers[i]), Conversions::convert_string_tp_ipv4_decimal(source_ip_str), Conversions::convert_string_tp_ipv4_decimal(destination_ip_str));
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