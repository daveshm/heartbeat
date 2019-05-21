#include<sys/time.h>
#include<time.h>
#include<stdio.h>
typedef unsigned long long ticks;


static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx)
{
        /* ecx is often an input as well as an output. */
        asm volatile("cpuid"
            : "=a" (*eax),
              "=b" (*ebx),
              "=c" (*ecx),
              "=d" (*edx)
            : "0" (*eax), "2" (*ecx));
}

static __inline__ ticks getticks(void)
{
     unsigned a, d, id;
     asm volatile("rdtsc" : "=a" (a), "=d" (d));

     return (((ticks)a) | (((ticks)d) << 32));
}

int main(){
	
	 unsigned eax, ebx, ecx, edx;

     eax = 1; /* processor info and feature bits */
     native_cpuid(&eax, &ebx, &ecx, &edx);

     printf("stepping %d\n", eax & 0xF);
     printf("model %d\n", (eax >> 4) & 0xF);
     printf("family %d\n", (eax >> 8) & 0xF);
     printf("processor type %d\n", (eax >> 12) & 0x3);
     printf("extended model %d\n", (eax >> 16) & 0xF);
     printf("extended family %d\n", (eax >> 20) & 0xFF);

  
     eax = 3; /* processor serial number */
     native_cpuid(&eax, &ebx, &ecx, &edx);
	
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
