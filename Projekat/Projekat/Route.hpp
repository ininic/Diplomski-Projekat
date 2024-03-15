#ifndef ROUTE_DEF
#define ROUTE_DEF

#include "Conversions.hpp"
using namespace std;

class Route
{
	public:
		int destination_ip;
		int subnet_mask;
		int interface_ip;
		int next_hop_ip;
		int distance;

		Route(int destination_ip, int subnet_mask,  int next_hop_ip, int interface_ip, int distance)
		{
			this->destination_ip = destination_ip;
			this->subnet_mask = subnet_mask;
			this->next_hop_ip = next_hop_ip;
			this->interface_ip = interface_ip;			
			this->distance = distance;
		}

		void print_info()
		{
			cout << "Destination: " << Conversions::convert_ipv4_decimal_to_string(this->destination_ip) 
				 << "   Subnet mask: " << Conversions::convert_ipv4_decimal_to_string(this->subnet_mask)
				 << "   Next hop: " << ((this->next_hop_ip != 0) ? Conversions::convert_ipv4_decimal_to_string(this->next_hop_ip) : "On-link")
				 << "   Interface: " << Conversions::convert_ipv4_decimal_to_string(this->interface_ip)
				 << "   Distance: " << this->distance << endl;
		}
};






#endif ROUTE_DEF