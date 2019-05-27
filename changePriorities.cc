#include <time.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <cstdlib>
#include <sys/resource.h>
#include "rdtscp.h"
using std::cout;  using std::cerr;  using std::endl;

#define CLOCK CLOCK_REALTIME

constexpr uint64_t BILLION {1000000000};
uint64_t to_ns(const timespec &ts) {
  return ts.tv_sec * BILLION + ts.tv_nsec;
}
void view_ticks_per_ns(int runs =10, int sleep =10);

int main(int argc, char **argv) {
  int runs = 10, sleep = 0;
  if (argc != 1 && argc != 3) {
    cerr << "Usage: " << argv[0] << " [ RUNS SLEEP ] \n";
    exit(1);
  } else if (argc == 3) {
    runs = std::atoi(argv[1]);
    sleep = std::atoi(argv[2]);
  }
  
    printf("cpu core: %d \n",sched_getcpu());
	
	int startingpriority;

	startingpriority =  getpriority(PRIO_PROCESS, getpid()); //get the nice of the current process
	 
	printf("the pid of this process is: %d \n", getpid());
	printf("the starting priority of this process is: %d \n", startingpriority);
	 
    printf("we will now iterate through priority levels!! \n");
    
    int i;
    
    for(i = 0; i < 20; i++){
		setpriority(PRIO_PROCESS, getpid(), i);
		printf("priority is now: %d \n", getpriority(PRIO_PROCESS, getpid()));
		
		printf("running sleep tests! \n");
		view_ticks_per_ns(runs, sleep); 
	}
}

  void view_ticks_per_ns(int RUNS, int SLEEP) {
// Prints out stream of RUNS tsc ticks per ns, each calculated over a SLEEP secs interval.
  timespec clock_start, clock_end;
  unsigned long elapsed_ns;
  double rate; // ticks per ns from each run.

  clock_getres(CLOCK, &clock_start);
  cout <<  "Clock resolution: " << to_ns(clock_start) << "ns\n\n";

  cout << "ns      " << endl;
  for (int i = 0; i < RUNS; ++i) {

    clock_gettime(CLOCK, &clock_start);


    usleep(SLEEP);


    clock_gettime(CLOCK, &clock_end);
                   
    elapsed_ns = to_ns(clock_end) - to_ns(clock_start);

    cout << elapsed_ns << " " << std::setprecision(12) << endl;
  } 
}
