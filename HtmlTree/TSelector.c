/* MANDRUTA Cezar-Dan - 314CB */

/*--- TSelector.c --- base operations for HTML selectors ---*/
#include"TSelector.h"

/* ---------------------------Selector functions--------------------------- */

TSelector interpretSelector(char* selector) {
  TSelector sel;
  if (selector[0] == '#') {
    sel = ID;
  } else if (selector[0] == '.') {
    sel = CLASS;
  } else if (strchr(selector, '.')) {
    sel = ELEMENT_CLASS;
  } else if (strchr(selector, '>')) {
    sel = EL1_GR_EL2;
  } else if (strchr(selector, ' ')) {
    sel = EL1_EL2;
  } else {
    sel = ELEMENT;
  }
  return sel;
}

TGL applySelector(TTree a, char* selector) {
  compf fc[6] = {NULL, classCheck, typeCheck, typeClassCheck,
                 typeParentCheck, typeAncestorCheck };
  TGL l = NULL;
  TSelector sel = interpretSelector(selector);
  if (sel == ID) {
    TTree node = findId(a->parent, selector + 1);
    if (node) {
      if (!insGl(&l, node)) {
        destroyArb(&a);
        exit(ERR_ALLOC);
      }
    }
  } else {
    if (!findTree(a, fc[sel], &l, selector)) {
      destroyArb(&a);
      exit(ERR_ALLOC);
    }
  }
  return l;
}

int typeCheck(void* node, void* sel) {
  TTree n = (TTree)node;
  char* str = (char*)sel;
  if (strcmp(str, n->info->type) == 0) {
    return 1;
  }
  return 0;
}

int classCheck(void* node, void* sel) {
  TTree n = (TTree)node;
  char* str = (char*)sel;
  if (str[0] == '.') {
    str++;
  }
  TGL l = n->info->otherAttributes;
  while (l) {
    if (strcmp(str, ((TAttr)(l->info))->value) == 0) {
      return 1;
    }
    l = l->next;
  }
  return 0;
}

int typeClassCheck(void* node, void* sel) {
  char* pct = strchr(sel, '.');
  char* type = calloc(pct - (char*)sel + 1, sizeof(char));
  if (!type) {
    return -1;
  }
  strncpy(type, sel, pct - (char*)sel);

  int ok = typeCheck(node, type);
  free(type);
  return ok && classCheck(node, pct + 1);
}

int typeParentCheck(void* node, void* sel) {
  char* grt = strchr(sel, '>');
  char* type = calloc(grt - (char*)sel + 1, sizeof(char));
  if (!type) {
    return -1;
  }
  strncpy(type, sel, grt - (char*)sel);

  int ok = 0;
  if (((TTree)node)->parent->info->type) {
    if (strcmp(type, ((TTree)node)->parent->info->type) == 0) {
      ok = 1;
    }
  }
  free(type);
  return ok && typeCheck(node, grt + 1);
}

int typeAncestorCheck(void* node, void* sel) {
  char* spc = strchr(sel, ' ');
  char* type = calloc(spc - (char*)sel + 1, sizeof(char));
  if (!type) {
    return -1;
  }
  strncpy(type, sel, spc - (char*)sel);

  int ok = 0;
  TTree n = node;
  while (n->parent->info->type) {
    if (strcmp(type, n->parent->info->type) == 0) {
      ok = 1;
      break;
    }
    n = n->parent;
  }
  free(type);
  return ok && typeCheck(node, spc + 1);
}
