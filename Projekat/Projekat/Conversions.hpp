#ifndef CONVERSIONS_DEF
#define CONVERSIONS_DEF

#include<string>

using namespace std;

class Conversions
{
	public:

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

		static int form_ipv4_addr(unsigned char octet1, unsigned char octet2, unsigned char octet3, unsigned char octet4)
		{
			int ipv4 = 0;

			((unsigned char*)&ipv4)[3] = octet1;
			((unsigned char*)&ipv4)[2] = octet2;
			((unsigned char*)&ipv4)[1] = octet3;
			((unsigned char*)&ipv4)[0] = octet4;

			return ipv4;
		}

};


#endif CONVERSIONS_DEF