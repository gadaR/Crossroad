//
//  carrefour.c
//  
//
//
//

#include "carrefour.h"


int main (int argc, char ** argv){
    int i;
    /*** interactif mode ***/
    if (argc == 1) {
        //mode interactif
    }else{
    
    for (i=1; i<argc; i++) {
        /*** automatic mode ***/
        if (strcmp(argv[i],"-a")) {
            if (strcmp(argv[i],"-t")) {
                //tempsMax=atoi(argv[i+1])
            }
        } else {
            <#statements#>
        }
    }
    }
    return 0;
}