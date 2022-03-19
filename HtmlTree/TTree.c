/* MANDRUTA Cezar-Dan - 314CB */

/*--- TTree.c --- base operations for html tree ---*/
#include "TTree.h"

/* ------------------------General tree functions------------------------ */

TTree allocNode() {
  TTree a = malloc(sizeof(TTreeNode));
  if (!a) {
    return NULL;
  }
  a->info = malloc(sizeof(TNodeInfo));
  if (!a->info) {
    free(a);
    return NULL;
  }

  a->info->type = NULL;
  a->info->id = NULL;
  a->info->style = NULL;
  a->info->otherAttributes = NULL;
  a->info->isSelfClosing = 0;
  a->info->contents = NULL;
  a->parent = NULL;
  a->nextSibling = NULL;
  a->firstChild = NULL;
  return a;
}

int findTree(TTree node, compf f, TGL* l, void* sel) {
  if (node) {
    int found = f(node, sel);
    if (found > 0) {
      if (!insGl(l, node)) {
        return 0;
      }
    } else if (found < 0) {
      return 0;
    }
    if (!findTree(node->firstChild, f, l, sel)) {
      return 0;
    }
    if (!findTree(node->nextSibling, f, l, sel)) {
      return 0;
    }
  }
  return 1;
}

int sprint_style(void* style, void* str) {
  TAttr s = (TAttr)style;
  if (s) {
    char* temp = calloc(MAX_STRING, sizeof(char));
    if (!temp) {
      return 0;
    }
    sprintf(temp, "%s: %s; ", s->name, s->value);
    strcat(str, temp);
    free(temp);
  }
  return 1;
}

int print_attribute(void* attribute, void* file_ptr) {
  TAttr atr = (TAttr)attribute;
  char* name = atr->name;
  char* value = atr->value;
  fprintf(file_ptr, " %s=\"%s\"", name, value);
  return 1;
}

int formatTree(TTree a, int indent, FILE* f_out) {
  if (a) {
    if (a->info->type) {
      addTab(indent, f_out);
      fprintf(f_out, "<%s", a->info->type);
      if (a->info->style) {
        fprintf(f_out, " style=\"");
        char* temp = calloc(MAX_STRING, sizeof(char));
        if (!temp) {
          return 0;
        }
        if (!fPrintGl(&a->info->style, sprint_style, temp)) {
          return 0;
        }
        temp[strlen(temp) - 1] = '\"';
        fprintf(f_out, "%s", temp);
        free(temp);
      }
      if (a->info->otherAttributes) {
        fPrintGl(&a->info->otherAttributes, print_attribute, f_out);
      }
      if (!a->info->isSelfClosing) {
        fprintf(f_out, ">\n");
      }
      if (a->info->contents) {
        addTab(indent + 1, f_out);
        fprintf(f_out, "%s\n", a->info->contents);
      }
      if (!formatTree(a->firstChild, indent + 1, f_out)) {
        return 0;
      }
      if (!a->info->isSelfClosing) {
        addTab(indent, f_out);
        fprintf(f_out, "</%s>\n", a->info->type);
      } else {
        fprintf(f_out, "/>\n");
      }
      if (!formatTree(a->nextSibling, indent, f_out)) {
        return 0;
      }
    }
  }
  return 1;
}

void free_attr(void* atr) {
  TAttr a = (TAttr)atr;
  free(a->name);
  free(a->value);
  free(a);
}

void destroyInfo(TInfo* i) {
  if (*i) {
    free((*i)->type);
    free((*i)->id);
    if ((*i)->style) {
      destroyGl(&(*i)->style, free_attr);
    }
    if ((*i)->otherAttributes) {
      destroyGl(&(*i)->otherAttributes, free_attr);
    }
    if ((*i)->contents) {
      free((*i)->contents);
    }
    free(*i);
    *i = NULL;
  }
}

void destroyArb(TTree* a) {
  if (*a) {
    destroyArb(&(*a)->firstChild);
    destroyArb(&(*a)->nextSibling);
    destroyInfo(&(*a)->info);
    free(*a);
    (*a) = NULL;
  }
}

int deleteRec(TTree a, TTree n) {
  TTree prev = NULL, cur = n->parent->firstChild;
  while (cur) {
    if (cur == n) break;
    prev = cur;
    cur = cur->nextSibling;
  }
  if (prev == NULL) {
    destroyArb(&cur->firstChild);
    destroyInfo(&cur->info);
    cur->parent->firstChild = cur->nextSibling;
    if (!rewriteIdRec(cur->nextSibling, prev)) {
      return 0;
    }
    free(cur);
  } else if (cur->nextSibling == NULL) {
    destroyArb(&cur->firstChild);
    destroyInfo(&cur->info);
    prev->nextSibling = NULL;
    free(cur);
  } else {
    destroyArb(&cur->firstChild);
    destroyInfo(&cur->info);
    prev->nextSibling = cur->nextSibling;
    if (!rewriteIdRec(cur->nextSibling, prev)) {
      return 0;
    }
    free(cur);
  }
  return 1;
}

/* --------------------------Tree ID functions-------------------------- */

/* FUNCTION USES STRTOK AND IT MESSES UP LAST STRTOK POINTER. BEWARE */
TTree findId(TTree a, char* id) {
  TTree root = a;
  a = a->firstChild->firstChild;
  TTree nod = NULL;
  int len = strlen(id);
  char* copy = calloc(len + 1, sizeof(char));
  if (!copy) {
    destroyArb(&root);
    exit(ERR_ALLOC);
  }
  char* partial = calloc(len + 1, sizeof(char));
  if (!partial) {
    free(copy);
    destroyArb(&root);
    exit(ERR_ALLOC);
  }
  strncpy(copy, id, len);
  char* pchr = strtok(copy, ".\0");
  strcat(partial, pchr);
  while (a != NULL) {
    if (strcmp(partial, a->info->id) == 0) {
      break;
    }
    a = a->nextSibling;
  }
  if (!a) {
    free(partial);
    free(copy);
    return NULL;
  }
  while (1) {
    pchr = strtok(NULL, ".\0");
    if (!pchr) {
      nod = a;
      break;
    }
    a = a->firstChild;
    appendChar('.', partial);
    strcat(partial, pchr);
    while (a != NULL) {
      if (strcmp(partial, a->info->id) == 0) {
        break;
      }
      a = a->nextSibling;
    }
    if (!a) {
      free(partial);
      free(copy);
      return NULL;
    }
  }
  free(partial);
  free(copy);
  return nod;
}

char* getNewIdPrev(char* id) {
  int len = strlen(id);
  char* aux = calloc((len + 1), sizeof(char));
  if (!aux) {
    return NULL;
  }
  strncpy(aux, id, len + 1);
  char* last_nr = NULL;
  char* next = strtok(aux, ".");
  while (next != NULL) {
    last_nr = next;
    next = strtok(NULL, ".");
  }
  int nr = atoi(last_nr);
  int new_nr = nr + 1;
  int old_nr_digits = nrDigits(nr);
  int new_nr_digits = nrDigits(new_nr);
  int surplus_digit = new_nr_digits - old_nr_digits;

  char* new_id = calloc((len + 1 + surplus_digit), sizeof(char));
  if (!new_id) {
    return NULL;
  }
  strncpy(new_id, id, len);
  /* len - old_nr_digits --> we write over the dot of the last id */
  snprintf(new_id + len - old_nr_digits, new_nr_digits + 1, "%d", new_nr);

  free(aux);
  return new_id;
}

int addId(TTree cur, TTree prev) {
  if (cur->parent->info->id == NULL) {
    cur->info->id = calloc(2, sizeof(char));
    if (!cur->info->id) {
      return 0;
    }
    cur->info->id = strncpy(cur->info->id, "0", 1);
  } else {
    if (strcmp(cur->parent->info->id, "0") == 0) {
      cur->info->id = calloc(2, sizeof(char));
      if (!cur->info->id) {
        return 0;
      }
      if (prev == NULL) {
        strncpy(cur->info->id, "1", 1);
      } else {
        strncpy(cur->info->id, "2", 1);
      }
    } else {
      if (prev) {
        cur->info->id = getNewIdPrev(prev->info->id);
        if (!cur->info->id) {
          return 0;
        }
      } else {
        int oldIdlen = strlen(cur->parent->info->id);
        cur->info->id = calloc(oldIdlen + 3, sizeof(char)); /* 3 <--> ".1\0" */
        if (!cur->info->id) {
          return 0;
        }
        strncpy(cur->info->id, cur->parent->info->id, oldIdlen);
        strncat(cur->info->id, ".1", 2);
      }
    }
  }
  return 1;
}

int rewriteIdRec(TTree cur, TTree prev) {
  if (cur) {
    free(cur->info->id);
    if (!addId(cur, prev)) {
      return 0;
    }
    if (!rewriteIdRec(cur->firstChild, NULL)) {
      return 0;
    }
    if (!rewriteIdRec(cur->nextSibling, cur)) {
      return 0;
    }
  }
  return 1;
}

/* ------------------------Tree building functions------------------------ */

int addNode(char* tagname, TTree* parent_node) {
  if (!(*parent_node)->firstChild) {
    (*parent_node)->firstChild = allocNode();
    if (!(*parent_node)->firstChild) {
      return 0;
    }
    (*parent_node)->firstChild->parent = (*parent_node);
    (*parent_node) = (*parent_node)->firstChild;
    if (!addId(*parent_node, NULL)) {
      return 0;
    }
  } else {
    TTree parent = *parent_node;
    *parent_node = (*parent_node)->firstChild;
    while ((*parent_node)->nextSibling) {
      *parent_node = (*parent_node)->nextSibling;
    }
    (*parent_node)->nextSibling = allocNode();
    if (!(*parent_node)->nextSibling) {
      return 0;
    }
    TTree prev = *parent_node;
    *parent_node = (*parent_node)->nextSibling;
    (*parent_node)->parent = parent;
    if (!addId(*parent_node, prev)) {
      return 0;
    }
  }
  int len = strlen(tagname);
  (*parent_node)->info->type = calloc(len + 1, sizeof(char));
  if (!(*parent_node)->info->type) {
    return 0;
  }
  strncpy((*parent_node)->info->type, tagname, len);
  memset(tagname, '\0', MAX_STRING * sizeof(char));
  return 1;
}

int addStyle(char* styles, TTree node) {
  TGL cell = node->info->style;
  cell->info = malloc(sizeof(TNodeAttr));
  if (!cell->info) {
    return 0;
  }
  TAttr atr = cell->info;
  atr->name = NULL;
  atr->value = NULL;
  char* pchr = strtok(styles, ":");
  int len = strlen(pchr);
  atr->name = calloc(len + 1, sizeof(char));
  if (!atr->name) {
    return 0;
  }
  strncpy(atr->name, pchr, len);
  pchr = strtok(NULL, ";\0");
  removeAllChar(pchr, ' ');
  len = strlen(pchr);
  atr->value = calloc(len + 1, sizeof(char));
  if (!atr->value) {
      return 0;
  }
  strncpy(atr->value, pchr, len);
  while (1) {
    pchr = strtok(NULL, ":");
    if (!pchr) {
      return 1;
    }
    removeAllChar(pchr, ' ');
    if (!insGl(&node->info->style, NULL)) {
      return 0;
    }
    cell = node->info->style;
    cell->info = malloc(sizeof(TNodeAttr));
    if (!cell->info) {
      return 0;
    }
    TAttr atr = cell->info;
    atr->name = NULL;
    atr->value = NULL;
    len = strlen(pchr);
    atr->name = calloc(len + 1, sizeof(char));
    if (!atr->name) {
      return 0;
    }
    strncpy(atr->name, pchr, len);
    pchr = strtok(NULL, ";\0");
    removeAllChar(pchr, ' ');
    len = strlen(pchr);
    atr->value = calloc(len + 1, sizeof(char));
    if (!atr->value) {
      return 0;
    }
    strncpy(atr->value, pchr, len);
  }
  memset(styles, '\0', MAX_STRING * sizeof(char));
  return 1;
}

int addAttributeName(char* attr_name, TTree node) {
  if (strcmp(attr_name, "style") == 0) {
    if (!insGl(&node->info->style, NULL)) {
      return 0;
    }
    memset(attr_name, '\0', MAX_STRING * sizeof(char));
    return 1;
  } else {
    TAttr atr = malloc(sizeof(TNodeAttr));
      if (!atr) {
        return 0;
      }
    int len = strlen(attr_name);
    atr->name = calloc(len + 1, sizeof(char));
    if (!atr->name) {
      free(atr);
      return 0;
    }
    strncpy(atr->name, attr_name, len);
    atr->value = NULL;
    if (!insGl(&node->info->otherAttributes, atr)) {
      return 0;
    }
    memset(attr_name, '\0', MAX_STRING * sizeof(char));
    }
    return 1;
}

int addAttributeValue(char* attr_value, TTree node) {
  if (node->info->otherAttributes) {
    TAttr atr = node->info->otherAttributes->info;
    if (atr->value == NULL) {
      int len = strlen(attr_value);
      atr->value = calloc(len + 1, sizeof(char));
      if (!atr->value) {
        free(atr);
        return 0;
      }
      strncpy(atr->value, attr_value, len);
    } else {
      if (!addStyle(attr_value, node)) {
        return 0;
      }
    }
  } else {
    if (!addStyle(attr_value, node)) {
      return 0;
    }
  }
  memset(attr_value, '\0', MAX_STRING * sizeof(char));
  return 1;
}

int addContent(char* content, TTree node) {
  char* pchr = content;
  pchr = trimBack(pchr);
  if (*pchr == '\0') {
    memset(content, '\0', MAX_STRING * sizeof(char));
    return 1;
  } else {
    int len = strlen(pchr);
    trimFront(pchr);
    node->info->contents = calloc(len + 1, sizeof(char));
    if (!node->info->contents) {
      return 0;
    }
    strncpy(node->info->contents, pchr, len);
  }
  memset(content, '\0', MAX_STRING * sizeof(char));
  return 1;
}

TParseState buildTree(TParseState curState, char c, TTree* n, char* temp) {
  TParseState nextState = PARSE_ERROR;

  switch (curState) {
  case PARSE_CONTENTS:
    if (c == '<')           {   nextState = PARSE_OPENING_BRACKET;
      addContent(temp, *n);            }
    else                    {   nextState = PARSE_CONTENTS;
      appendChar(c, temp);                }
    break;
  case PARSE_OPENING_BRACKET:
    if (isspace(c))         {   nextState = PARSE_OPENING_BRACKET;  }
    else if (c == '>')      {   nextState = PARSE_ERROR;            }
    else if (c == '/')      {   nextState = PARSE_CLOSING_TAG;
      reverseGl(&(*n)->info->style);
      reverseGl(&(*n)->info->otherAttributes);
                                (*n) = (*n)->parent ;         }
    else                    {   nextState = PARSE_TAG_TYPE;
      appendChar(c, temp);                }
    break;
  case PARSE_TAG_TYPE:
    if (isspace(c))         {   nextState = PARSE_REST_OF_TAG;
                                if (!addNode(temp, n)) return 0; }
    else if (c == '>')      {   nextState = PARSE_CONTENTS;
                                if (!addNode(temp, n)) return 0; }
    else                    {   nextState = PARSE_TAG_TYPE;
      appendChar(c, temp);                }
    break;
  case PARSE_CLOSING_TAG:
    if (c == '>')           {   nextState = PARSE_CONTENTS;         }
    else                    {   nextState = PARSE_CLOSING_TAG;      }
    break;
  case PARSE_REST_OF_TAG:
    if (isspace(c))         {   nextState = PARSE_REST_OF_TAG;      }
    else if (c == '>')      {   nextState = PARSE_CONTENTS;         }
    else if (c == '/')      {   nextState = PARSE_SELF_CLOSING;
                                (*n)->info->isSelfClosing = 1;
      reverseGl(&(*n)->info->style);
      reverseGl(&(*n)->info->otherAttributes);
                                (*n) = (*n)->parent ;         }
    else                    {   nextState = PARSE_ATTRIBUTE_NAME;
      appendChar(c, temp);                }
    break;
  case PARSE_ATTRIBUTE_NAME:
    if (c == '=')           {   nextState = PARSE_ATTRIBUTE_EQ;
                                if (!addAttributeName(temp, *n)) return 0;  }
    else                    {   nextState = PARSE_ATTRIBUTE_NAME;
      appendChar(c, temp);                }
    break;
  case PARSE_ATTRIBUTE_EQ:
    if (c == '\"')          {   nextState = PARSE_ATTRIBUTE_VALUE;  }
    break;
  case PARSE_ATTRIBUTE_VALUE:
    if (c == '\"')          {   nextState = PARSE_REST_OF_TAG;
                                if (!addAttributeValue(temp, *n)) return 0; }
    else                    {   nextState = PARSE_ATTRIBUTE_VALUE;
      appendChar(c, temp);                }
    break;
  case PARSE_SELF_CLOSING:
    if (c == '>')           {   nextState = PARSE_CONTENTS;         }
    else                    {   nextState = PARSE_ERROR;            }
    break;
  default:
    break;
  }
  return nextState;
}

void overrideNodeStyle(TTree a, TTree n, char* style) {
  TGL l = buildStyleList(style);
  destroyGl(&n->info->style, free_attr);
  n->info->style = l;
}

int appendToStyle(TTree a, TTree n, char* style) {
  TGL lnew = buildStyleList(style);
  TGL pnew = lnew;
  TGL prev = NULL;
  reverseGl(&n->info->style);
  TGL pold = n->info->style;
  while (pnew) {
    pold = n->info->style;
    char* new_name = ((TAttr)(pnew->info))->name;
    char* new_val = ((TAttr)(pnew->info))->value;
    while (pold) {
      char* old_name = ((TAttr)(pold->info))->name;
      char* old_val = ((TAttr)(pold->info))->value;
      if (strcmp(old_name, new_name) == 0) {
        old_val = calloc(strlen(new_val) + 1, sizeof(char));
        if (!old_val) {
          destroyGl(&lnew, free_attr);
          return 0;
        }
        strncpy(old_val, new_val, strlen(new_val) + 1);
        free(((TAttr)(pold->info))->value);
        ((TAttr)(pold->info))->value = old_val;
        break;
      }
      pold = pold->next;
    }
    if (!pold) {
      if (!prev) {
        lnew = lnew->next;
        pnew->next = n->info->style;
        n->info->style = pnew;
        pnew = lnew;
      } else {
        prev->next = pnew->next;
        pnew->next = n->info->style;
        n->info->style = pnew;
        pnew = prev->next;
      }
    } else {
      prev = pnew;
      pnew = pnew->next;
    }
  }
  reverseGl(&n->info->style);
  destroyGl(&lnew, free_attr);
  return 1;
}
