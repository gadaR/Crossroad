//
//  carrefour.c
//  
//
//
//

#include "carrefour.h"
#include <string.h>
#include <stdlib.h>


int main (int argc, char ** argv){
    int i;
    
    /*** interactif mode ***/
    if (argc == 1) {
        puts("interactif\n");
        //mode interactif
    }else{
        
    
    for (i=0; i<argc; i++) {

        /*** automatic mode ***/
        if (strcmp(argv[i],"-a")==0) {
            puts("auto\n");

            
        }
        if (strcmp(argv[i],"-t")==0) {
            printf("temps %d\n", atoi(argv[i+1]));
            
            //tempsMax=atoi(argv[i+1])
        }
    }
    }
    return 0;
}