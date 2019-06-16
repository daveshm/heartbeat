#include <time.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sys/resource.h>
#include "rdtscp.h"
using std::cout;  using std::cerr;  using std::endl;
using namespace std; 

#define CLOCK CLOCK_REALTIME

long long int averageNS;
long long int averageTicks;


ofstream myfile; 
//ofstream loadfile; 



constexpr uint64_t BILLION {1000000000};
uint64_t to_ns(const timespec &ts) {
  return ts.tv_sec * BILLION + ts.tv_nsec;
}

void view_ticks_per_ns(int runs =10, int sleep =10);

int main(int argc, char **argv) {
  int runs = 10, sleep = 0;
  
  string load = argv[1];
  string priorityString = argv[2];
  int priority = atoi(argv[2]);
  
  myfile.open("data/loadsxtimeatpriority" +priorityString +".csv", ios::app); 
  
    //myfile.open("data/atload" + load + ".csv"); 
    
    //loadfile.open("data/loadsxtime.csv", ios::app);
    
    
    cout << "THE LOAD IS CURRENTLY AT: " << load << std::endl;
    
    //myfile << "Load" << "," << "Avg. NS" << "," << "Avg. Ticks" << std::endl;
    
    //loadfile << "Load" << "," << "Avg. NS" << "," << "Avg. Ticks" << std::endl;
  

    printf("cpu core: %d \n",sched_getcpu());
	
	
    
    
    //for(i = 0; i < 20; i++){
		setpriority(PRIO_PROCESS, getpid(), priority);
		usleep(1000000);
		//printf("priority is now: %d \n", getpriority(PRIO_PROCESS, getpid()));
		myfile << load << ",";
		//printf("running sleep tests! \n");
		view_ticks_per_ns(runs, sleep); 
	//}
	
	int startingpriority;

	startingpriority =  getpriority(PRIO_PROCESS, getpid()); //get the nice of the current process
	 
	printf("the pid of this process is: %d \n", getpid());
	printf("the starting priority of this process is: %d \n", startingpriority);
	 
    printf("we will now iterate through priority levels!! \n");
    
}

 void view_ticks_per_ns(int RUNS, int SLEEP) {
// Prints out stream of RUNS tsc ticks per ns, each calculated over a SLEEP secs interval.
  timespec clock_start, clock_end;
  unsigned long tsc1, tsc2, tsc_start, tsc_end;
  unsigned long elapsed_ns, elapsed_ticks;
  double rate; // ticks per ns from each run.
  averageNS = 0;
  averageTicks = 0;
  clock_getres(CLOCK, &clock_start);
  cout <<  "Clock resolution: " << to_ns(clock_start) << "ns\n\n";

  cout << " tsc ticks      " << "ns      " << " tsc ticks per ns\n";
  for (int i = 0; i < RUNS; ++i) {
    tsc1 = rdtscp_start();
    clock_gettime(CLOCK, &clock_start);
    tsc2 = rdtscp_end();                      
    tsc_start = (tsc1 + tsc2) / 2;


    usleep(SLEEP);

    tsc1 = rdtscp_start();
    clock_gettime(CLOCK, &clock_end);
    tsc2 = rdtscp_end();                     
    tsc_end = (tsc1 + tsc2) / 2;

	

    elapsed_ticks = tsc_end - tsc_start;
    elapsed_ns = to_ns(clock_end) - to_ns(clock_start);
    
    
    averageNS = averageNS + elapsed_ns;
    averageTicks = averageTicks + elapsed_ticks;
    
    rate = static_cast<double>(elapsed_ticks) / elapsed_ns;

    cout << elapsed_ticks << " " << elapsed_ns << " " << std::setprecision(12) << rate << endl;
    
    
  } 
  printf("average NS @ priority: %llu \n", averageNS/RUNS);
  printf("average ticks @ priority: %llu \n\n", averageTicks/RUNS);
  
  myfile << averageNS/RUNS << "," << averageTicks/RUNS << endl;
  
}
