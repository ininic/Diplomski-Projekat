
#ifndef NETWORK_DEF
#define NETWORK_DEF


#include <regex>
#include <fstream>

#include "Router.hpp"
#include "Conversions.hpp"

#define MAX_BUFF 159
#define DELIMITER_1 "--------------------------"
#define DELIMITER_2 "##########################"
class Network
{
	public:
       
		vector<Router> routers;

        void print_routers_info()
		{
            cout << "Routers:" << endl << endl;
            for (size_t i = 0; i < this->routers.size(); i++)
            {
                this->routers[i].print_info();
            }
		}

        //import data from configuration file
        void load_routers(const char* filename)
        {
            string delimiter_1(DELIMITER_1);
            string delimiter_2(DELIMITER_2);

            fstream f(filename);
            regex r1("router_id=(\\d+)");
            regex r2("(\\d+).(\\d+).(\\d+).(\\d+)\\|(\\d+).(\\d+).(\\d+).(\\d+)\\|(\\d+)\\|(\\d+)\\|(\\d+)\\|(\\d+)");
            regex r3("(\\d+).(\\d+).(\\d+).(\\d+)");
            smatch m;
            char buff[MAX_BUFF] = { 0 };

            while (!f.eof()) {
                f.getline(buff, 159, '\n');
                string s(buff);

                int router_id;
                int interface_addr_1;
                int interface_addr_2;
                int interface_id;
                int nbr_router_id;
                int device_addr;
                int link_cost;
                short interface_type;
                unsigned char o1, o2, o3, o4; 
                string s1, s2, s3;
                Router router;

                if (regex_search(s, m, r1)) 
                {
                    router_id = atoi(m[1].str().c_str());
                    router.router_id = router_id;
                }

                do
                {
                    f.getline(buff, 159, '\n');
                    s2.assign(buff);
                    if (regex_search(s2, m, r2)) 
                    {
                        o1 = atoi(m[1].str().c_str());
                        o2 = atoi(m[2].str().c_str());
                        o3 = atoi(m[3].str().c_str());
                        o4 = atoi(m[4].str().c_str());
                        interface_addr_1 = Conversions::form_ipv4_addr(o1, o2, o3, o4);

                        o1 = atoi(m[5].str().c_str());
                        o2 = atoi(m[6].str().c_str());
                        o3 = atoi(m[7].str().c_str());
                        o4 = atoi(m[8].str().c_str());
                        interface_addr_2 = Conversions::form_ipv4_addr(o1, o2, o3, o4);

                        nbr_router_id = atoi(m[9].str().c_str());
                        interface_id = atoi(m[10].str().c_str());
                        interface_type = atoi(m[11].str().c_str());
                        link_cost = atoi(m[12].str().c_str());
                        Interface interface(interface_addr_1, interface_addr_2, interface_id, nbr_router_id, interface_type, link_cost);
                        router.interfaces.push_back(interface);
                    }                   
                } while (s2.compare(delimiter_1) != 0);
                
                do
                {
                    f.getline(buff, 159, '\n');
                    s3.assign(buff);
                    if (regex_search(s3, m, r3)) {
                        o1 = atoi(m[1].str().c_str());
                        o2 = atoi(m[2].str().c_str());
                        o3 = atoi(m[3].str().c_str());
                        o4 = atoi(m[4].str().c_str());
                        device_addr = Conversions::form_ipv4_addr(o1, o2, o3, o4);
                        Device device(device_addr);
                        router.devices.push_back(device);
                    }
                } while (s3.compare(delimiter_2) != 0);
                    
                router.init_routes();
                routers.push_back(router);
            }
            f.close();
        }
};









#endif NETWORK_DEF