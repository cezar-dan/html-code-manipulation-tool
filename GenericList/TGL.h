/* MANDRUTA Cezar-Dan - 314CB */

/*--- TGL.h --- GENERIC SINGLY LINKED LIST ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#ifndef _GENERIC_LIST_H_
#define _GENERIC_LIST_H_

#define ERR_ALLOC -2

typedef struct gcell {
  void* info;           /* Address of information */
  struct gcell *next;   /* Address of next cell */
} TGCell, *TGL;         /* Types Cell and List  */

typedef struct {
  TGL list;             /* Beginning of list */
  TGL last;             /* End of list */
} TGLlast;

// Function that compares two elements
typedef int (*compf)(void*, void*);

// Function that prints/frees an element */
typedef void (*TF)(void*);

// Function that prints an element in file/ in buffer
typedef void (*fTF)(void*, void*);

// Function that prints an element in file/ in buffer with return value
typedef int (*frTF)(void*, void*);


/* -------------------------Generic list functions------------------------- */

// Allocates space for a generic cell
TGL allocCellGl(void* e);
/** e           - pointer to the allocated element to be added in ->info field
*** return      - returns a pointer to the allocated cell
**/

// Inserts at beginning of list
int insGl(TGL* aL, void* ae);
/** aL          - address of the pointer to the beginning of the list
*** ae          - pointer to a value that needs to be inserted
*** return      - returns whether the insertion was succesful or not (1-y/ 0-n)
**/

// Inserts in ordered list, maintaining the priority rule
int insGlPriority(TGL* aL, void* ae, compf priority);
/** aL          - address of the pointer to the beginning of the list
*** ae          - pointer to a value that needs to be inserted
*** priority    - pointer to a function that represents the priority rule
*** return      - returns whether the insertion was succesful or not (1-y/ 0-n)
**/

// Destroys the list
void destroyGl(TGL* aL, TF free_elem);
/** aL          - address of the pointer to the beginning of the list
*** free_elem   - function that frees an element / NULL to destroy list cells only
**/

// Prints list
void printGl(TGL* aL, TF afiEl);
/** aL          - address of the pointer to the beginning of the list
*** afiEl       - function that prints an element
**/

// Prints list in file f
int fPrintGl(TGL* aL, frTF afiEl, void* f);
/** aL          - address of the pointer to the beginning of the list
*** afiEl       - function that prints an element
*** f           - pointer to the output file
*** return      - returns whether the print was succesful or not (1-y/ 0-n)
**/

// Inserts after element
int insAfterGl(TGL* cell, void* ae);
/** cell        - address of the pointer that points to the cell we want to
***               insert after
*** ae          - pointer to a value that needs to be inserted
*** return      - returns whether the insertion was succesful or not (1-y/ 0-n)
**/

// Reverses list L
void reverseGl(TGL* aL);
/** aL          - address of the pointer to the beginning of the list
**/

// Removes first element in the list
void removeGl(TGL* aL, TF free_elem);
/** aL          - address of the pointer to the beginning of the list
*** free_elem   - function that frees an element
**/

// Counts the number of elements in the list
int numEgl(TGL L);
/** L          - pointer to the beginning of the list
** returns     - number of elements in list L
**/

// Prints list in reverse order
void printRevGl(TGL* aL, TF afiEL);
/** aL          - address of the pointer to the beginning of the list
*** afiEl       - function that prints an element
**/

// Prints list in reverse order
void fPrintRevGL(TGL* aL, fTF afiEL, FILE* f);
/** aL          - address of the pointer to the beginning of the list
*** afiEl       - function that prints an element
*** f           - pointer to the output file
**/

#endif /* _GENERIC_LIST_H_ */
