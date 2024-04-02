#ifndef CONVERSIONS_DEF
#define CONVERSIONS_DEF

#include<string>
#include <regex>
#include <fstream>

using namespace std;

class Conversions
{
	public:

		//Method for converting decimal ip_v4 addres to string respresentation
		static string convert_ipv4_decimal_to_string(int ipv4_dec)
		{
			string ipv4_str;
			unsigned char octet1, octet2, octet3, octet4;

			octet1 = ((unsigned char*)(&ipv4_dec))[3];
			octet2 = ((unsigned char*)(&ipv4_dec))[2];
			octet3 = ((unsigned char*)(&ipv4_dec))[1];
			octet4 = ((unsigned char*)(&ipv4_dec))[0];

			ipv4_str = to_string(octet1) + "." + to_string(octet2) + "." + to_string(octet3) + "." + to_string(octet4);

			return ipv4_str;
		}

		//Method that forms the ip addres from 4 octets
		static int form_ipv4_addr(unsigned char octet1, unsigned char octet2, unsigned char octet3, unsigned char octet4)
		{
			int ipv4 = 0;

			((unsigned char*)&ipv4)[3] = octet1;
			((unsigned char*)&ipv4)[2] = octet2;
			((unsigned char*)&ipv4)[1] = octet3;
			((unsigned char*)&ipv4)[0] = octet4;

			return ipv4;
		}

		static int convert_string_tp_ipv4_decimal(string ipv4_str)
		{
			regex r("(\\d+).(\\d+).(\\d+).(\\d+)");
			smatch m;
			unsigned char o1, o2, o3, o4;
			int ipv4_decimal = 0;
			if (regex_search(ipv4_str, m, r))
			{
				o1 = atoi(m[1].str().c_str());
				o2 = atoi(m[2].str().c_str());
				o3 = atoi(m[3].str().c_str());
				o4 = atoi(m[4].str().c_str());
				ipv4_decimal = Conversions::form_ipv4_addr(o1, o2, o3, o4);
			}

			return ipv4_decimal;
		}



};


#endif CONVERSIONS_DEF