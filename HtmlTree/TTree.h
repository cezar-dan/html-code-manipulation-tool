/* MANDRUTA Cezar-Dan - 314CB */

/*--- TTree.h --- HTML TREE DATA STRUCTURE ---*/
#include "../InOut/Parse.h"

#ifndef _TARB_H_
#define _TARB_H_

typedef struct TNodeAttr {
  char *name;
  char *value;
} TNodeAttr, *TAttr;

typedef struct TNodeInfo {
  char *type;
  char *id;
  TGL style;
  TGL otherAttributes;
  int isSelfClosing;
  char *contents;
} TNodeInfo, *TInfo;

typedef struct TTreeNode {
  TInfo info;
  struct TTreeNode *parent;
  struct TTreeNode *nextSibling;
  struct TTreeNode *firstChild;
} TTreeNode, *TTree;

/* ----------------------------Tree functions---------------------------- */

// Allocates html tree node (includes ->info field allocation)
TTree allocNode();
/** return      - allocated node
**/

// Finds all nodes that satisfy the function f, based on sel value,
// and adds them to list l
int findTree(TTree node, compf f, TGL* l, void* sel);
/** node        - node from which the search begins (the function will scan
***               all this node's children)
*** f           - isvalid function based on parameter sel
*** l           - address to the list of found elements that will be passed
***               by refference / NULL if no such element is found
*** sel         - selection value that determines the outcome of function f
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Pastes the style string into the str string (str works as a buffer)
// Used in formatTree; see (TTree.c:74 -> 79)
int sprint_style(void* style, void* str);
/** style       - pointer to a style attribute
*** str         - pointer to the buffer
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Prints attribute to output file
int print_attribute(void* attribute, void* file_ptr);
/** attribute   - pointer to attribute
*** file_ptr    - pointer to output file
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Prints the formatted html code
int formatTree(TTree a, int indent, FILE* f_out);
/** a           - pointer to <html> node
*** indent      - number that counts the tab indentation (use 0)
*** f_out       - pointer to output file
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Frees a TAttr attribute
void free_attr(void* atr);
/** atr         - pointer to attribute
**/

// Frees a TInfo element
void destroyInfo(TInfo* i);
/** i           - pointer to info
**/

// Destroys tree / subtree a
void destroyArb(TTree* a);
/** a           - address of the beginning of tree / subree
**/


int deleteRec(TTree a, TTree n);
/** a           - pointer to <html> node
*** n           - node from which the deletion begins
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

/* --------------------------Tree ID functions-------------------------- */

// Function that returns the node with id "id" in tree / subtree a
// FUNCTION USES STRTOK AND IT MESSES UP LAST STRTOK POINTER. BEWARE
TTree findId(TTree a, char* id);
/** a           - pointer to the beginning of tree (the (null) node)
*** id          - the id to be searched
*** return      - pointer to the node with the given id / NULL if not found
**/

// Function that returns an id based on the previous sibling's id
char* getNewIdPrev(char* id);
/** id          - previous sibling's id
*** return      - new id / NULL if operation was unsuccesful
**/

// Function that adds an id to the current node based on its previous sibling
int addId(TTree cur, TTree prev);
/** cur         - pointer to current node
*** prev        - pointer to previous sibling / NULL if cur is a firstChild
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
***/

// Function that recursively rewrites id's after the deletion of a node
int rewriteIdRec(TTree cur, TTree prev);
/** cur         - pointer to the start node
*** prev        - pointer to previous sibling / NULL if cur is a firstChild
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

/* ------------------------Tree building functions------------------------ */

// Adds node to parent_node
int addNode(char* tagname, TTree* parent_node);
/** tagname     - the tagname of the new node
*** parent_node - pointer to the parent node
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Adds style list to node
int addStyle(char* styles, TTree node);
/** styles      - style list string
*** node        - pointer to node
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Adds attribute name to node (builds the list cell)
int addAttributeName(char* attr_name, TTree node);
/** attr_name   - attribute name string
*** node        - pointer to node
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Adds attritube value to node (completes already existing list cell)
int addAttributeValue(char* attr_value, TTree node);
/** attr_value  - attribute value string
*** node        - pointer to node
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Adds content to node
int addContent(char* content, TTree node);
/** content     - content string
*** node        - pointer to node
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

// Function that builds the tree, reading the input character by character
TParseState buildTree(TParseState curState, char c, TTree* n, char* temp);
/** curState    - current state of building
*** c           - current char being read
*** n           - pointer to current node (initialise with root - (null) node)
*** temp        - buffer string (allocate MAX_STRING elements for it)
*** return      - returns the next state of building, based on the character c
***/

// Function that overrides the current node's styles with the new styles
void overrideNodeStyle(TTree a, TTree n, char* style);
/** a           - pointer to <html> node
*** n           - pointer to current node
*** style       - new style list string
***/

// Function that appends the new styles to the current node's styles
int appendToStyle(TTree a, TTree n, char* style);
/** a           - pointer to <html> node
*** n           - pointer to current node
*** style       - new style list string
*** return      - returns whether the operation was succesful or not (1-y/ 0-n)
**/

#endif /* _TARB_H_ */
