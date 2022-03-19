/* MANDRUTA Cezar-Dan - 314CB */

/*--- GLfunctions.c --- base operations for generic singly linked lists ---*/
#include "TGL.h"

/* -------------------------Generic list functions------------------------- */

TGL allocCellGl(void* e) {
  TGL aux = (TGL)malloc(sizeof(TGCell));  /* (1) tries alloc */
  if (aux)                         /* aux holds the address of the new cell */
  { aux->info = e;                 /* (2,3) completes the new cell */
    aux->next = NULL;
  }
  return aux;          /* the result is the address of the new cell or NULL */
}

int insGl(TGL* aL, void* ae) {
  TGL aux = malloc(sizeof(TGCell));
  if (!aux) {
    return 0;
  }
  aux->info = ae;
  aux->next = *aL;
  *aL = aux;

  return 1;
}

int insGlPriority(TGL* aL, void* ae, compf priority) {
  TGL aux = malloc(sizeof(TGCell));
  if (!aux) {
    return 0;
  }
  aux->info = ae;
  if (*aL == NULL) {   /* list is empty */
    aux->next = *aL;
    *aL = aux;
    return 1;
  } else {    /* list is not empty */
    TGL cur = *aL, next = (*aL)->next;
    if (priority(ae, cur->info) >= 0) {   /* insert at beginning of list */
      aux->next = *aL;
      *aL = aux;
      return 1;
    }
    while (next) {
      if (priority(ae, cur->info) < 0 && priority(ae, next->info) >= 0) {
        aux->next = next;
        cur->next = aux;
        return 1;
      }
      cur = next;
      next = next->next;
    }
    /* insert at end of list */
    cur->next = aux;
    aux->next = NULL;
    return 1;
  }
  return 0;
}

void destroyGl(TGL* aL, TF free_elem) {
  while (*aL != NULL) {
    TGL aux = *aL;     /* address of eliminated cell */
    if (!aux)
      return;
    if (free_elem) {
      free_elem(aux->info);  /* frees the space occupied by an element */
    }
    *aL = aux->next;    /* disconnects the cell from the list */
    free(aux);   /* frees the space occupied by the cell */
  }
}

void printGl(TGL* aL, TF afiEl) {
  for (; *aL; aL = &(*aL)->next) {
    afiEl((*aL)->info);
  }
}

int fPrintGl(TGL* aL, frTF afiEl, void* f) {
  for (; *aL; aL = &(*aL)->next) {
    if (!afiEl((*aL)->info, f)) {
      return 0;
    }
  }
  return 1;
}

int insAfterGl(TGL* cell, void* ae) {
  TGL aux = allocCellGl(ae);
  if (!aux) {
    return 0;
  }
    aux->next = (*cell)->next;
    (*cell)->next = aux;
  return 1;
}

void reverseGl(TGL* aL) {
  TGL p = *aL;
  TGL prev = NULL, next = NULL;

  while (p != NULL) {
      next = p->next;
      p->next = prev;
      prev = p;
      p = next;
  }
  *aL = prev;
}

void removeGl(TGL* aL, TF free_elem) {
  if (*aL) {
    TGL aux = *aL;
    (*aL) = (*aL)->next;
    free_elem(aux->info);
    free(aux);
  }
}

int numEgl(TGL L) {
  int count = 0;
  while (L) {
    ++count;
    L = L->next;
  }
  return count;
}

void printRevGl(TGL* aL, TF afiEL) {
  if (*aL) {
    printRevGl(&(*aL)->next, afiEL);
    afiEL((*aL)->info);
  }
}

void fPrintRevGL(TGL* aL, fTF afiEL, FILE* f) {
  if (*aL) {
    fPrintRevGL(&(*aL)->next, afiEL, f);
    afiEL((*aL)->info, f);
  }
}
