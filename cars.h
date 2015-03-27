//
/// \file cars.h
/// \brief cars header
//
///  \author {  Gada Rezgui and  Alexandre FAUCHER}
/// \date 23/03/2015
//

#ifndef ____cars__
#define ____cars__

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include "carrefour.h"
#include "ipcTools.h"
/**********************
 * \struct Car cars.h
 * \brief create a car.
 **********************/
typedef struct{
    int pid;
    int route;
    int index;
}Car;
/*************************************************************************
 * \fn Car genereCar(int route)
 * \brief genere car (process)
 *
 * \param route the route we choose to manage.
 * \return car the car we genere
 *************************************************************************/
Car genereCar(int route);
/*************************************************************************
 * \fn Car genereCarRandomly( int valMax, int nbCarsMax)
 * \brief genere car (process)
 *
 * \param valMax the maximum value that the user choose to wait.
 * \param nbCarsMax maximum number of cars that we have to create.
 * \return car the car we genere
 *************************************************************************/
Car genereCarRandomly( int valMax, int nbCarsMax);
/*************************************************************************
 * \fn void carsCrossroad(Car car)
 * \brief manage the circulation of the car in the crossroad.
 *
 * \param car the car we choose to manage.
 *************************************************************************/
void carsCrossroad(Car car);
#endif /* defined(____cars__) */
