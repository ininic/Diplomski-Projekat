#ifndef ROUTER_DEF
#define ROUTER_DEF
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

#include "Interface.hpp"
#include "Device.hpp"
#include "Route.hpp"

#define DEFAULT_SUBNET_MASK 0xffffff00
#define DEFAULT_ZERO_SUBNET_MASK 0x00000000

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
			int next_hop_ip = 0;
			int lan_interface_ip = 0;
			bool default_route = false;
			bool local_route = false;
			for (int i = 0; i < this->interfaces.size(); i++)
			{
				if (this->interfaces[i].type == 0 && !local_route)
				{
					lan_interface_ip = this->interfaces[i].own_ip_addr;

					if (!this->devices.empty())
					{
						int destination_ip = devices[0].ip_addr & DEFAULT_SUBNET_MASK;
						if (!routing_table_contains(destination_ip))
						{
							Route route(destination_ip, DEFAULT_SUBNET_MASK, 0, lan_interface_ip, 0);
							this->routing_table.push_back(route);
							local_route = true;
						}
					}
				}

				if(this->interfaces[i].type == 1 && !default_route)
				{
					lan_interface_ip = this->interfaces[i].own_ip_addr;
					next_hop_ip = this->interfaces[i].nbr_ip_addr;
					Route route_p(0, DEFAULT_ZERO_SUBNET_MASK, next_hop_ip, lan_interface_ip, 0);
					this->routing_table.push_back(route_p);
					default_route = true;
				}

				if (local_route && default_route)
				{
					break;
				}
			}
		}

		bool routing_table_contains(int destination_ip)
		{
			for (Route r : routing_table)
			{
				if (r.destination_ip == destination_ip)
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

			
		void update_table(string message)
		{
			vector<int> destionations_ip;
			vector<int> distances;
			vector<int> senders_id;

			vector<string> strings;
			customSplit(message, '#', strings);


			regex r1("(\\d+):([-]?\\d+):(\\d+)");
			smatch m;

			for (string s : strings)
			{
				if (regex_search(s, m, r1))
				{
					senders_id.push_back(atoi(m[1].str().c_str()));
					destionations_ip.push_back(atoi(m[2].str().c_str()));
					distances.push_back(atoi(m[3].str().c_str()));
				}
			}

			for (int i = 0; i < destionations_ip.size(); i++)
			{

				int next_hop_ip;
				int interface_ip;
				int cost = 0;
				for (Interface intf : this->interfaces)
				{
					if (intf.nbr_router_id == senders_id[i])
					{
						interface_ip = intf.own_ip_addr;
						next_hop_ip = intf.nbr_ip_addr;
						cost = intf.link_cost;
						break;
					}
				}
			

				//if destination is not in routing table, add it
				if (!this->routing_table_contains(destionations_ip[i]))
				{	
					Route r(destionations_ip[i], DEFAULT_SUBNET_MASK, next_hop_ip, interface_ip, distances[i] + cost);
					this->routing_table.push_back(r);
				}
				else
				{
					/*If the route already exists in the routing table, 
					it is necessary to check whether the distance 
					from the sender plus 1 is less than the current distance*/
					for (int j = 0; j < this->routing_table.size(); j++)
					{
						if (this->routing_table[j].destination_ip == destionations_ip[i])
						{
							if (this->routing_table[j].distance > distances[i] + cost)
							{
								this->routing_table[j].distance = distances[i] + cost;
								this->routing_table[j].interface_ip = interface_ip;
								this->routing_table[j].next_hop_ip = next_hop_ip;
							}
							break;
						}
					}
				}
			}

		}

		static void customSplit(string str, char separator, vector<string>& strings)
		{
			int startIndex = 0, endIndex = 0;
			for (int i = 0; i <= str.size(); i++) {

				// If we reached the end of the word or the end of the input.
				if (str[i] == separator || i == str.size()) {
					endIndex = i;
					string temp;
					temp.append(str, startIndex, endIndex - startIndex);
					strings.push_back(temp);
					startIndex = endIndex + 1;
				}
			}
		}

		void update(vector<string>& messages)
		{
			for (string s : messages)
			{
				this->update_table(s);
			}
		}

		string form_update_message()
		{
			string message("");

			if (this->routing_table.empty())
			{
				//dodati nesto za slucaj da ruter nema nijedan uredjaj...
			}

			for (int i = 0; i < this->routing_table.size(); i++)
			{
				if (this->routing_table[i].destination_ip != 0)
				{
					message += to_string(this->router_id) + ":" + to_string(this->routing_table[i].destination_ip) + ":" + to_string(this->routing_table[i].distance);
					if (i != this->routing_table.size() - 1)
					{
						message += "#";
					}
				}
			}
			return message;
		}

		void export_routing_table(chrono::high_resolution_clock::duration d)
		{
			ofstream outputFile;
			outputFile.open(to_string(this->router_id) + ".txt", ios_base::app); // create a new output file or append an existing one

			if (outputFile.is_open()) { // check if the file was opened successfully
				outputFile <<chrono::duration_cast<chrono::milliseconds>(d).count() << endl;
				for (Route r : this->routing_table)
				{
					outputFile << setw(13) << left << "Destination: " << setw(15) << left << Conversions::convert_ipv4_decimal_to_string(r.destination_ip)
						<< setw(13) << left << "Subnet mask: " << setw(16) << left  << Conversions::convert_ipv4_decimal_to_string(r.subnet_mask)
						<< setw(10) << left << "Next hop: " << setw(15) << left<< ((r.next_hop_ip != 0) ? Conversions::convert_ipv4_decimal_to_string(r.next_hop_ip) : "On-link")
						<< setw(11) << left << "Interface: " << setw(15) << left << Conversions::convert_ipv4_decimal_to_string(r.interface_ip)
						<< setw(11) << left << " Metric: " << setw(5) << left << r.distance << endl;
				}
			}
			else 
			{
				//std::cerr << "Error opening file(router.txt)\n";
			}

			outputFile.close();
		}

		
		string form_ip_packet(int destionation_ip, int source_ip, string data)
		{
			return "999#" + to_string(destionation_ip) + "#" + to_string(source_ip) + "#" + data;
		}

		bool exists_in_devices(int device_ip)
		{
			if (this->devices.empty())
			{
				return false;
			}

			for (Device d : this->devices)
			{
				if (d.ip_addr == device_ip)
				{
					return true;
				}
			}
			return false;
		}

		
};


#endif ROUTER_DEF