#ifndef COMUNICATION_DEF
#define COMUNICATION_DEF



#include <mutex>      
#include <map>
#include <winsock2.h>
#include <conio.h>

#include "Router.hpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib, "Ws2_32.lib")
#pragma warning( disable : 4996)
#define SERVER_PORT 15000
#define SERVER_SLEEP_TIME 50
#define ACCESS_BUFFER_SIZE 1024
#define IP_ADDRESS_LEN 16
#define OUTGOING_BUFFER_SIZE 1024
// for demonstration purposes we will hard code
// local host ip adderss
#define SERVER_IP_ADDERESS "127.0.0.1"
#define DEFAULT_SUBNET_MASK 0b11111111111111111111111100000000


 mutex mtx;

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
	
		static void send_recv(Router& router)
		{
            mtx.lock();
            map<int, bool> primljeno;
            for (int i = 0; i < router.interfaces.size(); i++)
            {
                if (router.interfaces[i].nbr_router_id != 0)
                {
                    primljeno[router.interfaces[i].nbr_router_id] = false;
                }
            }
       
            vector<string> messages;

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
            iResult = bind(serverSocket, (LPSOCKADDR)&serverAddress, sizeof(serverAddress));

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


            mtx.unlock();
            // Main server loop
            bool once = true;

            int brojac_prijema = 7;
            while (1)
            {

                Sleep(SERVER_SLEEP_TIME);
                if (once)
                {
                    send_to_all_nbr(router);
                    once = false;
                }
                
                //mtx.lock();
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

                printf("Client connected from ip: %s, port: %d, sent: %s.\n", ipAddress, clientPort, accessBuffer);            
                // possible server-shutdown logic could be put here

                messages.push_back(accessBuffer);
                int id = atoi(accessBuffer);
                //cout <<"********%%%%%%%%%***********"<< accessBuffer << endl;
                primljeno[id] = true;
                int brojac = primljeno.size() ;        
                for (int i = 0; i < router.interfaces.size(); i++)
                {
                    if (router.interfaces[i].nbr_router_id != 0 && primljeno[router.interfaces[i].nbr_router_id] == true  )
                    {
                        brojac--;
                    }
                }
               
                //cout << "Ruter " << router.router_id << " primio poruku od " << id << endl;
                
                if (brojac == 0)
                {
                    //mtx.lock();
                    //Kada je primio poruke od svih suseda, ruter zapocinje osvezavanje svoje tabele rutiranja
                    cout << "Poruke koje je dobio ruter: " << router.router_id << endl;
                    for (string s : messages)
                    {
                        cout << s << endl;
                        update_table(router, s);
                    }
                   
                    //mtx.unlock();
                    //staviti u funkciju resetovanje mape prijema
                    for (int i = 0; i < router.interfaces.size(); i++)
                    {
                        if (router.interfaces[i].nbr_router_id != 0)
                        {
                            primljeno[router.interfaces[i].nbr_router_id] = false;
                        }
                    }
                    brojac_prijema--;

                    //nakon sto je primio poruke od svih, ponovo ce da posalje svoju tabelu svim susedima
                    once = true;
                    if (brojac_prijema == 0)
                    {
                        break;
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

            printf("Server successfully shut down.\n");
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

            //printf("Enter message from server:\n");

            // Read string from user into outgoing buffer
            //gets_s(outgoingBuffer, OUTGOING_BUFFER_SIZE);
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

            //printf("Message sent to server, press any key to exit.\n");
            //_getch();

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

        
        static void send_to_all_nbr(Router& router)
        {
            string poruka;

            poruka = form_a_message(router);
            
            //wait a while for all router to start
            Sleep(SERVER_SLEEP_TIME);

            for (int i = 0; i < router.interfaces.size(); i++)
            {
                if (router.interfaces[i].type != 0)
                {
                    send(SERVER_PORT + router.interfaces[i].nbr_router_id - 100, poruka);
                    //cout << "Ruter " << router.router_id << " Poslao poruku na " << router.interfaces[i].nbr_router_id << endl;
                }
            }                 
        }

        static string form_a_message(Router& router)
        {
            string message("");

            for (int i = 0; i < router.routing_table.size(); i++)
            {
                message += to_string(router.router_id) + ":" + to_string(router.routing_table[i].destination_ip) + ":" + to_string(router.routing_table[i].distance);
                if (i != router.routing_table.size() - 1)
                {
                    message += "#";
                }
            }
            return message;
        }

        static void update_table(Router& router, string message)
        {
            vector<int> destionations_ip;
            vector<int> distances;
            vector<int> senders_id;

            vector<string> strings;
            customSplit(message, '#', strings);


            //fstream f(filename);
            regex r1("(\\d+):([-]?\\d+):(\\d+)");
            smatch m;

            for (string s : strings)
            {
               // cout << "$$$" << s << endl;
                if (regex_search(s, m, r1))
                {
                    //cout << "URADJENO2" << atoi(m[2].str().c_str()) << endl;
                    senders_id.push_back(atoi(m[1].str().c_str()));
                    destionations_ip.push_back(atoi(m[2].str().c_str()));
                    distances.push_back(atoi(m[3].str().c_str()));

                }
            }

            for (int i = 0; i < destionations_ip.size(); i++)
            {

                int next_hop_ip;
                int interface_ip;
                for (Interface intf : router.interfaces)
                {
                    if (intf.nbr_router_id == senders_id[i])
                    {
                        interface_ip = intf.own_ip_addr;
                        next_hop_ip = intf.nbr_ip_addr;
                    }
                }

                //if destination is not in routing table, add it
                if (!router.contains_in_table(destionations_ip[i]))
                {
                    //(int destination_ip, int subnet_mask, int next_hop_ip, int interface_ip, int distance)   
                    

                    Route r(destionations_ip[i], DEFAULT_SUBNET_MASK, next_hop_ip, interface_ip, distances[i] + 1);
                    router.routing_table.push_back(r);
                    //cout << "URADJENO" << endl;
                }
                else
                {
                    //Ako ruta vec postoji u tabeli rutiranja, treba proveriti da li je distanca
                    //od posiljaoca plus 1 manja od trenutne udaljenosti
                    for (int j = 0; j < router.routing_table.size(); j++)
                    {
                        if (router.routing_table[j].destination_ip == destionations_ip[i])
                        {
                            if (router.routing_table[j].distance > distances[i] + 1)
                            {
                                //Route r(destionations_ip[i], DEFAULT_SUBNET_MASK, next_hop_ip, interface_ip, distances[i] + 1);
                                //router.routing_table.push_back(r);
                                router.routing_table[j].distance = distances[i] + 1;
                                router.routing_table[j].interface_ip = interface_ip;
                                router.routing_table[j].next_hop_ip = next_hop_ip;
                            }

                            break;
                        }
                    }
                }
            }

        }

      



        static void customSplit(string str, char separator, vector<string>& strings) 
        {
            int startIndex = 0, endIndex = 0;
            for (int i = 0; i <= str.size(); i++) {

                // If we reached the end of the word or the end of the input.
                if (str[i] == separator || i == str.size()) {
                    endIndex = i;
                    string temp;
                    temp.append(str, startIndex, endIndex - startIndex);
                    strings.push_back(temp);
                    startIndex = endIndex + 1;
                }
            }
        }
};






#endif COMUNICATION_DEF