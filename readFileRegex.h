//
/// \file readFileRegex.h
//  
//
/// \brief read from a file informations, then analyse them with the regular expression
///\author { Gada REZGUI and Alexandre FAUCHER}
//

//

#ifndef ____readFileRegex__
#define ____readFileRegex__

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <regex.h>
#include "carrefour.h"

/*************************************************************************
 * \fn readFile (char * filename, int * nbCarsMax, int * valMaxTimeToWaitForCreatingCar)
 * \brief read data from a file
 *
 * \param filename the name of the file
 * \param nbCarsMax pointer
 * \param valMaxTimeToWaitForCreatingCar pointer
 *************************************************************************/
void readFile (char * filename, int * nbCarsMax, int * valMaxTimeToWaitForCreatingCar);
#endif /* defined(____readFileRegex__) */
