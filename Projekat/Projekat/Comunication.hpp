#ifndef COMUNICATION_DEF
#define COMUNICATION_DEF

#include <mutex>          

#include "Router.hpp"


 mutex mtx;

 class Comunication
{
	public:
	
		static void send_recv(Router& router)
		{
			mtx.lock();
			router.print_info();
			mtx.unlock();
		}
};






#endif COMUNICATION_DEF