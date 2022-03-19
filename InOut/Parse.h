/* MANDRUTA Cezar-Dan - 314CB */

/*--- Parse.h --- TEXT PARSING FUNCTIONS ---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../GenericList/TGL.h"

#ifndef _PARSE_H_
#define _PARSE_H_

#define MAX_STRING 1000

typedef enum
{
    PARSE_ERROR             = 0,
    PARSE_CONTENTS          = 1,
    PARSE_OPENING_BRACKET   = 2,
    PARSE_TAG_TYPE          = 3,
    PARSE_CLOSING_TAG       = 4,
    PARSE_REST_OF_TAG       = 5,
    PARSE_ATTRIBUTE_NAME    = 6,
    PARSE_ATTRIBUTE_EQ      = 7,
    PARSE_ATTRIBUTE_VALUE   = 8,
    PARSE_SELF_CLOSING      = 9,
} TParseState;

/* -----------------------------Parse functions----------------------------- */

// Function that appends the char c to string str
void appendChar(char c, char* str);
/** c           - character to be appended
*** str         - string
**/

// Function that removes whitespaces and newlines from the beginning of string
char* trimBack(char* string);
/** string      - string to be trimmed
*** return      - pointer to trimmed string
**/

// Function that removes whitespaces and newlines from the end of string
void trimFront(char* string);
/** string      - string to be trimmed
**/

// Function that removes all apparitions of char c in string
int removeAllChar(char* string, char c);
/** string      - pointer to string
*** c           - character to be removed
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Function that counts the number of digits of int nr
int nrDigits(int nr);
/** nr          - number to be checked
*** return      - number of digits of said number
**/

// Function that prints an indent amount of tabs
void addTab(int indent, FILE* f_out);
/** indent      - number of tabs to be printed
*** f_out       - pointer to output file
**/

// Function that gets the selector and style from buffer string
// Used for overrideStyle and appendStyle (see IO.c: 48 -> 86)
void getSelectorAndStyle(char** selector, char** style);
/** selector    - address of selector string to be passed by refference
*** style       - address of style string to be passed by refference
**/

// Function that builds a style list based on a style string
TGL buildStyleList(char* style);
/** style       - style string
*** return      - style list
**/

#endif /* _PARSE_H_ */
