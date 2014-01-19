#include <stdio.h> 
#include <stdlib.h>


int main() {

        int george;
        printf("&george %p\n", &george);			/* the address of george (&george) is on the stack*/	

        int *sam;								
        printf("&sam %p\n", &sam);				/* the address of sam (&sam) is on the stack*/
        
        void *p1, *p2;				
        
        p1 = malloc(1 * sizeof(int));
        printf("p1 %p\n", p1);					/* the address in p1 (p1) is on the heap */
        
        p2 = malloc;
        printf("p2 %p\n", p2);					/* the address in p2 (p2) is on the stack*/
        
        sam = ((int *)p1) + 4;
        printf("sam %p", sam);					/* the address in sam (sam) is on the heap */
         

        /* base your answers on this point in the program */
        
        
        
        

}