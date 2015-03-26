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

        printf("CROSSROAD : the roadLight on the route %d is green\n", route);
        if(shared->nbCarWaitingFirstRoadLights>0){
            printf("CROSSROAD : %d cars are freed \n ",shared->nbCarWaitingFirstRoadLights);
            V(drive[PRIMARY_ROUTE]);
        }
        usleep(timeToWaitRoadLights*2000);
        shared->firstRoadLights= RED;
        V(roadLights[SECONDARY_ROUTE]);
        printf("CROSSROAD : the roadLight on the route %d is red \n", route);
        if (shared->end){	/* if prog stopped while we were waiting */
            shmfree(sharedKey, shared);
            exit(0);
        }

    }
    }else if (route ==SECONDARY_ROUTE){
        if (shared->numberOfCarsInCrossroads == 0 && shared->firstRoadLights == RED) {
            P(roadLights[SECONDARY_ROUTE]);
            if (shared->end){	/* if prog stopped while we were waiting */
                shmfree(sharedKey, shared);
                exit(0);
            }
            shared->secondRoadLights= GREEN;

            printf("CROSSROAD : the roadLight on the route %d is green\n", route);
            if(shared->nbCarWaitingSecondRoadLights>0){
                printf("CROSSROAD : %d cars are freed \n ",shared->nbCarWaitingSecondRoadLights);
                V(drive[SECONDARY_ROUTE]);
            }
            usleep(timeToWaitRoadLights*1000);
            shared->secondRoadLights= RED;
            V(roadLights[PRIMARY_ROUTE]);
            printf("CROSSROAD : the roadLight on the route %d is red \n", route);

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

        puts("press : \n - P if you want to create a car in the first route \n - S in the second one \n");
                while (1) {

            
            scanf("%c", &rep);
            getchar();
            
            switch (rep) {
                   
				case 'P' :
                case 'p' :
                    
                    car = genereCar(PRIMARY_ROUTE);
                    usleep(100000);// car go in the crossroad
                    carsCrossroad(car);
                    if (shared->end){
                        shmfree(sharedKey, shared);
                        exit(0);
                    }
                    break;
				case 'S' :
                case 's' :
                    
                    car = genereCar(SECONDARY_ROUTE);
                    usleep(100000);// car go in the crossroad
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
                                valMaxTimeToWaitForCreatingCar = atoi(argv[i+1]);
                                
                            }
                            if (strcmp(argv[i],"-t")==0) {
                                timeToWaitRoadLights = atoi(argv[i+1]);
                                
                                //tempsMax=atoi(argv[i+1])
                            }
                            if (strcmp(argv[i],"-n")==0) {
                                nbCarsMax =atoi(argv[i+1]);

                                
                                //carMax=atoi(argv[i+1])
                            }
                            if (strcmp(argv[i],"-f")==0) {
                                printf("file name %s\n", argv[i+1]);
                                
                            }
                        }
                        Car car;
                         while (shared->numberOfAllCarCreated<nbCarsMax) {
                            car = genereCarRandomly( valMaxTimeToWaitForCreatingCar);
                             usleep(100000);// car go in the crossroad
                            carsCrossroad(car);
                        }
                        /* quit */
                        puts("END CROSSROAD");
                        /********* release memory ***/
                        shared->end =1;
                        for (i = 0; i < NB_ROADLIGHTS; i++)
                            semfree(roadLights[i]);
                        mutfree(crossroadMutex);
                        for (i = 0; i < NB_ROADLIGHTS; i++)
                            semfree(drive[i]);
                        shmfree(sharedKey, shared);
                    }
    
   
    return 0;
    
    
}

