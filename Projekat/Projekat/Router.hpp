#ifndef ROUTER_DEF
#define ROUTER_DEF
#include <iostream>
#include <vector>
#include <string>

#include "Interface.hpp"
#include "Device.hpp"
#include "Route.hpp"

#define DEFAULT_SUBNET_MASK 0b11111111111111111111111100000000

using namespace std;
class Router
{
	public:
		int router_id;
		vector<Interface> interfaces;
		vector<Device> devices;
		vector<Route> routing_table;

		Router()
		{
			this->router_id = 0;
		}

		Router(int router_id)
		{
			this->router_id = router_id;
		}

		//setting routes for devices in local area network
		void init_routes()
		{
			
			int lan_interface_ip = 0;
			for (int i = 0; i < this->interfaces.size(); i++)
			{
				if (this->interfaces[i].type == 0)
				{
					lan_interface_ip = this->interfaces[i].own_ip_addr;
					break;
				}
			}

			for (int i = 0; i < devices.size(); i++)
			{
				int destination_ip = devices[i].ip_addr & DEFAULT_SUBNET_MASK;
				if (!routing_table_contains(destination_ip))
				{
					Route route(destination_ip, DEFAULT_SUBNET_MASK, 0, lan_interface_ip, 0);
					this->routing_table.push_back(route);
				}
			}
		}

		bool routing_table_contains(int destination_ip)
		{
			for (int i = 0; i < this->routing_table.size(); i++)
			{
				if (this->routing_table[i].destination_ip == destination_ip)
				{
					return true;
				}
			}

			return false;
		}

		void print_info()
		{
			cout << "############################" << endl << endl;
			cout << "Router: " << this->router_id << endl<<endl;
			cout << "Interfaces:" << endl<<endl;
			for (size_t i = 0; i < this->interfaces.size(); i++)
			{
				this->interfaces[i].print_info();
			}
			cout << endl;
			cout << "Devices:" << endl << endl;
			for (size_t i = 0; i < this->devices.size(); i++)
			{
				this->devices[i].print_info();
			}

			cout << endl;
			cout << "Routing table:" << endl << endl;
			for (size_t i = 0; i < this->routing_table.size(); i++)
			{
				this->routing_table[i].print_info();
			}
			cout << endl;
		}



};


#endif ROUTER_DEF