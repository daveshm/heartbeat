#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define CLOCK CLOCK_REALTIME

constexpr uint64_t BILLION {1000000000};

extern __uint16_t bg_loop_cnt = 0;
extern __uint16_t PreemptionFlag = 0;
extern __uint16_t FiltIdlePeriod;

void MonitorIdlePeriod();

__uint64_t to_ns(const timespec &ts) {
  return ts.tv_sec * BILLION + ts.tv_nsec;
}


int main(int argc, char *argv[]){

   while(1) /* endless loop - spin in the background */
   {
      MonitorIdlePeriod();
   }
}

void MonitorIdlePeriod() 
{
   static timespec RT_Clock, prevRT_Clock;
   unsigned long IdlePeriod;
   bool interrupted = true;
   bg_loop_cnt++;
   prevRT_Clock = RT_Clock;

   clock_gettime(CLOCK, &RT_Clock);

   IdlePeriod = to_ns(RT_Clock) - to_ns(prevRT_Clock);
   printf("%ul \n", IdlePeriod);
}