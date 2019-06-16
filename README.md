# heartbeat
Measuring system health through heartbeats

Utilizing system clocks (http://man7.org/linux/man-pages/man2/clock_getres.2.html), as well as processor clocks (rdtsc and rdtscp) we are currently working on finding the impact of varied CPU load and process priority on the time in elapsed ns/ticks in covering a usleep(0) call. 

A usleep(0) call symbolizes the time it takes a system at certain stress levels to execute an empty line. This will eventually replaced with real examples of concensus algorithms, or systems such as ptp, where system load may affect the performance and accurateness of such services. 


To set the simulated CPU load we are using a tool called stress-ng, which is available here: https://wiki.ubuntu.com/Kernel/Reference/stress-ng. In tools/stress-ng there is a file called stress-cpu.c, of which we are utilizing the "loop" stress method as well as the load limit function written in stress-ng. The algorithm used to limit load is described in this file. 

changePriorities.cc records 10 trials of elapsed ticks/ns when called, and outputs findings into csv files for easy manipulation. This file also has an arguement to set the priority of itself, to allow for measure of impact on higher/lower priorities.

cpustress.py is a driver that sets the cpu load in increments on a specific core, and then calls viewRates.cc each time to record elapsed ticks/ns on the same core. This file creates a full spread of loads and priorities, and the time it takes each to complete usleep(0).







