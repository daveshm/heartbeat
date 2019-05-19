#include<sys/time.h>
#include<time.h>
#include<stdio.h>
typedef unsigned long long ticks;

static __inline__ ticks getticks(void)
{
     unsigned a, d;
     asm("cpuid");
     asm volatile("rdtsc" : "=a" (a), "=d" (d));

     return (((ticks)a) | (((ticks)d) << 32));
}
int main(){
     int r = 0;
     ticks tick,tick1,tickh;
     unsigned time =0;

     tick = getticks();
   
     sleep(10);
   
     tick1 = getticks();
     
     long currentprocessorMHZ = 3600000;
		
     time = (unsigned)((tick1-tick)/currentprocessorMHZ);
    printf("\ntime in MS%u\n",time);
return 0;
}
