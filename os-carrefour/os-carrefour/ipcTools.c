//
//  ipcTools.c
//  
//
//  Created by Gada Rezgui on 17/02/2015.
//
//

#include "ipcTools.h"

/*******************************
 * cree un MUTEX ou SEMAPHORE
 * La fonction renvoie le semid associe en cas de succes, -1 en cas
 *	d'echec.
 * Si le semaphore existe deja, la valeur initiale est ignoree.
 *******************************/
int semalloc(key_t key, int valInit) {
    int semid = semget(key,1,0);
    if (semid == -1){
        semid = semget(key,1,IPC_CREAT|IPC_EXCL|0600);
        if (semid == -1)
            return -1;
        if (semctl(semid,0,SETVAL,valInit) == -1) {
            /* We have to destruct the semaphore */
            semctl(semid,0,IPC_RMID,0);
            return -1;
        }
    }
    return semid;
}
int mutalloc(key_t key) { return semalloc(key, 1); }

static   struct sembuf sP = {0,-1,0};	/* decrementation of the mutex */
static   struct sembuf sV = {0, 1,0};	/* incrementation of the mutex */

/*******************************
 * wait a MUTEX or a SEMAPHORE
 *******************************/
void P(int semid){
    semop(semid,&sP,1);
}

/*******************************
 * freed a MUTEX or a SEMAPHORE
 *******************************/
void V(int semid) {
    semop(semid,&sV,1);
}

/*******************************
 * destruct a MUTEX or a SEMAPHORE
 *
 * if the sem is already freed,
 * return -1
 *******************************/
int semfree(int semid) {
    return semctl(semid,0,IPC_RMID,0);
}


/********************************
 * create a shared memory
 *
 * return the adress of area
 * return 0, if they are an error
 *
 *******************************/
void * shmalloc(key_t key, int size) {
    void * res;
    int owner = 0;                /* did *I* create the SHM area ? */
    /* on essaie d'abord une zone existante */
    int shmid = shmget(key,1,0600);
    if (shmid == -1) {
        owner = 1;                  /* yes I did */
        shmid = shmget(key,size,IPC_CREAT|IPC_EXCL|0600);
    }
    if (shmid == -1) return 0;
    res =  shmat(shmid, 0, 0);
    if (res == (void*)-1) {
        if (owner) shmctl(shmid, IPC_RMID, 0);
        return 0;
    }
    return res;
}
 int shmstat(key_t key, struct shmid_ds * buf) {
    // Check if it exists locally
    int shmid = shmget(key, 0, 0600);
    if (shmid == -1) {
        perror("not accessible");
        return -1;
    }
    // put stats in buf
    return shmctl(shmid, IPC_STAT, buf);
}

 int shmattaches(key_t key) {
    struct shmid_ds stats;
    if (shmstat(key, &stats) == -1) {
        perror("stats failed");
        return -1;
    }
    return stats.shm_nattch;
}

/****************************
 * freed the SM
 *
 * return 0, if it succed
 * return -1, if it was
 * already freed or if they
 * are an error
 *
 ***************************/
int shmfree(key_t key, void * addr) {
    // Check if it exists
    int shmid = shmget(key, 0, 0600);
    if (shmid == -1) {
        perror("shared memory space already free'd");
        return 0;
    }
    
    // Detach
    if (shmdt(addr) == -1) {
        perror("shmdt failed");
        return -1;
    }
    
    // Get stats and destroy if nobody is attached anymore
    struct shmid_ds stats;
    if (shmctl(shmid, IPC_STAT, &stats) == -1) {
        perror("stats failed");
        return -1;
    }
    if (stats.shm_nattch > 0) {
//        printf("shared memory still attached (%d links), destroy aborted\n", stats.shm_nattch);
        return -1;
    } else {
        return shmctl(shmid, IPC_RMID, 0);
    } 
}

/*********************************************************************/
/*******************************
 *******************************/
int mutfree(int mId) { return semfree(mId); }
