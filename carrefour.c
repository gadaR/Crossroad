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
void changeRoadLights(  int route){
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
//        printf("\n\ntime to wait %d \n\n", shared->timeToWaitRoadLights);
        usleep(shared->timeToWaitRoadLights*2000);
        shared->firstRoadLights= RED;
        V(roadLights[SECONDARY_ROUTE]);
        if (shared->end){	/* if prog stopped while we were waiting */
            shmfree(sharedKey, shared);
            exit(0);
        }
        printf("CROSSROAD : the roadLight on the route %d is red \n", route);
        

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
            usleep(shared->timeToWaitRoadLights*1000);
            shared->secondRoadLights= RED;
            V(roadLights[PRIMARY_ROUTE]);
            if (shared->end){	/* if prog stopped while we were waiting */
                shmfree(sharedKey, shared);
                exit(0);
            }
            printf("CROSSROAD : the roadLight on the route %d is red \n", route);

        }
    }
    
}
int main (int argc, char ** argv){
    int i, roadLightsKey[NB_ROADLIGHTS], crossroadMutexKey, driveKey[NB_ROADLIGHTS];
    int pidPrimaryRoadLight, pidSecondRoadLight;
    Car car;
    char rep;
    /* take Key */
    sharedKey =  ftok("/etc/passwd", 0);
    crossroadMutexKey = sharedKey +1;
    srand(time(NULL));

    /* shared memory / semaphore initialisation  */
    shared = (Shared*)shmalloc(sharedKey, sizeof(Shared));
    if (!shared) {
        perror("error when allocating shared memory");
        return 1;
    }
    shared->end = 0;
    shared->numberOfCarsInCrossroads=0;
    shared->numberOfAllCarCreated=1;
    shared->firstRoadLights=GREEN;
    shared->secondRoadLights=RED;
    puts("CROSSROAD : the roadLight on the route 1 is red ");
    shared->nbCarWaitingFirstRoadLights = 0;
    shared->timeToWaitRoadLights=1000;//in ms, by default we wait 1s econd
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
   
    switch (pidPrimaryRoadLight = fork()) {
        case -1:
            perror("error in fork");
            break;
        case 0 :
            V(roadLights[PRIMARY_ROUTE]);
            while (!shared->end) {
                changeRoadLights(PRIMARY_ROUTE);
            }
            shmfree(sharedKey, shared);
            exit(0);
            break;
        default:
            break;
    }
                        
    switch (pidSecondRoadLight = fork()) {
        case -1:
            perror("error in fork");
            break;
        case 0:
            while (!shared->end) {
                    changeRoadLights(SECONDARY_ROUTE);
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
                    if (shared->end){
                        shmfree(sharedKey, shared);
                        exit(0);
                    }
                    break;
				case 'S' :
                case 's' :
                    
                    car = genereCar(SECONDARY_ROUTE);
                    if (shared->end){
                        shmfree(sharedKey, shared);
                        exit(0);
                    }
                    break;
				case 'Q' :
                case 'q' :
                    /* quit */
                    /********* release memory ***/
                    shared->end =1;
                    for (i = 0; i < NB_ROADLIGHTS; i++)
                        semfree(roadLights[i]);
                    mutfree(crossroadMutex);
                    for (i = 0; i < NB_ROADLIGHTS; i++)
                        semfree(drive[i]);
                    shmfree(sharedKey, shared);
                    puts("END CROSSROAD");
                    exit(0);
                default:
                    puts(" Error : you can only press : \n - P if you want to create a car in the first route \n - S in the second one \n");
                    if (shared->end){	/* if prog stopped while we were waiting */
                        shmfree(sharedKey, shared);
                        exit(0);
                    }
                    break;
            }
        }
    
                    }else{
                        int nbCarsMax = 30;
                        int valMaxTimeToWaitForCreatingCar = 1000;
                        /** read the argument **/
                        for (i=0; i<argc; i++) {
                            
                            /*** automatic mode ***/
                            if (strcmp(argv[i],"-a")==0) {
                                valMaxTimeToWaitForCreatingCar = atoi(argv[i+1]);
                                
                            }
                            if (strcmp(argv[i],"-t")==0) {
                                shared->timeToWaitRoadLights = atoi(argv[i+1]);
                                
                                //tempsMax=atoi(argv[i+1])
                            }
                            if (strcmp(argv[i],"-n")==0) {
                                nbCarsMax =atoi(argv[i+1]);

                                
                                //carMax=atoi(argv[i+1])
                            }
                            if (strcmp(argv[i],"-f")==0) {
                                readFile(argv[i+1], &nbCarsMax, &valMaxTimeToWaitForCreatingCar);
//                                printf(" a= %d, n = %d \n", valMaxTimeToWaitForCreatingCar, nbCarsMax);
                                
                            }
                        }
                        Car car;
                         while (shared->numberOfAllCarCreated < nbCarsMax) {
                            car = genereCarRandomly(valMaxTimeToWaitForCreatingCar, nbCarsMax);
                        }
                        /* quit */
                        /********* release memory ***/
                        shared->end =1;
                        for (i = 0; i < NB_ROADLIGHTS; i++)
                            semfree(roadLights[i]);
                        mutfree(crossroadMutex);
                        for (i = 0; i < NB_ROADLIGHTS; i++)
                            semfree(drive[i]);
                        shmfree(sharedKey, shared);

                        puts("END CROSSROAD");

                        exit(0);
                        
                        
                    }
   
    while (waitpid(car.pid,0,0));
    while (waitpid(pidPrimaryRoadLight,0,0));
    while (waitpid(pidSecondRoadLight,0,0));
    puts("END CROSSROAD");

    return 0;
    
    
}

