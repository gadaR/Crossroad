//
/// \file  ipcTools.h
//  
/// \brief contain all the tools we need for using shared memory, mutex and semaphores
///\author { Gada REZGUI and Alexandre FAUCHER}
//
//
#ifndef ____ipcTools__
#define ____ipcTools__

#include <stdio.h>



#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/ipc.h>


/*********************************************************
 * \fn int semalloc(key_t key, int valInit)
 * \brief create  a   SEMAPHORE
 *
 * \param key the key of the semaphore
 * \param valInit his initial value
 * \return semid the semaphore's id, if it succeed
 *
 * If the semaphore already exists, valInit is ignored
 *********************************************************/
int semalloc(key_t key, int valInit);
/*********************************************************
 * \fn int mutalloc(key_t key)
 * \brief create  a MUTEX
 *
 * \param key the key of the semaphore
 * \return semid the semaphore's id, if it succeed
 *
 *********************************************************/
int mutalloc(key_t key);
/******************************************************
 * \fn void P(int semid)
 * \brief wait a MUTEX or a SEMAPHORE
 * \param semid the semaphore's id
 *****************************************************/
void P(int semid);
/*******************************
 * \fn void V(int semid)
 * \brief freed a MUTEX or a SEMAPHORE
 * \param semid the semaphore's id
 *******************************/
void V(int semid);

/*******************************
 * \fn int semfree(int semid)
 *  \brief destruct a  SEMAPHORE
 *
 * \param semid the semaphore's id
 * \return if the sem is already freed
 * return -1, otherwise 0
 *******************************/
int semfree(int semid);
/*******************************
 * \fn int mutfree(int mId)
 *  \brief destruct  a SEMAPHORE
 *
 * \param mId the mutex's id
 * \return if the mutex is already freed
 * return -1, otherwise 0
 *******************************/
int mutfree(int mId);

/********************************
 * \fn void * shmalloc(key_t key, int size)
 * \brief create a shared memory
 *
 * \param key the key of the shared memory
 * \param size
 * \return the adress of area
 * \return 0, if they are an error
 *
 *******************************/
void *  shmalloc( key_t key, int size);
/****************************
 * \fn int shmfree(key_t key, void * addr)
 * \brief freed the SM
 *
 * \param key the key of the shared memory
 * \param addr shared's adress
 * \return 0, if it succed
 * \return -1, if it was
 * already freed or if they
 * are an error
 *
 ***************************/int shmfree(key_t key, void * addr) ;
#endif /* defined(____ipcTools__) */

/************************************************************************/

