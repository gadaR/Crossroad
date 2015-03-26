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

int sharedKey;
void changeRoadLights( int timeToWaitRoadLights, int route){
    if (route == PRIMARY_ROUTE) {
    if (shared->numberOfCarsInCrossroads == 0 && shared->secondRoadLights == RED) {
        P(roadLights[PRIMARY_ROUTE]);
        if (shared->end){	/* if prog stopped while we were waiting */
            shmfree(sharedKey, shared);
            exit(0);
        }
            shared->firstRoadLights= GREEN;
<<<<<<< Updated upstream
        printf("CROSSROAD : the roadLight on the road %d is green\n", route);
        V(drive[PRIMARY_ROUTE]);
        usleep(timeToWaitRoadLights*2000);
        shared->firstRoadLights= RED;
        V(roadLights[SECONDARY_ROUTE]);
=======
<<<<<<< HEAD
        printf("CROSSROAD : the roadLight in the route %d is green\n", route);
        if(shared->nbCarWaitingFirstRoadLights>0){
            printf("CROSSROAD : %d cars are freed \n ",shared->nbCarWaitingFirstRoadLights);
            V(drive[PRIMARY_ROUTE]);
        }
        usleep(timeToWaitRoadLights*2000);
        shared->firstRoadLights= RED;
        V(roadLights[SECONDARY_ROUTE]);
        printf("CROSSROAD : the roadLight in the route %d is red \n", route);
//        usleep(timeToWaitRoadLights*1000);
=======
        printf("CROSSROAD : the roadLight on the road %d is green\n", route);
        V(drive[PRIMARY_ROUTE]);
        usleep(timeToWaitRoadLights*2000);
        shared->firstRoadLights= RED;
        V(roadLights[SECONDARY_ROUTE]);
>>>>>>> Stashed changes
        printf("CROSSROAD : the roadLight on the road %d is red \n", route);
        usleep(timeToWaitRoadLights*1000);
>>>>>>> origin/master

    }
    }else if (route ==SECONDARY_ROUTE){
        if (shared->numberOfCarsInCrossroads == 0 && shared->firstRoadLights == RED) {
            P(roadLights[SECONDARY_ROUTE]);
            if (shared->end){	/* if prog stopped while we were waiting */
                shmfree(sharedKey, shared);
                exit(0);
            }
            shared->secondRoadLights= GREEN;
<<<<<<< Updated upstream
            printf("CROSSROAD : the roadLight on the road %d is green\n", route);
            V(drive[SECONDARY_ROUTE]);
            usleep(timeToWaitRoadLights*1000);
            shared->secondRoadLights= RED;
            V(roadLights[PRIMARY_ROUTE]);
=======
<<<<<<< HEAD
            printf("CROSSROAD : the roadLight in the route %d is green\n", route);
            if(shared->nbCarWaitingSecondRoadLights>0){
                printf("CROSSROAD : %d cars are freed \n ",shared->nbCarWaitingSecondRoadLights);
                V(drive[SECONDARY_ROUTE]);
            }
            usleep(timeToWaitRoadLights*1000);
            shared->secondRoadLights= RED;
            V(roadLights[PRIMARY_ROUTE]);
            printf("CROSSROAD : the roadLight in the route %d is red \n", route);
//            usleep(timeToWaitRoadLights*2000);
=======
            printf("CROSSROAD : the roadLight on the road %d is green\n", route);
            V(drive[SECONDARY_ROUTE]);
            usleep(timeToWaitRoadLights*1000);
            shared->secondRoadLights= RED;
            V(roadLights[PRIMARY_ROUTE]);
>>>>>>> Stashed changes
            printf("CROSSROAD : the roadLight on the road %d is red \n", route);
            usleep(timeToWaitRoadLights*2000);
>>>>>>> origin/master

        }
    }
    
}
int main (int argc, char ** argv){
    int timeToWaitRoadLights=1000;//in ms, by default we wait 1s econd
    int i, roadLightsKey[NB_ROADLIGHTS], crossroadMutexKey, driveKey[NB_ROADLIGHTS];
    Car car;
    char rep;
    /* take Key */
    sharedKey =  ftok("/etc/passwd", 0);
    crossroadMutexKey = sharedKey +1;
  
    /* shared memory / semaphore initialisation  */
    shared = (Shared*)shmalloc(sharedKey, sizeof(Shared));
    if (!shared) {
        perror("error when allocating shared memory");
        return 1;
    }
    shared->end = 0;
    shared->numberOfCarsInCrossroads=0;
    shared->numberOfAllCarCreated=0;
    shared->firstRoadLights=GREEN;
    shared->secondRoadLights=RED;
    shared->nbCarWaitingFirstRoadLights = 0;
    for (i = 0; i < NB_ROADLIGHTS; i++) {
        roadLightsKey[i] = crossroadMutexKey + 1 + i;
        roadLights[i] = semalloc(roadLightsKey[i],0);
        if (roadLights[i] == -1) {
            int j = i-1;
            perror("error during semaphore creation");
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
            perror("error during semaphore creation");
            while (j>=0)
                semfree(driveKey[j--]);
            shmfree(sharedKey, shared);
            return 2;
        }
    }

    crossroadMutex = mutalloc(crossroadMutexKey);
    if (crossroadMutex == -1) {
        perror("error during mutex creation");
        shmfree(sharedKey, shared);
            mutfree(crossroadMutexKey);
        return 2;
    }
   
    switch (fork()) {
        case -1:
            perror("error in fork");
            break;
        case 0 :
            V(roadLights[PRIMARY_ROUTE]);
            while (!shared->end) {
                changeRoadLights( timeToWaitRoadLights ,PRIMARY_ROUTE);
            }
            shmfree(sharedKey, shared);
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
            shmfree(sharedKey, shared);
            exit(0);
            break;
        default:
            break;
    }
    
    /*** interactif mode ***/
    if (argc == 1) {
        puts("you choose the interactive mode\n");
        
        puts("press q if you want to exit\n");
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
        puts("press : \n - P if you want to create a car in the first route \n - S in the second one \n");
                while (1) {
=======
>>>>>>> Stashed changes
        puts("press : \n - P if you want to create a car on the first road \n - S on the second one \n");
        while (1) {
>>>>>>> origin/master
            
            scanf("%c", &rep);
            getchar();
            
            switch (rep) {
                    
                case 'P' :
                case 'p' :
                    
                    car = genereCar(PRIMARY_ROUTE);
                    usleep(100000);
                    carsCrossroad(car);
                    if (shared->end){
                        shmfree(sharedKey, shared);
                        exit(0);
                    }
                    break;
                case 'S' :
                case 's' :
                    
                    car = genereCar(SECONDARY_ROUTE);
                    usleep(100000);
                    carsCrossroad(car);
                    if (shared->end){
                        shmfree(sharedKey, shared);
                        exit(0);
                    }
                    break;
                case 'Q' :
                case 'q' :
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
                    puts(" Error : you can only press : \n - P if you want to create a car in the first route \n - S in the second one \n");
                    break;
            }
        }
    
                    }else{
                        int nbCarsMax = 50;
                        int valMaxTimeToWaitForCreatingCar = 1000;
                        /** read the argument **/
                        for (i=0; i<argc; i++) {
                            
                            /*** automatic mode ***/
                            if (strcmp(argv[i],"-a")==0) {
                                puts("auto\n");
                                valMaxTimeToWaitForCreatingCar = atoi(argv[i+1]);
                                printf("You choose to wait %d ms before creating another car \n", valMaxTimeToWaitForCreatingCar);
                                
                            }
                            if (strcmp(argv[i],"-t")==0) {
                                timeToWaitRoadLights = atoi(argv[i+1]);
                                printf("You choose to wait %d ms before the the roadlights turn to green, (the primary will wait 2 * %d ms, and the secondary %d ms\n", timeToWaitRoadLights, timeToWaitRoadLights, timeToWaitRoadLights);
                                
                                //tempsMax=atoi(argv[i+1])
                            }
                            if (strcmp(argv[i],"-n")==0) {
                                nbCarsMax =atoi(argv[i+1]);
                                printf("number max of cars %d\n", nbCarsMax );
                                
                                //carMax=atoi(argv[i+1])
                            }
                            if (strcmp(argv[i],"-f")==0) {
                                printf("file name %s\n", argv[i+1]);
                                
                            }
                        }
                        Car car;
                        do {
                            car = genereCarRandomly( 400);
                            sleep(4);
                            carsCrossroad(car);
                        } while (shared->numberOfAllCarCreated<10);
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

