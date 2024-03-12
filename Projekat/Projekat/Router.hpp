#ifndef ROUTER_DEF
#define ROUTER_DEF
#include <iostream>
#include <vector>
#include <string>

#include "Interface.hpp"
#include "Device.hpp"

using namespace std;
class Router
{
	public:
		int router_id;
		vector<Interface> interfaces;
		vector<Device> devices;
		Router()
		{
			this->router_id = 0;
		}

		Router(int router_id)
		{
			this->router_id = router_id;
		}


		void print_info()
		{
			cout << "Router: " << this->router_id << endl;
			cout << "Interfaces:" << endl;
			for (size_t i = 0; i < this->interfaces.size(); i++)
			{
				this->interfaces[i].print_info();
			}
			cout << "Devices:" << endl;
			for (size_t i = 0; i < this->devices.size(); i++)
			{
				this->devices[i].print_info();
			}
		}


};


#endif ROUTER_DEF