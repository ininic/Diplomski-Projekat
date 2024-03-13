#ifndef INTERFACE_DEF
#define INTERFACE_DEF

#include <iostream>

#include "Conversions.hpp"

using namespace std;

class Interface
{
	public:
		int interface_id;
		int own_ip_addr;
		int nbr_ip_addr;
		int nbr_router_id;
		short type;

		Interface(int own_ip_addr, int nbr_ip_addr, int interface_id, int nbr_router_id, short type)
		{
			this->own_ip_addr = own_ip_addr;
			this->nbr_ip_addr = nbr_ip_addr;
			this->interface_id = interface_id;
			this->nbr_router_id = nbr_router_id;
			this->type = type;
		}

		void print_info()
		{
			cout << "Interface: " << this->interface_id << endl;
			cout << "Own ip addres: " << Conversions::convert_ipv4_decimal_to_string(this->own_ip_addr) << endl;
			cout << "Neighbor's ip address: " << Conversions::convert_ipv4_decimal_to_string(this->nbr_ip_addr) << endl;
			cout << "Neighbor's router id:: " << this->nbr_router_id << endl;
			cout << "Type: " << this->type << endl << endl;
		}
	
};




#endif INTERFACE_DEF