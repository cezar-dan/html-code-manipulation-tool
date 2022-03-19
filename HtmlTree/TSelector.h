/* MANDRUTA Cezar-Dan - 314CB */

/*--- TSelector.h --- HTML SELECTORS FUNCTIONS ---*/

#include "TTree.h"

#ifndef _SELECTORS_H_
#define _SELECTORS_H_

typedef enum
{
    ID                = 0,
    CLASS             = 1,
    ELEMENT           = 2,
    ELEMENT_CLASS     = 3,
    EL1_GR_EL2        = 4,
    EL1_EL2           = 5,
} TSelector;

/* ---------------------------Selector functions--------------------------- */

// Function that returns the selector type based on the string
TSelector interpretSelector(char* selector);
/** selector    - selector string
*** return      - returns selector type
**/

// Finds all nodes that satisfy the selector and adds them to a list
TGL applySelector(TTree a, char* selector);
/** a           - pointer to <html> node
*** selector    - selector string
*** return      - returns list of nodes that satisfy the selector / NULL if no
***               elements satisfy the selector
**/

/* These functions return whether the node satisfies the selector or not*/

int typeCheck(void* node, void* sel);

int classCheck(void* node, void* sel);

int typeClassCheck(void* node, void* sel);

int typeParentCheck(void* node, void* sel);

int typeAncestorCheck(void* node, void* sel);

/** node        - node to be checked
*** sel         - selector string
*** return      - returns: -  0  - if selector is not satisfied
***                        -  1  - if selector is satisfied
***                        - -1  - allocation error
**/

#endif /* _SELECTORS_H_ */
