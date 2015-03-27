//
//  carrefour.h
//  
//
//
//

#ifndef ____carrefour__
#define ____carrefour__
#include <unistd.h>

#include <stdio.h>
/***********************************
 *  consants
 * \def PRIMARY_ROUTE
 * \def SECONDARY_ROUTE
 **********************************/

#define PRIMARY_ROUTE 0
#define SECONDARY_ROUTE 1

#define RED 0
#define GREEN 1

#define NB_ROADLIGHTS 2
/***********************************
 *  shared memory
 * crossroads = 0 means that they 
 * are not cars in the crossroad
 * crossroads != 0 they are cars
 **********************************/
typedef struct {
    int numberOfCarsInCrossroads;
    int end;
    int numberOfAllCarCreated;
    int firstRoadLights;
    int secondRoadLights;
    int nbCarWaitingFirstRoadLights;
    int nbCarWaitingSecondRoadLights;
    int timeToWaitRoadLights;

} Shared;
extern Shared * shared;



/**********************************
 * Semaphores
 *********************************/

extern int roadLights[NB_ROADLIGHTS];
extern int drive[NB_ROADLIGHTS];

/*********************************
 * Mutex
 *********************************/
extern int crossroadMutex;
extern int nbCarWaitingFirstRoadLightsMutex;
extern int nbCarWaitingSecondRoadLightsMutex;
extern int numberOfAllCarCreatedMutex;


/***** the shared memory key's ****/
extern int sharedKey;// now we can detached all the link, no matter where it found

void createProcessConntrolRoadLight();
void changeRoadLights(int route);

#endif /* defined(____carrefour__) */
