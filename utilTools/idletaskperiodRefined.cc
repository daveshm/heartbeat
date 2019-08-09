#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define CLOCK CLOCK_REALTIME
#define RT_CLOCKS_PER_TASK ( 10000 / 5 )


unsigned int CPU_util_pct, FiltCPU_Pct; /* 0 = 0% , 255 = 100% */

int p,q,c;

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

   bg_loop_cnt++;
   prevRT_Clock = RT_Clock;

   clock_gettime(CLOCK, &RT_Clock);


   IdlePeriod = to_ns(RT_Clock) - to_ns(prevRT_Clock);
   printf("%ul \n", IdlePeriod);
}

void INT_10ms_tasks( void )
{
   static unsigned int prev_bg_loop_cnt = 0;
   static unsigned int delta_cnt;
   unsigned int idle_pct;
   unsigned int idle_time;

   delta_cnt = bg_loop_cnt - prev_bg_loop_cnt;
   prev_bg_loop_cnt = bg_loop_cnt;

   idle_time = delta_cnt * FiltIdlePeriod;
   if ( idle_time > RT_CLOCKS_PER_TASK )
      idle_time = RT_CLOCKS_PER_TASK;
   idle_pct = (int)( (255 * idle_time) / RT_CLOCKS_PER_TASK );
   CPU_util_pct = 255 - idle_pct;

  for(q = 0; q < 10700; q++){
		q++;
		for(p = 0; p < 1000; p++){
			c++;
		}
	}


   return;
}
