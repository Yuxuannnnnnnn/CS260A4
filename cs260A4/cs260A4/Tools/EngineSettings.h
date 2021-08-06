#include <mutex>
#include <iostream>


#ifndef ENGINE_SETTINGS
#define ENGINE_SETTINGS



//define debug mode
#define DEBUG

//if debug mode is defined, ASSERT macro 
//& PRINTOUT() will print something, otherwise will not
#ifdef DEBUG


#define ASSERT(arg)    printf(arg)

static std::mutex StdCout_Mutex;

template<typename Arg, typename... Args>
static void PRINTOUT(Arg arg, Args... args)
{
	std::lock_guard<std::mutex> StdCout_lock{ StdCout_Mutex };

	std::cout << std::forward<Arg>(arg);
	((std::cout << std::forward<Args>(args)), ...);
	std::cout << std::endl;
}

#else


#define ASSERT(arg)

template<typename Arg, typename... Args>
static void PRINTOUT(Arg arg, Args... args) 
{

}


#endif





#endif
