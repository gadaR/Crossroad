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
                puts("error during car generationr");
            case 0:
                printf("\t CARS : new car %d has just been created on road %d !\n",shared->numberOfAllCarCreated, route);
                car.pid = getpid();
                car.route = route;
                car.index = shared->numberOfAllCarCreated;
                shared->numberOfAllCarCreated ++;
        }
    return car;

    
}

void carsCrossroad(Car car){
    int fire;
    switch (car.route) {
        case PRIMARY_ROUTE:
            fire =0;
            if (shared->firstRoadLights == RED) {
                printf("\t CARS : the car %d is waiting the roadLights to turn to green in the primary route\n", car.index);
                P(drive[PRIMARY_ROUTE]);
            }
            //entrer ds le carrefour

            if (shared->firstRoadLights == GREEN) {
                P(crossroadMutex);
                if (shared->end) exit(0);		/* if prog stopped while we were waiting */
                
                printf("\t CARS : the car %d is on the crossroad \n", car.index);
                
                shared->numberOfCarsInCrossroads ++;
                
                V(crossroadMutex);
                usleep(random() % 10000000);// car go in the crossroad
                
                P(crossroadMutex);
                if (shared->end) exit(0);		/* if prog stopped while we were waiting */
                
                shared->numberOfCarsInCrossroads --;
                V(crossroadMutex);
                printf("\t CARS : the car %d left the crossroad \n", car.index);
//                V(roadLights[SECONDARY_ROUTE]);

            }
                        break;
            
        case SECONDARY_ROUTE:
            fire = 1;
            if (shared->secondRoadLights == RED) {
                printf("\t CARS : the car %d is waiting the roadLights to turn to green on the secondary road \n", car.index);
                P(drive[SECONDARY_ROUTE]);
            }
            if (shared->secondRoadLights == GREEN) {

//            V(roadLights[fire]);
            //entrer ds le carrefour
            
            P(crossroadMutex);
            if (shared->end) exit(0);		/* if prog stopped while we were waiting */
            
            printf("\t CARS : the car %d is on the crossroad \n", car.index);
            
            shared->numberOfCarsInCrossroads ++;
            
            V(crossroadMutex);
            usleep(random() % 10000000);// car go in the crossroad
            
            P(crossroadMutex);
            if (shared->end) exit(0);		/* if prog stopped while we were waiting */
            
            shared->numberOfCarsInCrossroads --;
            V(crossroadMutex);
            printf("\t CARS : the car %d left the crossroad \n", car.index);
                exit(0);
            }
            break;
        default:
            break;
    }
   
}
