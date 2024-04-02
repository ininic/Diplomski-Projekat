#ifndef DEVICE_DEF
#define DEVICE_DEF

#include "Conversions.hpp"

using namespace std;

class Device
{
	public:
		int ip_addr;

		Device(int ip_addr)
		{
			this->ip_addr = ip_addr;
		}

		void print_info()
		{
			cout << "Ip address: " << Conversions::convert_ipv4_decimal_to_string(this->ip_addr) << endl;
		}
};


#endif DEVICE_DEF