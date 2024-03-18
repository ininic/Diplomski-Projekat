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

		bool contains_in_table(int destination_ip) const
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
				// cout << "$$$" << s << endl;
				if (regex_search(s, m, r1))
				{
					//cout << "URADJENO2" << atoi(m[2].str().c_str()) << endl;
					senders_id.push_back(atoi(m[1].str().c_str()));
					destionations_ip.push_back(atoi(m[2].str().c_str()));
					distances.push_back(atoi(m[3].str().c_str()));

				}
			}

			for (int i = 0; i < destionations_ip.size(); i++)
			{

				int next_hop_ip;
				int interface_ip;
				for (Interface intf : this->interfaces)
				{
					if (intf.nbr_router_id == senders_id[i])
					{
						interface_ip = intf.own_ip_addr;
						next_hop_ip = intf.nbr_ip_addr;
					}
				}

				//if destination is not in routing table, add it
				if (!this->contains_in_table(destionations_ip[i]))
				{
					Route r(destionations_ip[i], DEFAULT_SUBNET_MASK, next_hop_ip, interface_ip, distances[i] + 1);
					this->routing_table.push_back(r);
				}
				else
				{
					//Ako ruta vec postoji u tabeli rutiranja, treba proveriti da li je distanca
					//od posiljaoca plus 1 manja od trenutne udaljenosti
					for (int j = 0; j < this->routing_table.size(); j++)
					{
						if (this->routing_table[j].destination_ip == destionations_ip[i])
						{
							if (this->routing_table[j].distance > distances[i] + 1)
							{
								this->routing_table[j].distance = distances[i] + 1;
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
			//cout << "Poruke koje je dobio ruter: " << router.router_id << endl;
			for (string s : messages)
			{
				//cout << s << endl;
				//update_table(router, s);
				this->update_table(s);
			}
		}

		string form_update_message()
		{
			string message("");

			for (int i = 0; i < this->routing_table.size(); i++)
			{
				message += to_string(this->router_id) + ":" + to_string(this->routing_table[i].destination_ip) + ":" + to_string(this->routing_table[i].distance);
				if (i != this->routing_table.size() - 1)
				{
					message += "#";
				}
			}
			return message;
		}
};


#endif ROUTER_DEF