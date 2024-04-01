#ifndef ROUTE_DEF
#define ROUTE_DEF

#include <iomanip>  

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

		Route(int destination_ip, int subnet_mask, int next_hop_ip, int interface_ip, int distance)
		{
			this->destination_ip = destination_ip;
			this->subnet_mask = subnet_mask;
			this->next_hop_ip = next_hop_ip;
			this->interface_ip = interface_ip;			
			this->distance = distance;
		}

		void print_info()
		{
			cout << setw(13)<< left<< "Destination: "  << setw(13) << left << Conversions::convert_ipv4_decimal_to_string(this->destination_ip)
				 << setw(13) << left<< "Subnet mask: " << setw(15) << left<< Conversions::convert_ipv4_decimal_to_string(this->subnet_mask)
				 << setw(11) << left << "Next hop: " << setw(13) << left << ((this->next_hop_ip != 0) ? Conversions::convert_ipv4_decimal_to_string(this->next_hop_ip) : "On-link")
				 << setw(11) << left << "Interface: " << setw(13) << left << Conversions::convert_ipv4_decimal_to_string(this->interface_ip)
				 << setw(11) << left <<" Distance: "  << setw(3) << left <<  this->distance << endl;
		}
};






#endif ROUTE_DEF