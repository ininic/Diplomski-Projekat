#ifndef COMUNICATION_DEF
#define COMUNICATION_DEF

#include <mutex>      
#include <map>
#include <winsock2.h>
#include <conio.h>
#include <random>
#include <functional>

#include "Router.hpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib, "Ws2_32.lib")
#pragma warning( disable : 4996)
#define SERVER_PORT 15000
#define SERVER_SLEEP_TIME 50
#define ACCESS_BUFFER_SIZE 2048
#define IP_ADDRESS_LEN 16
#define OUTGOING_BUFFER_SIZE 2048

#define SERVER_IP_ADDERESS "127.0.0.1"
#define ITERATIONS 10
#define MIN_ITERATIONS 5
#define MODE_INDICATOR 999
 

 mutex mtx;
 typedef chrono::high_resolution_clock hrc_t;

 class Comunication
{
	public:
        static bool InitializeWindowsSockets()
        {
            WSADATA wsaData;
            // Initialize windows sockets library for this process
            int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (iResult != 0)
            {
                printf("WSAStartup failed with error: %d\n", iResult);
                return false;
            }
            return true;
        }
	
        //if source_ip and destination_ip have zero values, it is a "updating routing table" mode
        //otherwise, it is a "sending data" mode 
		static void send_recv(Router& router, int source_ip, int destination_ip)
		{ 
            mtx.lock();
            int mode = source_ip + destination_ip;
            hrc_t::time_point tp = hrc_t::now();   

            map<int, bool> received;
            vector<string> messages;
            reset_received(received, router);
            default_random_engine generator;

            hrc_t::duration d = hrc_t::now() - tp;
            generator.seed(d.count());
            uniform_real_distribution<double> dist(0, 50);
            auto rd = std::bind(dist, generator);

            // Server address
            sockaddr_in serverAddress;
            // Server's socket
            int serverPort = SERVER_PORT + router.router_id - 100;
            // size of sockaddr structure
            int sockAddrLen = sizeof(struct sockaddr);
            // buffer we will use to receive client message
            char accessBuffer[ACCESS_BUFFER_SIZE];
            // variable used to store function return value
            int iResult;

            if (InitializeWindowsSockets() == false)
            {
                // we won't log anything since it will be logged
                // by InitializeWindowsSockets() function
                return;
            }

            // Initialize serverAddress structure used by bind
            memset((char*)&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET; /*set server address protocol family*/
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            serverAddress.sin_port = htons(serverPort);

            // create a socket
            SOCKET serverSocket = socket(AF_INET,      // IPv4 address famly
                SOCK_DGRAM,   // datagram supporting socket
                IPPROTO_UDP); // UDP

            // check if socket creation succeeded
            if (serverSocket == INVALID_SOCKET)
            {
                printf("Creating socket failed with error: %d\n", WSAGetLastError());
                WSACleanup();
                return;
            }

            // Bind port number and local address to socket
            iResult = ::bind(serverSocket, (LPSOCKADDR)&serverAddress, sizeof(serverAddress));

            if (iResult == SOCKET_ERROR)
            {
                printf("Socket bind failed with error: %d\n", WSAGetLastError());
                closesocket(serverSocket);
                WSACleanup();
                return;
            }

            // Set socket to nonblocking mode
            unsigned long int nonBlockingMode = 1;
            iResult = ioctlsocket(serverSocket, FIONBIO, &nonBlockingMode);

            if (iResult == SOCKET_ERROR)
            {
                printf("ioctlsocket failed with error: %ld\n", WSAGetLastError());
                return;
            }

            printf("Simple UDP server started and waiting clients.\n");
        
            // Main server loop
            bool send_ready = true;
            bool once = true;
            int iteration = 0;

            chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
            mtx.unlock();
            Sleep(SERVER_SLEEP_TIME);
            while (1)
            {        
              
                /*If source_ip is a "router's device", and if there were enough iterations
                  router sends message to the destination
               */
                if(router.exists_in_devices(source_ip) && iteration > MIN_ITERATIONS && once)
                {
                    string log_message = "Router: " + to_string(router.router_id) + "\n";
                    log_message += "Source: " + Conversions::convert_ipv4_decimal_to_string(source_ip) + "\n";
                    string message = router.form_ip_packet(destination_ip, source_ip, "Aloha");

                    cout << "\n\nRouter: " << router.router_id << endl;
                    cout << "Source: " << Conversions::convert_ipv4_decimal_to_string(source_ip) << endl;
                    send_to_destinaion(destination_ip, message, router, log_message);
                    once = false;
                }


                if (send_ready)
                {
                    send_to_all_nbr(router);
                    send_ready = false;
                }
                
                // clientAddress will be populated from recvfrom
                sockaddr_in clientAddress;
                memset(&clientAddress, 0, sizeof(sockaddr_in));

                // set whole buffer to zero
                memset(accessBuffer, 0, ACCESS_BUFFER_SIZE);

                // Initialize select parameters
                FD_SET set;
                timeval timeVal;

                FD_ZERO(&set);
                // Add socket we will wait to read from
                FD_SET(serverSocket, &set);

                // Set timeouts to zero since we want select to return
                // instantaneously
                timeVal.tv_sec = 0;
                timeVal.tv_usec = 0;

                iResult = select(0 /* ignored */, &set, NULL, NULL, &timeVal);

                // lets check if there was an error during select
                if (iResult == SOCKET_ERROR)
                {
                    fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
                    continue;
                }

                // now, lets check if there are any sockets ready
                if (iResult == 0)
                {
                    // there are no ready sockets, sleep for a while and check again
                    Sleep(SERVER_SLEEP_TIME);
                    continue;
                }

                iResult = recvfrom(serverSocket,
                    accessBuffer,
                    ACCESS_BUFFER_SIZE,
                    0,
                    (LPSOCKADDR)&clientAddress,
                    &sockAddrLen);

                if (iResult == SOCKET_ERROR)
                {
                    printf("recvfrom failed with error: %d\n", WSAGetLastError());
                    continue;
                }

                char ipAddress[IP_ADDRESS_LEN];
                // copy client ip to local char[]
                strcpy_s(ipAddress, sizeof(ipAddress), inet_ntoa(clientAddress.sin_addr));
                // convert port number from TCP/IP byte order to
                // little endian byte order
                int clientPort = ntohs((u_short)clientAddress.sin_port);

                int id = atoi(accessBuffer);
                if (id == MODE_INDICATOR)
                {
                    //Router parse and forward message with MODE_INDICATOR header
                    //If destination in message is one of devices in this router's LAN
                    //message will be printed
                    parse_message(accessBuffer, router);
                }
                else
                {
                    messages.push_back(accessBuffer);

                    set_received(id, received);

                    int recv_counter = received.size();

                    for (int i = 0; i < router.interfaces.size(); i++)
                    {
                        if (router.interfaces[i].nbr_router_id != 0)
                        {
                            if (received[router.interfaces[i].nbr_router_id] == true)
                            {
                                recv_counter--;
                            }
                        }
                    }

                    if (recv_counter == 0)
                    {
                        //If router has received message from all its neighbours, it starts 
                        //updating its routing table
                        router.update(messages);
                        router.export_routing_table(chrono::high_resolution_clock::now() - start);
                        reset_received(received, router);
                        iteration++;

                        //after receiving messages from everyone, it will resend its table to all neighbors
                        send_ready = true;

                        if (iteration == ITERATIONS)
                        {
                            break;
                        }
                    }
                }            
            }

            // if we are here, it means that server is shutting down
            // close socket and unintialize WinSock2 library
            iResult = closesocket(serverSocket);
            if (iResult == SOCKET_ERROR)
            {
                printf("closesocket failed with error: %d\n", WSAGetLastError());
                return;
            }

            iResult = WSACleanup();
            if (iResult == SOCKET_ERROR)
            {
                printf("WSACleanup failed with error: %d\n", WSAGetLastError());
                return;
            }

            printf("Server %d successfully shut down.\n",router.router_id);
            return ;
        		
		}

        static int send(int port, string poruka)
        {
            // Server address
            sockaddr_in serverAddress;
            // size of sockaddr structure    
            int sockAddrLen = sizeof(struct sockaddr);
            // buffer we will use to store message
            char outgoingBuffer[OUTGOING_BUFFER_SIZE];
            // port used for communication with server
            int serverPort = port;
            // variable used to store function return value
            int iResult;

            // Initialize windows sockets for this process
            InitializeWindowsSockets();

            // Initialize serverAddress structure
            memset((char*)&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDERESS);
            serverAddress.sin_port = htons((u_short)serverPort);

            // create a socket
            SOCKET clientSocket = socket(AF_INET,      // IPv4 address famly
                SOCK_DGRAM,   // datagram socket
                IPPROTO_UDP); // UDP

            // check if socket creation succeeded
            if (clientSocket == INVALID_SOCKET)
            {
                printf("Creating socket failed with error: %d\n", WSAGetLastError());
                WSACleanup();
                return 1;
            }

            strcpy(outgoingBuffer, poruka.c_str());

            iResult = sendto(clientSocket,
                outgoingBuffer,
                strlen(outgoingBuffer),
                0,
                (LPSOCKADDR)&serverAddress,
                sockAddrLen);

            if (iResult == SOCKET_ERROR)
            {
                printf("sendto failed with error: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }

            iResult = closesocket(clientSocket);
            if (iResult == SOCKET_ERROR)
            {
                printf("closesocket failed with error: %d\n", WSAGetLastError());
                return 1;
            }

            iResult = WSACleanup();
            if (iResult == SOCKET_ERROR)
            {
                printf("WSACleanup failed with error: %ld\n", WSAGetLastError());
                return 1;
            }

            return 0;
        }

        static void parse_message(string message, Router& router)
        {
            regex r1("(\\d+)#([-]?\\d+)#([-]?\\d+)#(.)");
            smatch m;
            int destination_ip = 0;
            if (regex_search(message, m, r1))
            {
                destination_ip = atoi(m[2].str().c_str());
            }
            int lan_ip;

            for (Interface i : router.interfaces)
            {
                if (i.type == 0)
                {
                  
                    lan_ip = i.own_ip_addr;
                    cout << "Router: " << router.router_id << endl;
                    cout << "Lan: " << Conversions::convert_ipv4_decimal_to_string(lan_ip)<< endl;
                }
            }
            string log_message = "Router: " + to_string(router.router_id) + "\n";
            log_message += "Lan: " + Conversions::convert_ipv4_decimal_to_string(lan_ip)+ "\n";
            send_to_destinaion(destination_ip, message, router, log_message);

        }

        static void send_to_destinaion(int destination_ip, string message, Router& router, string log_message)
        {
            int next_hop = 0;
            bool find = false;
            for (Route r : router.routing_table)
            {
                if (r.destination_ip == (DEFAULT_SUBNET_MASK & destination_ip))
                {
                    next_hop = r.next_hop_ip;
                    find = true;
                    break;
                }
            }       

            //If router cant find destination in its routing table, packet is forwarding
            //to the predefined next destination which is stored in first row in routing table
  
            if (!find)
            {
                next_hop = router.routing_table[0].next_hop_ip;
            }

            log_message += "Destination: " + Conversions::convert_ipv4_decimal_to_string(destination_ip)+"\n";
            cout << "Destination: " << Conversions::convert_ipv4_decimal_to_string(destination_ip) << endl;
            if (next_hop != 0)
            {
                log_message += "Next hop : " + Conversions::convert_ipv4_decimal_to_string(next_hop) + "\n";
                cout << "Next hop : " << Conversions::convert_ipv4_decimal_to_string(next_hop) << endl;
            }
            else
            {
                log_message += "Next hop : On-Link\n";
                cout << "Next hop : On-Link" << endl;
            }

            //If packet destinatin is in local network, packet forwarding
            //through the network ends here

            if (next_hop == 0)
            {
                log_message += "Message received! \n";
                save_ip_packet_path(log_message);
                cout << "Message received: " << message << endl<<endl<<endl;
            }
            else
            {
                for (Interface i : router.interfaces)
                {
                    if (i.nbr_ip_addr == next_hop)
                    {
                        log_message += "Interface: " + Conversions::convert_ipv4_decimal_to_string(i.own_ip_addr) + "\n";
                        log_message += "----------------------\n";
                        cout << "Interface: "<< Conversions::convert_ipv4_decimal_to_string(i.own_ip_addr)<< endl;
                        cout<<"----------------------"<<endl;
                        send(SERVER_PORT + i.nbr_router_id - 100, message);
                        save_ip_packet_path(log_message);
                        break;
                    }
                }
            }
           
        }
        
        static void send_to_all_nbr(Router& router)
        {
            string poruka;
            poruka = router.form_update_message();
            
            hrc_t::time_point tp = hrc_t::now();     
            default_random_engine generator;
            hrc_t::duration d = hrc_t::now() - tp;
            generator.seed(d.count());
            uniform_real_distribution<double> dist(50, 50);
            auto rd = std::bind(dist, generator);

            //!!!
            //Router waits some time for all previously sent messages to be delivered
            //This can be more deterministic if every router also have timer 
            //or keeps record who sent message to whom

            Sleep(200 + rd());

            for (int i = 0; i < router.interfaces.size(); i++)
            {
                if (router.interfaces[i].type != 0)
                {
                    send(SERVER_PORT + router.interfaces[i].nbr_router_id - 100, poruka);
                }
            }                 
        }

        static void reset_received(map<int, bool>& received, Router& router)
        {
            for (int i = 0; i < router.interfaces.size(); i++)
            {
                if (router.interfaces[i].nbr_router_id != 0)
                {
                    received[router.interfaces[i].nbr_router_id] = false;
                }
            }
        }

        static void set_received(int id, map<int, bool>& received)
        {
            received[id] = true;
        }

        static void save_ip_packet_path(string message)
        {
            ofstream outputFile;
            outputFile.open("packet_path.txt", ios_base::app); // create a new output file or append an existing one

            if (outputFile.is_open())
            {
          
                outputFile << message;

            }
            else
            {
                std::cerr << "Error opening file\n";
            }
        }

};


#endif COMUNICATION_DEF