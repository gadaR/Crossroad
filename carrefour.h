//
/// \file  carrefour.h
//  
///\author { Gada REZGUI and Alexandre FAUCHER}
//
//

#ifndef ____carrefour__
#define ____carrefour__
#include <unistd.h>

#include <stdio.h>
/***********************************
 *  constants
 *
 ***********************************
 * \def PRIMARY_ROUTE
 *
 * \def SECONDARY_ROUTE
 *
 * \def RED
 *
 * \def GREEN 
 *
 * \def NB_ROADLIGHTS
 **********************************/

#define PRIMARY_ROUTE 0
#define SECONDARY_ROUTE 1

#define RED 0
#define GREEN 1

#define NB_ROADLIGHTS 2
/***********************************
 *  shared memory
 *
 **********************************
 * \struct Shared carrefour.h
 * \brief structure which includes 
 *        all the shared memory
 **********************************/
typedef struct {
    int numberOfCarsInCrossroads;/*!< numberOfCarsInCrossroad = 0 mean that they
                                  * are no car in the crossroad
                                  * numberOfCarsInCrossroad != 0 they are cars */
    int end;/*!< end of the program */
    int numberOfAllCarCreated;/*!< we use it in the cars' index */
    int firstRoadLights;/*!< indique the color of the roadLight, in the primary road. */
    int secondRoadLights;/*!< indique the color of the roadLight, in the primary road. */
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

/*************************************************************************
 * \fn void changeRoadLights(int route)
 * \brief manage the circulation in the crossroad.
 *
 * \param route the route which we choose to manage.
 * \return O
 *************************************************************************/
void changeRoadLights(int route);

#endif /* defined(____carrefour__) */
