//
//  cars.c
//  
//
//  Created by Gada Rezgui on 23/03/2015.
//
//

#include "cars.h"




Car genereCar(int route){
    Car car ;
        switch (fork()) {
            case -1:
                puts("error during car generation");
            case 0:
                printf("\t CARS : new car %d has just been created on road %d !\n",shared->numberOfAllCarCreated, route);
                car.pid = getpid();
                car.route = route;
                car.index = shared->numberOfAllCarCreated;
                shared->numberOfAllCarCreated ++;
        }
    if (shared->end){	/* if prog stopped while we were waiting */
        shmfree(sharedKey, shared);
        exit(0);
    }
    return car;

    
}

Car genereCarRandomly( int valMax){
    int route = random()%2;
    int timeToWait = random()%valMax;
    Car car ;

    usleep(timeToWait*1000);
        switch (fork()) {
            case -1:
                puts("error in genere car");
            case 0:
                printf("\t CARS : new car %d has just been created on road %d !\n",shared->numberOfAllCarCreated, route);
                car.pid = getpid();
                car.route = route;
                car.index = shared->numberOfAllCarCreated;
                shared->numberOfAllCarCreated ++;
        }

    
        if (shared->end){
        shmfree(sharedKey, shared);
        exit(0);
    }
    return car;
    
    
}

void carsCrossroad(Car car){
    switch (car.route) {
        case PRIMARY_ROUTE:
            if (shared->firstRoadLights == RED) {
                printf("\t CARS : the car %d is waiting the roadLights to turn to green in the primary route\n", car.index);
                shared->nbCarWaitingFirstRoadLights ++;
                printf("\t CARS : %d car(s) are/is waiting in the principal route \n ",shared->nbCarWaitingFirstRoadLights);
                if (shared->end){
                    shmfree(sharedKey, shared);
                    exit(0);
                }
                P(drive[PRIMARY_ROUTE]);
                if (shared->end){	/* if prog stopped while we were waiting */
                    shmfree(sharedKey, shared);
                    exit(0);
                }
                shared->nbCarWaitingFirstRoadLights --;
            }
            //entrer ds le carrefour

            if (shared->firstRoadLights == GREEN) {
                P(crossroadMutex);
                if (shared->end){	/* if prog stopped while we were waiting */
                    shmfree(sharedKey, shared);
                    exit(0);
                }
                printf("\t CARS : the car %d is on the crossroad \n", car.index);

                
                shared->numberOfCarsInCrossroads ++;
                printf("CROSSROAD : %d car(s) in the crossroad\n",shared->numberOfCarsInCrossroads );
                V(crossroadMutex);
                usleep(100000);// car go in the crossroad
                
                P(crossroadMutex);
                if (shared->end){	/* if prog stopped while we were waiting */
                    shmfree(sharedKey, shared);
                    exit(0);
                }
                shared->numberOfCarsInCrossroads --;
                V(crossroadMutex);
                printf("\t CARS : the car %d left the crossroad \n", car.index);
                printf("CROSSROAD : %d car(s) in the crossroad\n",shared->numberOfCarsInCrossroads );
//                shmfree(sharedKey, shared);
//                exit(0);
            }
                        break;
            
        case SECONDARY_ROUTE:
            if (shared->secondRoadLights == RED) {
                printf("\t CARS : the car %d is waiting the roadLights to turn to green on the secondary road \n", car.index);

                shared->nbCarWaitingSecondRoadLights ++;
                printf("\t CARS : %d car(s) are/is waiting in the secondary route \n ",shared->nbCarWaitingSecondRoadLights);
                if (shared->end){
                    shmfree(sharedKey, shared);
                    exit(0);
                }
                P(drive[SECONDARY_ROUTE]);
                if (shared->end){	/* if prog stopped while we were waiting */
                    shmfree(sharedKey, shared);
                    exit(0);
                }

                shared->nbCarWaitingSecondRoadLights --;

            }
            if (shared->secondRoadLights == GREEN) {
            //entrer ds le carrefour
            
            P(crossroadMutex);
                if (shared->end){	/* if prog stopped while we were waiting */
                    shmfree(sharedKey, shared);
                    exit(0);
                }
            printf("\t CARS : the car %d is on the crossroad \n", car.index);

            
            shared->numberOfCarsInCrossroads ++;
            printf("CROSSROAD : %d car(s) in the crossroad\n",shared->numberOfCarsInCrossroads );
            V(crossroadMutex);
                usleep(100000);// car go in the crossroad
            
            P(crossroadMutex);
                if (shared->end){	/* if prog stopped while we were waiting */
                    shmfree(sharedKey, shared);
                    exit(0);
                }
            shared->numberOfCarsInCrossroads --;
            V(crossroadMutex);
            printf("\t CARS : the car %d left the crossroad \n", car.index);
            printf("CROSSROAD : %d car(s) in the crossroad\n",shared->numberOfCarsInCrossroads );
//            shmfree(sharedKey, shared);
//                puts("quit");
//            exit(0);
            }
            break;
        default:
            break;
    }
   
}
