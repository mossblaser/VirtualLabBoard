#include <stdio.h>

#define TRUE  (0==0)
#define FALSE (0!=0)
typedef int boolean;

boolean
constructor (unsigned char *me, unsigned char *string)
{
	fprintf(stderr, "=============================\n"
	                "= Hello World!\n"
	                "=   me     = '%s'\n"
	                "=   string = '%s'\n"
	                "=============================\n",
	                me, string);
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
