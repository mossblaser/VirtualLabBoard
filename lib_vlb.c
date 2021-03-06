#include "lib_vlb.h"


/******************************************************************************
 * SHM Management Code                                                        *
 ******************************************************************************/

shm_t shm;

static boolean
create_shm(void)
{
	// Initialise the struct
	shm.mem = NULL;
	shm.connected = FALSE;
	
	// Create the shared memory area
	shm.id = shmget(VLB_SHM_ID, VLB_SHM_SIZE, IPC_CREAT | VLB_SHM_FLG);
	if (shm.id == -1)
		return FALSE;
	
	// Attatch the shared memory to a pointer
	shm.mem = shmat(shm.id, NULL, 0);
	
	// This peutrid cast is specified by SHM
	if (shm.mem == (void *)-1)
		return FALSE;
	
	shm.connected = TRUE;
	return TRUE;
}


static boolean
free_shm(void)
{
	// Detatch the shared memory if attatched
	if (shm.connected == TRUE)
		return shmdt(shm.mem) == 0;
	else
		return TRUE;
}


static void
init_shm(void)
{
	if (shm.connected) {
		int i;
		for (i = 0; i < VLB_MEMORY_SIZE; i++) {
			shm.mem->writeable[i] = TRUE;
			shm.mem->memory[i] = 0x00;
		}
	}
}


/******************************************************************************
 * Jimulator Plugin Code                                                      *
 ******************************************************************************/


boolean
constructor (unsigned char *me, unsigned char *string)
{
	fprintf(stderr, "Starting Virtual Lab Board...\n");

	if (!create_shm()) {
		fprintf(stderr, "ERROR: vlb: Could not create SHM.\n");
		return FALSE;
	}
	
	init_shm();
	return TRUE;
}


boolean
destructor (unsigned char *me)
{
	fprintf(stderr, "Stopping Virtual Lab Board...\n");
	
	if (!free_shm()) {
		fprintf(stderr, "ERROR: vlb: Could not detatch SHM.\n");
		return FALSE;
	}
	
	return TRUE;
}


boolean
mem_w_handler(unsigned int address, unsigned int data, int size,
              boolean T, int source, boolean* abort)
{
	if (!shm.connected)
		return FALSE;
	
	
	// Is the memory address within the range handled by VLB?
	if (address < VLB_MEMORY_START
	    || address >= VLB_MEMORY_START + VLB_MEMORY_SIZE)
		return FALSE;
	
	// Copy the value written into the SHM
	int byte;
	for (byte = 0; byte < size; byte++) {
		// The address of the byte in the SHM memory
		unsigned int shm_address = (address - VLB_MEMORY_START) + byte;
		
		
		if (!shm.mem->writeable[shm_address])
			// TODO: Something more useful/realistic -- catch fire?
			fprintf(stderr, "WARNING: vlb: Write to unwriteable memory!\n");
		
		fprintf(stderr, "INFO: vlb: Write 0x%02x to %d\n",
		                (data >> (8*byte)) & 0xFF,
		                shm_address);
		shm.mem->memory[shm_address] = (data>>(8*byte)) & 0xFF;
	}
	
	return TRUE;
}


boolean
mem_r_handler(unsigned int address, unsigned int *data, int size,
              boolean sign, boolean T, int source, boolean* abort)
{
	if (!shm.connected)
		return FALSE;
	
	// Is the memory address within the range handled by VLB?
	if (address < VLB_MEMORY_START
	    || address >= VLB_MEMORY_START + VLB_MEMORY_SIZE)
		return FALSE;
	
	(*data) = 0;
	
	// Copy the value from the SHM
	int byte;
	for (byte = 0; byte < size; byte++) {
		// The address of the byte in the SHM memory
		unsigned int shm_address = (address - VLB_MEMORY_START) + byte;
		
		fprintf(stderr, "INFO: vlb: Read 0x%02x from %d\n",
		                shm.mem->memory[shm_address],
		                shm_address);
		(*data) |= (shm.mem->memory[shm_address]) << (8*byte);
	}
	
	return TRUE;
}
