#include <stdio.h>

#include "vlb.h"

#define TRUE  (0==0)
#define FALSE (0!=0)
typedef int boolean;

boolean shm_connected = FALSE;
int shmid;
void *shm;


boolean
constructor (unsigned char *me, unsigned char *string)
{
	fprintf(stderr, "=============================\n"
	                "= Hello World!\n"
	                "=   me     = '%s'\n"
	                "=   string = '%s'\n"
	                "=============================\n",
	                me, string);
	
	shmid = shmget(VLB_SHM_ID, VLB_SHM_SIZE, IPC_CREAT | VLB_SHM_FLG);
	if (shmid == -1) {
		fprintf(stderr, "Failed to create SHM!");
		return FALSE;
	}
	
	shm = shmat(shmid, NULL, 0);
	// This peutrid cast is specified by SHM
	if (shm == (void *)-1) {
		fprintf(stderr, "Failed to attatch SHM!");
		return FALSE;
	}
	
	*((int *)shm) = 1337;
	
	shm_connected = TRUE;
	
	return TRUE;
}


boolean
destructor (unsigned char *me)
{
	fprintf(stderr, "=============================\n"
	                "= Goodbye, cruel world!\n"
	                "=   me     = '%s'\n"
	                "=============================\n",
	                me);
	
	if (shm_connected)
		shmdt(shm);
	
	return TRUE;
}


boolean mem_w_handler (unsigned int address, unsigned int data, int size,
                       boolean T, int source, boolean* abort)
{
	fprintf(stderr, "=============================\n"
	                "= Write Handler\n"
	                "=   address = 0x%08x\n"
	                "=   data    = 0x%08x\n"
	                "=   size    = 0x%08x\n"
	                "=   T       = %d\n"
	                "=   source  = 0x%08x\n"
	                "=   *abort  = %d\n"
	                "=============================\n",
	                address, data, size, T, source,
	                (abort!=NULL) ? (*abort) : 0);
	
	return FALSE;
}


boolean mem_r_handler (unsigned int address, unsigned int *data, int size,
                       boolean sign, boolean T, int source, boolean* abort)
{
	//fprintf(stderr, "=============================\n"
	//                "= Read Handler\n"
	//                "=   address = 0x%08x\n"
	//                "=   size    = 0x%08x\n"
	//                "=   T       = %d\n"
	//                "=   source  = 0x%08x\n"
	//                "=   *abort  = %d\n"
	//                "=============================\n",
	//                address, size, T, source,
	//                (abort!=NULL) ? (*abort) : 0);
	//
	(*data) = 1337;
	return TRUE;
}
