#include <stdio.h>
#include <pthread.h>

#include "gui.h"

#define TRUE  (0==0)
#define FALSE (0!=0)
typedef int boolean;

void *
my_thread(void *attrs)
{
	gui_start();
	return NULL;
}

pthread_t *thread;

boolean
constructor (unsigned char *me, unsigned char *string)
{
	fprintf(stderr, "=============================\n"
	                "= Hello World!\n"
	                "=   me     = '%s'\n"
	                "=   string = '%s'\n"
	                "=============================\n",
	                me, string);
	
	       pthread_create(thread, NULL, my_thread, NULL);
	
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
	fprintf(stderr, "=============================\n"
	                "= Read Handler\n"
	                "=   address = 0x%08x\n"
	                "=   size    = 0x%08x\n"
	                "=   T       = %d\n"
	                "=   source  = 0x%08x\n"
	                "=   *abort  = %d\n"
	                "=============================\n",
	                address, size, T, source,
	                (abort!=NULL) ? (*abort) : 0);
	
	(*data) = 1337;
	return TRUE;
}
