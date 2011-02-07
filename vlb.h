#ifndef VLB_H
#define VLB_H

#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define TRUE  (0==0)
#define FALSE (0!=0)
typedef int boolean;


// Two words of memory -- ports A and B
#define VLB_MEMORY_START 0x10000000u
#define VLB_MEMORY_SIZE  (2 * 4)

#define VLB_SHM_ID    941
#define VLB_SHM_SIZE sizeof(vlb_memory_t)
#define VLB_SHM_FLG  0600


typedef struct {
	// For each address, is it writeable?
	boolean writeable[VLB_MEMORY_SIZE];
	
	// Memory where the IO resides
	uint8_t memory[VLB_MEMORY_SIZE];
} vlb_memory_t;


typedef struct {
	key_t         id;
	vlb_memory_t *mem;
	boolean       connected;
} shm_t;


#endif
