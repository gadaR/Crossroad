//
//  ipcTools.h
//  
//
//  Created by Gada Rezgui on 17/02/2015.
//
//
/*************************************************************************
 *
 *
 * Utilisation des segments de memoire partagee et des semaphores
 * pour la synchronisation de processus collaboratifs
 *
 ************************************************************************/
#ifndef ____ipcTools__
#define ____ipcTools__

#include <stdio.h>



#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/ipc.h>
/*******************************
 * create a SEMAPHORE or a MUTEX.
 *
 * La fonction renvoie le semid associe en cas de succes, -1 en cas
 *	d'echec.
 * Si le semaphore existe deja, la fonction le renvoie simplement.
 *******************************/
int semalloc(key_t key, int valInit);
int mutalloc(key_t key);
/*******************************
 * wait a MUTEX or a SEMAPHORE
 *******************************/
void P(int semid);

/*******************************
 * freed a MUTEX or a SEMAPHORE
 *******************************/
void V(int semid);

/*******************************
 * destruct a MUTEX or a SEMAPHORE
 *
 * if the sem is already freed,
 * return -1
 *******************************/
int semfree(int semid);


/********************************
 * create a shared memory
 *
 * return the adress of area
 * return 0, if they are an error
 *******************************/
void *  shmalloc( key_t key, int size);
//static int shmstat(key_t key, struct shmid_ds * buf) ;
//static int shmattaches(key_t key);
/****************************
 * freed the SM
 *
 * return 0, if it succed
 * return -1, if it was
 * already freed or if they
 * are an error
 ***************************/
int shmfree(key_t key, void * addr) ;
/*******************************
 *******************************/
int mutfree(int mId);
#endif /* defined(____ipcTools__) */

/************************************************************************/

