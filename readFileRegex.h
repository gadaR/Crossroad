//
/// \file readFileRegex.h
//  
//
/// \brief reaf from a file informations, then analyse them with the regular expression
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

void readFile (char * filename, int * nbCarsMax, int * valMaxTimeToWaitForCreatingCar);
#endif /* defined(____readFileRegex__) */
