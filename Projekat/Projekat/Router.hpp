#ifndef ROUTER_DEF
#define ROUTER_DEF
#include <iostream>
#include <vector>
#define IPV4_ADR_MAX 15 + 1

using namespace std;
class Router
{
	public:
		int ip_adress;
		vector<int> neighboring_routers;

		Router(int ip_adress)
		{
			this->ip_adress = ip_adress;
		}

		void print_info()
		{
			cout << "------------------------" << endl;
			cout << "         Router:" << endl;
			shared_ptr<string> str_tmp = convert_ipv4_decimal_to_string(this->ip_adress);
			cout <<"ip adress: "<< *str_tmp << endl;
			cout << "neighboring routers: ";
			for (int i = 0; i < neighboring_routers.size(); i++)
			{
				cout << *convert_ipv4_decimal_to_string(neighboring_routers[i]) <<"  ";
			}
			cout << endl;
		}

		shared_ptr<string> convert_ipv4_decimal_to_string(int ipv4_dec)
		{
			char* ipv4_str_tmp = (char*)malloc(IPV4_ADR_MAX * sizeof(char));
			unsigned char octet1, octet2, octet3, octet4;
			unsigned char o1_size, o2_size, o3_size, o4_size;

			octet1 = ((unsigned char*)(&ipv4_dec))[0];
			o1_size = size_of_octet(octet1);
			sprintf(ipv4_str_tmp, "%d", octet1);
			ipv4_str_tmp[o1_size] = '.';

			octet2 = ((unsigned char*)(&ipv4_dec))[1];
			o2_size = size_of_octet(octet2);
			sprintf(ipv4_str_tmp + o1_size + 1, "%d", octet2);
			ipv4_str_tmp[o1_size + o2_size + 1] = '.';


			octet3 = ((unsigned char*)(&ipv4_dec))[2];
			o3_size = size_of_octet(octet3);
			sprintf(ipv4_str_tmp + o1_size + o2_size + 2, "%d", octet3);
			ipv4_str_tmp[o1_size + o2_size + o3_size + 2] = '.';

			octet4 = ((unsigned char*)(&ipv4_dec))[3];
			o4_size = size_of_octet(octet4);
			sprintf(ipv4_str_tmp + o1_size + o2_size + o3_size + 3, "%d", octet4);

			ipv4_str_tmp[o1_size + o2_size + o3_size + o4_size + 3] = '\0';

			shared_ptr<string> str_ptr = make_shared<string>(ipv4_str_tmp);

			free(ipv4_str_tmp); //funkcija odgovorna za oslobadjanje svoje memorije
								//ono sto vraca vraca kao smart pointer, osigurano od
								//curenja memorije
			return str_ptr;
		};

	private:
		int size_of_octet(unsigned char octet)
		{
			if (octet < 10)
			{
				return 1;
			}
			else if (octet < 100)
			{
				return 2;
			}
			else
			{
				return 3;
			}
		}
};








#endif ROUTER_DEF