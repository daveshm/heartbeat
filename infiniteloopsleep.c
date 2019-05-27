#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>




int main(int argc, char *argv[]){ 
	printf("cpu core: %d \n",sched_getcpu());
	
	int i, count;
	
	
	while(1){
		for(i = 0; i < 100000; i++){
			count++;
		}
		
		//usleep(0);
		
	}
}
	
	
		
			
