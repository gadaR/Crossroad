//
/// \file  carrefour.c
/// \brief main program  
///\author { Gada REZGUI and Alexandre FAUCHER}
//
//
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cars.h"
#include "carrefour.h"

Shared * shared;
/**********************************
 * semaphores
 *********************************/
int crossroadMutex;
int drive[NB_ROADLIGHTS];

int roadLights[NB_ROADLIGHTS];


void changeRoadLights( int timeToWaitRoadLights, int route){
    if (route == PRIMARY_ROUTE) {

    if (shared->numberOfCarsInCrossroads == 0 && shared->secondRoadLights == RED) {
//        printf("bla le 2 feu %d (0rouge 1 vert) et le 1 est %d , voiture ds le carrefour %d\n\n", shared->secondRoadLights, shared->firstRoadLights,shared->numberOfCarsInCrossroads );
        V(roadLights[PRIMARY_ROUTE]);
        if (shared->end) exit(0);		/* if prog stopped while we were waiting */
            shared->firstRoadLights= GREEN;
        printf("CROSSROAD : the roadLight in the route %d is green\n", route);
        V(drive[PRIMARY_ROUTE]);
        usleep(timeToWaitRoadLights*2000);
        shared->firstRoadLights= RED;
        V(roadLights[SECONDARY_ROUTE]);
        printf("CROSSROAD : the roadLight in the route %d is red \n", route);
        usleep(timeToWaitRoadLights*1000);

    }
    }else if (route ==SECONDARY_ROUTE){
        if (shared->numberOfCarsInCrossroads == 0 && shared->firstRoadLights == RED) {
//printf("boum le 2 feu %d (0rouge 1 vert) et le 1 est %d , voiture ds le carrefour %d\n\n", shared->secondRoadLights, shared->firstRoadLights,shared->numberOfCarsInCrossroads );
            P(roadLights[SECONDARY_ROUTE]);
            if (shared->end) exit(0);		/* if prog stopped while we were waiting */
            shared->secondRoadLights= GREEN;
            printf("CROSSROAD : the roadLight in the route %d is green\n", route);
            V(drive[SECONDARY_ROUTE]);
            usleep(timeToWaitRoadLights*1000);
            shared->secondRoadLights= RED;
            V(roadLights[PRIMARY_ROUTE]);
            printf("CROSSROAD : the roadLight in the route %d is red \n", route);
            usleep(timeToWaitRoadLights*2000);

        }
    }
    
}
int main (int argc, char ** argv){
    int timeToWaitRoadLights=10000;//in ms
    int i, sharedKey, roadLightsKey[NB_ROADLIGHTS], crossroadMutexKey, driveKey[NB_ROADLIGHTS];
    Car car;
    char rep;
    /* take Key */
    sharedKey =  ftok("/etc/passwd", 0);
    crossroadMutexKey = sharedKey +1;
  
    /* shared memory / semaphore initialisation  */
    shared = (Shared*)shmalloc(sharedKey, sizeof(Shared));
    if (!shared) {
        perror("error allocating shared memory");
        return 1;
    }
    shared->end = 0;
    shared->numberOfCarsInCrossroads=0;
    shared->numberOfAllCarCreated=0;
    shared->firstRoadLights=GREEN;
    shared->secondRoadLights=RED;
    for (i = 0; i < NB_ROADLIGHTS; i++) {
        roadLightsKey[i] = crossroadMutexKey + 1 + i;
        roadLights[i] = semalloc(roadLightsKey[i],0);
        if (roadLights[i] == -1) {
            int j = i-1;
            perror("error creating semaphore");
            while (j>=0)
                semfree(roadLightsKey[j--]);
            shmfree(sharedKey, shared);
            return 2;
        }
    }
    for (i = 0; i < NB_ROADLIGHTS; i++) {
        driveKey[i] = roadLightsKey[1] + 1 + i;
        drive[i] = semalloc(driveKey[i],0);
        if (drive[i] == -1) {
            int j = i-1;
            perror("error creating semaphore");
            while (j>=0)
                semfree(driveKey[j--]);
            shmfree(sharedKey, shared);
            return 2;
        }
    }

    crossroadMutex = mutalloc(crossroadMutexKey);
    if (crossroadMutex == -1) {
        perror("error creating mutex");
        shmfree(sharedKey, shared);
            mutfree(crossroadMutexKey);
        return 2;
    }
   
    switch (fork()) {
        case -1:
            perror("error in fork");
            break;
        case 0 :
            while (!shared->end) {
                changeRoadLights( timeToWaitRoadLights ,PRIMARY_ROUTE);
            }
            exit(0);
            break;
        default:
            break;
    }
                        
    switch (fork()) {
        case -1:
            perror("error in fork");
            break;
        case 0:
            while (!shared->end) {
                    changeRoadLights(timeToWaitRoadLights,SECONDARY_ROUTE);
            }
            exit(0);
            break;
        default:
            break;
    }
    
    /*** interactif mode ***/
    if (argc == 1) {
        puts("you choose the interactif mode\n");
        
        puts("press q if you want to exit\n");
        puts("press : \n - P if you want to create a car in the first route \n - S in the second one \n");
        while (1) {
            
            scanf("%c", &rep);
            getchar();
            
            switch (rep) {
                    
                case 'P'|'p':
                    
                    car = genereCar(PRIMARY_ROUTE);
                    usleep(random() % 1000000);
                    carsCrossroad(car);
                    if (shared->end) exit(0);
                    break;
                case 'S'|'s':
                    
                    car = genereCar(SECONDARY_ROUTE);
                    usleep(random() % 1000000);
                    carsCrossroad(car);
                    if (shared->end) exit(0);
                    break;
                case 'q'|'Q':
                    /* quit */
                    puts("quit");
                    /********* release memory ***/
                    shared->end =1;
                    for (i = 0; i < NB_ROADLIGHTS; i++)
                    printf("%d routeLoad semaphore is %d (if 0 : it's released correctly) \n",i,semfree(roadLights[i]));
                    printf("mutex %d (if 0 : it's released correctly)\n",mutfree(crossroadMutex));
                    for (i = 0; i < NB_ROADLIGHTS; i++)
                    printf(" drive semaphore is %d (if 0 : it's released correctly) \n",semfree(drive[i]));
                    shmfree(sharedKey, shared);
                    exit(0);
                default:
                    puts(" Error : you have only to press : \n - P if you want to create a car in the first route \n - S in the second one \n");
                    break;
            }
        }
    
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
                                
                                //carMax=atoi(argv[i+1])
                            }
                            if (strcmp(argv[i],"-f")==0) {
                                printf("file name %s\n", argv[i+1]);
                                
                            }
                        }
                    }
                    
    /********* release memory ***/
    for (i = 0; i < NB_ROADLIGHTS; i++)
        printf("%d routeLoad semaphore is %d (if 0 : it's released correctly) \n",i,semfree(roadLights[i]));
    shmfree(sharedKey, shared);
    mutfree(crossroadMutex);
    for (i = 0; i < NB_ROADLIGHTS; i++)
        printf(" drive semaphore is %d (if 0 : it's released correctly) \n",semfree(drive[i]));
    


   
    return 0;
    
    
}

