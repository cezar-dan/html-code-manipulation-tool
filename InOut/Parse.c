/* MANDRUTA Cezar-Dan - 314CB */

/*--- Parse.c --- text parsing functions ---*/
#include "../HtmlTree/TTree.h"

/* -----------------------------Parse functions----------------------------- */

void appendChar(char c, char* str) {
  char cToStr[1] = {c};
  strncat(str, cToStr, 1);
}

char* trimBack(char* string) {
  char* trimmed = string;
  while (*trimmed == ' ' || *trimmed == '\n') {
    ++trimmed;
  }
  return trimmed;
}

void trimFront(char* string) {
  char* pchr = NULL;
  if (string) {
    pchr = string + strlen(string) - 1;
  }
  if (!pchr) {
    return;
  }
  while (*pchr == ' ' || *pchr == '\n') {
    *pchr = '\0';
    --pchr;
  }
}

int removeAllChar(char* string, char c) {
  char* copy = calloc(strlen(string) + 1, sizeof(char));
  if (!copy) {
    return 0;
  }
  char* ptr = string;
  while (*ptr != '\0') {
    if (*ptr != c) {
      appendChar(*ptr, copy);
    }
    ++ptr;
  }
  strcpy(string, copy);
  free(copy);
  return 1;
}

int nrDigits(int nr) {
  int nr_digits = 0;
  while (nr) {
    nr_digits++;
    nr /= 10;
  }
  return nr_digits;
}

void addTab(int indent, FILE* f_out) {
  for (int i = 0; i < indent; ++i) {
    fprintf(f_out, "\t");
  }
}

void getSelectorAndStyle(char** selector, char** style) {
  char* text = strtok(NULL, "\0");
  *selector = strstr(text, "selector=");
  *style = strstr(text, "style=");
  *selector = strchr(*selector, '\"');
  char* ptr = *selector + 1;
  ptr = strchr(ptr, '\"');
  *ptr = '\0';
  (*selector)++;

  *style = strchr(*style, '\"');
  (*style)++;
  (*style)[strlen(*style) - 1] = '\0';
}

TGL buildStyleList(char* style) {
  char* copy = calloc(strlen(style) + 1, sizeof(char));
  if (!copy) {
    exit(ERR_ALLOC);
  }
  strncpy(copy, style, strlen(style));
  char* name = strtok(copy, ":");
  char* value = strtok(NULL, ";");;
  TGL l = NULL;
  while (value) {
    TAttr atr = malloc(sizeof(TNodeAttr));
    if (!atr) {
      free(copy);
      destroyGl(&l, free_attr);
      exit(ERR_ALLOC);
    }
    atr->name = calloc(strlen(name) + 1, sizeof(char));
    if (!atr->name) {
      free(copy);
      free(atr);
      destroyGl(&l, free_attr);
      exit(ERR_ALLOC);
    }
    atr->value = calloc(strlen(value) + 1, sizeof(char));
    if (!atr->value) {
      free(copy);
      free(atr->name);
      free(atr);
      destroyGl(&l, free_attr);
      exit(ERR_ALLOC);
    }
    strncpy(atr->name, name, strlen(name));
    strncpy(atr->value, value, strlen(value));
    insGl(&l, atr);
    name = strtok(NULL, ":");
    value = strtok(NULL, ";");
  }
  reverseGl(&l);
  free(copy);
  return l;
}
