#ifndef ROUTE_DEF
#define ROUTE_DEF

#include "Conversions.hpp"
using namespace std;

class Route
{
	public:
		int destination_ip;
		int next_hop_ip;
		int distance;

		Route(int destination_ip, int next_hop_ip, int distance)
		{
			this->destination_ip = destination_ip;
			this->next_hop_ip = next_hop_ip;
			this->distance = distance;
		}

		void print_info()
		{
			cout << "Destination: " << Conversions::convert_ipv4_decimal_to_string(this->destination_ip) << " Next hop: " << Conversions::convert_ipv4_decimal_to_string(this->next_hop_ip)
				 << " Distance: " << this->distance << endl;
		}
};






#endif ROUTE_DEF