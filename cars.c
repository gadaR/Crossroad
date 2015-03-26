//
//  cars.c
//  
//
//  Created by Gada Rezgui on 23/03/2015.
//
//

#include "cars.h"

#include <string.h>
#include <regex.h>


Car genereCar(int route){
    Car car ;
    if (shared->end){
        shmfree(sharedKey, shared);
        exit(0);
    }

        switch (fork()) {
            case -1:
                puts("error during car generation");
            case 0:
                printf("\t CARS : new car %d has just been created on road %d !\n",shared->numberOfAllCarCreated, route);
                car.pid = getpid();
                car.route = route;
                car.index = shared->numberOfAllCarCreated;
                shared->numberOfAllCarCreated ++;
                usleep(300000);// car go in the crossroad
                carsCrossroad(car);
                exit(0);
        }
    
    return car;
    
}

Car genereCarRandomly( int valMax, int nbCarsMax){
    int route = random()%2;
    int timeToWait = random()%valMax;
    Car car ;
    
    usleep(timeToWait*1000);
    if (shared->end){
        shmfree(sharedKey, shared);
        exit(0);
    }
        switch (fork()) {
            case -1:
                puts("error in genere car");
            case 0:
                printf("\t CARS : new car %d has just been created on road %d !\n",shared->numberOfAllCarCreated, route);
                car.pid = getpid();
                car.route = route;
                car.index = shared->numberOfAllCarCreated;
                shared->numberOfAllCarCreated ++;
                usleep(300000);// car go in the crossroad
                if (car.index <= nbCarsMax && car.index > 0) {
                    carsCrossroad(car);
                    
                }
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
                P(drive[PRIMARY_ROUTE]);
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
                usleep(500000);// car go in the crossroad
                
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
                P(drive[SECONDARY_ROUTE]);

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
                usleep(500000);// car go in the crossroad
            
            P(crossroadMutex);
               
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

int compteLig(char * filename){
    
    FILE  *stream = NULL;
    char  line[200];
    int cpt;
    
    size_t slen = strlen (filename);
    if ((slen > 0) && (filename[slen-1] == '\n'))
        filename[slen-1] = '\0';
    if ((stream = fopen(filename, "r")) == NULL) {
        perror("fopen");
        exit (0);
    }
    
    cpt = 0;
    while (fgets(line, sizeof(line), stream) != NULL){
        cpt++;
    }

    fclose (stream);
    return cpt;
}

/* The following is the size of a buffer to contain any error messages
 encountered when the regular expression is compiled. */

#define MAX_ERROR_MSG 0x1000

/* Compile the regular expression described by "regex_text" into
 "r". */

static int compile_regex (regex_t * r, const char * regex_text)
{
    int status = regcomp (r, regex_text, REG_EXTENDED|REG_NEWLINE);
    if (status != 0) {
        char error_message[MAX_ERROR_MSG];
        regerror (status, r, error_message, MAX_ERROR_MSG);
        printf ("Regex error compiling '%s': %s\n",
                regex_text, error_message);
        return 1;
    }
    return 0;
}

/*
 Match the string in "to_match" against the compiled regular
 expression in "r".
 */

static int match_regex (regex_t * r, const char * to_match, int * nbCarsMax, int * valMaxTimeToWaitForCreatingCar)
{
    /* "P" is a pointer into the string which points to the end of the
     previous match. */
    const char * p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = 10;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];
    size_t size ;
    int start;
    int finish;
    char * tab;
    int nomatch;
    while (1) {
        nomatch = regexec (r, p, n_matches, m, 0);
        if (nomatch) {
            printf ("No more matches.\n");
            return nomatch;
        }
        /* 1 */
        if (m[1].rm_so == -1) {
            break;
        }
        start = m[1].rm_so;
        finish = m[1].rm_eo ;
        size = finish -start;
        tab  = malloc (sizeof (*tab) * (size + 1) * 3);
        strncpy (tab, &to_match[start], size);
        *valMaxTimeToWaitForCreatingCar = atoi(tab);
        printf("%d \n",*valMaxTimeToWaitForCreatingCar);
        
        
        /* 2 */
        if (m[2].rm_so == -1) {
            break;
        }
        start = m[2].rm_so;
        finish = m[2].rm_eo ;
        size = finish -start;
        tab  = malloc (sizeof (*tab) * (size + 1) * 3);
        strncpy (tab, &to_match[start], size);
        *nbCarsMax = atoi(tab);
        printf("%d \n",*nbCarsMax);
        
        
        /* 3 */
        if (m[3].rm_so == -1) {
            break;
        }
        start = m[3].rm_so;
        finish = m[3].rm_eo ;
        size = finish -start;
        tab  = malloc (sizeof (*tab) * (size + 1) * 3);
        strncpy (tab, &to_match[start], size);
        shared->timeToWaitRoadLights = atoi(tab);
        printf("%d \n",shared->timeToWaitRoadLights);
        
        
        
        p += m[0].rm_eo;
    }
    return 0;
}

void readFile (char * filename, int * nbCarsMax, int * valMaxTimeToWaitForCreatingCar) {
    int  tmps = 0;
    char * rep;
    FILE  *stream = NULL;
    regex_t preg;
    int match, start, err, end;
    size_t size;
    size_t nmatch = 0;
    regmatch_t *pmatch = NULL;
    char *site = NULL;

    char  line[200];
    int cpt;
    const char *  regex_text = "([[:digit:]]+)[^[:digit:]]*([[:digit:]]*)[^[:digit:]]*([[:digit:]]+)";

    size_t slen = strlen (filename);
    if ((slen > 0) && (filename[slen-1] == '\n'))
        filename[slen-1] = '\0';
    if ((stream = fopen(filename, "r")) == NULL) {
        perror("fopen");
        exit (0);
    }
    
    cpt = 0;
    while (fgets(line, sizeof(line), stream) != NULL){
        printf(" %s \n", line);
        compile_regex(& preg, regex_text);
        match_regex(& preg, line, nbCarsMax, valMaxTimeToWaitForCreatingCar);
        regfree (& preg);
    }
    
    fclose (stream);
    
}

