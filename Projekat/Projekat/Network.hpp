
#ifndef NETWORK_DEF
#define NETWORK_DEF

#include "Router.hpp"
#define IPV4_ADR_MAX 15 + 1

class Network
{
	public:
		vector<Router> routers;


		int convert_ipv4_string_to_int(char* ipv4_str)
		{
			int decimal_value_ipv4 = 0;

			unsigned char octet_decimal_value;

			char ipv4_str_tmp[IPV4_ADR_MAX];
			strcpy(ipv4_str_tmp, ipv4_str);

			char* octet = strtok(ipv4_str_tmp, ".");

			short i = 0;
			while (octet != NULL) {
				octet_decimal_value = atoi(octet);
				((unsigned char*)(&decimal_value_ipv4))[i++] = octet_decimal_value;
				octet = strtok(NULL, ".");
			}

			return decimal_value_ipv4;
		}



		


		void print_routers()
		{
			for (int i = 0; i < routers.size(); i++)
			{
				routers[i].print_info();
			}
		}

        FILE* safe_fopen(const char* filename, const char* mode, int error_code) {
            FILE* fp = fopen(filename, mode);
            if (fp == NULL) {
                printf("File '%s' can not be oppened!\n", filename);
                exit(error_code);
            }
            return fp;
        }


        void load_routers(const char* filename)
        {
            FILE* fp = safe_fopen(filename, "r", 111);

            char new_line[50];
            char end_line[50];
            char ip_adress_str[IPV4_ADR_MAX + 1];
            char nbr_ip_adress_str[IPV4_ADR_MAX + 1];
            while (feof(fp) == 0)
            {

                fgets(ip_adress_str, IPV4_ADR_MAX + 1, fp);
                ip_adress_str[strlen(ip_adress_str) - 1] = '\0';

               // ROUTER_LIST_EL* new_router = create_new_router_el(convert_ipv4_string_to_int(ip_adress_str));
                Router newRouter(convert_ipv4_string_to_int(ip_adress_str));
				this->routers.push_back(newRouter);

                fgets(new_line, 50, fp);
                fgets(nbr_ip_adress_str, IPV4_ADR_MAX + 1, fp);

                while (strcmp(nbr_ip_adress_str, "###\n") != 0)
                {
                    fgets(nbr_ip_adress_str, IPV4_ADR_MAX + 1, fp);
                    //add to neighbor list
                }

                fgets(end_line, 50, fp);
                fgets(new_line, 50, fp);
            }

        }
};









#endif NETWORK_DEF