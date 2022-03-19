/* MANDRUTA Cezar-Dan - 314CB */

/*--- IO.h --- I/O SYSTEM ---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../HtmlTree/TTree.h"
#include "../HtmlTree/TSelector.h"

#ifndef _IO_SYSTEM_
#define _IO_STSTEM_

#define MAX_CMD_LEN 260

/* -------------------------I/O functions------------------------- */

/* These functions do the command they are named after */

void add(TTree tree, FILE* f_out);

void delete_rec(TTree tree, FILE* f_out);

void overrideStyle(TTree tree, FILE* f_out);

void appendStyle(TTree tree, FILE* f_out);

/** tree        - pointer to the root <html> node
*** f_out       - pointer to the output file
***/

#endif /* _IO_SYSTEM_ */
