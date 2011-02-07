#ifndef LIB_VLB_H
#define LIB_VLB_H

#include "vlb.h"


/* Functions called by Jimulator */
boolean constructor(unsigned char *me, unsigned char *string);
boolean destructor(unsigned char *me);

boolean mem_w_handler(unsigned int address, unsigned int data, int size,
                      boolean T, int source, boolean* abort);
boolean mem_r_handler(unsigned int address, unsigned int *data, int size,
                      boolean sign, boolean T, int source, boolean* abort);

#endif
