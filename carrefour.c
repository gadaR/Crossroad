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
            printf("genere voiture avec au max %d ms d'ecart entre 2 voitures\n", atoi(argv[i+1]));
            
        }
        if (strcmp(argv[i],"-t")==0) {
            printf("temps d'attente aux feux %d ms\n", atoi(argv[i+1]));
            
            //tempsMax=atoi(argv[i+1])
        }
        if (strcmp(argv[i],"-n")==0) {
            printf("nb max voiture %d\n", atoi(argv[i+1]));
            
            //tempsMax=atoi(argv[i+1])
        }
        if (strcmp(argv[i],"-f")==0) {
            printf("file name %s\n", argv[i+1]);
            
            //tempsMax=atoi(argv[i+1])
        }
    }
    }
    return 0;
}