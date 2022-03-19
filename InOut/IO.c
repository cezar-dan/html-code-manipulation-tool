/* MANDRUTA Cezar-Dan - 314CB */

/*--- IO.c --- base operations for the I/O system ---*/
#include "IO.h"

/* -------------------------I/O functions------------------------- */

void add(TTree tree, FILE* f_out) {
  strtok(NULL, "=");
  char* id = strtok(NULL, " ");
  strtok(NULL, "\"");
  char* tag = strtok(NULL, "\"");
  TTree node = findId(tree->parent, id);
  if (!node) {
    fprintf(f_out, "Add tag failed: node with id %s not found!\n", id);
    return;
  }
  int current = PARSE_CONTENTS;
  char* temp = calloc(MAX_STRING, sizeof(char));
  if (!temp) {
    destroyArb(&tree->parent);
    exit(ERR_ALLOC);
  }
  for (int i = 0; i < strlen(tag); ++i) {
    current = buildTree(current, tag[i], &node, temp);
    if (!current) {
      free(temp);
      destroyArb(&tree->parent);
      exit(ERR_ALLOC);
    }
  }
  free(temp);
}

void delete_rec(TTree tree, FILE* f_out) {
  char* selector = strtok(NULL, "\n");
  selector = strchr(selector, '\"');
  selector++;
  selector[strlen(selector) - 1] = '\0';

  TGL l = applySelector(tree, selector);
  if (!l) {
    fprintf(f_out, "Delete recursively failed: no node");
    fprintf(f_out, " found for selector %s!\n", selector);
  }
  TGL lptr = l;
  while (lptr) {
    if (!deleteRec(tree, lptr->info)) {
      destroyArb(&tree->parent);
      exit(ERR_ALLOC);
    }
    lptr = lptr->next;
  }
  destroyGl(&l, NULL);
}

void overrideStyle(TTree tree, FILE* f_out) {
  char *style, *selector;
  getSelectorAndStyle(&selector, &style);
  if (!removeAllChar(style, ' ')) {
    destroyArb(&tree->parent);
    exit(ERR_ALLOC);
  }

  TGL l = applySelector(tree, selector);
  if (!l) {
    fprintf(f_out, "Override style failed: no node");
    fprintf(f_out, " found for selector %s!\n", selector);
  }

  TGL lptr = l;
  while (lptr) {
    overrideNodeStyle(tree, lptr->info, style);
    lptr = lptr->next;
  }
  destroyGl(&l, NULL);
}

void appendStyle(TTree tree, FILE* f_out) {
  char *style, *selector;
  getSelectorAndStyle(&selector, &style);
  if (!removeAllChar(style, ' ')) {
    destroyArb(&tree->parent);
    exit(ERR_ALLOC);
  }

  TGL l = applySelector(tree, selector);
  if (!l) {
    fprintf(f_out, "Append to style failed: no node");
    fprintf(f_out, " found for selector %s!\n", selector);
  }

  TGL lptr = l;
  while (lptr) {
    if (!appendToStyle(tree, lptr->info, style)) {
      destroyArb(&tree->parent);
      exit(ERR_ALLOC);
    }
    lptr = lptr->next;
  }
  destroyGl(&l, NULL);
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage:\n\t./tema1 <input.html> <commands.in> <commands.out>\n");
    return 0;
  }

  FILE* f_html = fopen(argv[1], "rt");
  if (!f_html) {
    fprintf(stderr, "Failed to open %s file\n", argv[1]);
    return 0;
  }
  FILE* f_in = fopen(argv[2], "rt");
  if (!f_in) {
    fprintf(stderr, "Failed to open %s file\n", argv[2]);
    return 0;
  }

  FILE* f_out = fopen(argv[3], "wt");
  if (!f_out) {
    fprintf(stderr, "Failed to open %s file\n", argv[3]);
    return 0;
  }

  char c;
  int current = PARSE_CONTENTS;
  TTree arb = allocNode();
  TTree nod = arb;
  char* temp = calloc(MAX_STRING, sizeof(char));
  if (!temp) {
    exit(ERR_ALLOC);
  }

  while (fscanf(f_html, "%c", &c) == 1) {
    current = buildTree(current, c, &nod, temp);
    if (!current) {
      destroyArb(&arb);
      free(temp);
      fclose(f_html);
      exit(ERR_ALLOC);
    }
  }
  free(temp);
  fclose(f_html);

  int n;
  fscanf(f_in, "%d", &n);
  while (1) {
    char* s = malloc(sizeof(char) * MAX_CMD_LEN);
    if (!s) {
      exit(ERR_ALLOC);
    }
    if (!fgets(s, MAX_CMD_LEN, f_in)) {
      destroyArb(&arb);
      free(s);
      fclose(f_in);
      fclose(f_out);
      break;
    }
    strtok(s, "\n"); // replaces fgets newline(\n) with \0
    char* pch = strtok(s, " ");
    if (strcmp(pch, "format") == 0) {
      formatTree(arb->firstChild, 0, f_out);
    } else if (strcmp(pch, "add") == 0) {
      add(arb->firstChild, f_out);
    } else if (strcmp(pch, "deleteRecursively") == 0) {
      delete_rec(arb->firstChild, f_out);
    } else if (strcmp(pch, "overrideStyle") == 0) {
      overrideStyle(arb->firstChild, f_out);
    } else if (strcmp(pch, "appendStyle") == 0) {
      appendStyle(arb->firstChild, f_out);
    }
    free(s);
  }
  return 0;
}
