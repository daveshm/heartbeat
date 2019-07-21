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
   //SetupInterrupts();
   //InitializeModules();
   //EnableInterrupts();

   while(1) /* endless loop - spin in the background */
   {
      MonitorIdlePeriod();
      //CheckCRC();
      //MonitorStack();
      //... do other non-time critical logic here.
   }
}

void MonitorIdlePeriod() 
{
   static timespec RT_Clock, prevRT_Clock;
   unsigned long IdlePeriod;
   bool interrupted = true;
   bg_loop_cnt++;
   prevRT_Clock = RT_Clock;

   //DisableInterrupts(); /* start atomic section */
   clock_gettime(CLOCK, &RT_Clock);
   //if ( PreemptionFlag == 0 )
      //interrupted = FALSE;
   //PreemptionFlag = 0;
   //Enable Interrupts(); /* end atomic section */

   IdlePeriod = to_ns(RT_Clock) - to_ns(prevRT_Clock);
   printf("%ul \n", IdlePeriod);
   //if ( !interrupted )
     //FiltIdlePeriod = Filter( FiltIdlePeriod, IdlePeriod );
}