#ifndef VLB_H
#define VLB_H

#include <sys/ipc.h>
#include <sys/shm.h>

const key_t  VLB_SHM_ID   =  941;
const size_t VLB_SHM_SIZE = 1024;
const size_t VLB_SHM_FLG  = 0600;

#endif

