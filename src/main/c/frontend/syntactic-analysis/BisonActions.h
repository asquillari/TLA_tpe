#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Node *createNode(NodeType type); 
// Define-related functions
Node *createDefineNode(const char *name, Node *parameters, Node *statements);
Node *createUseNode(const char *name, Node *parameters);

// Parameter-related functions
Node *createParameterListNode(const char *parameter);
Node *appendParameterToList(Node *parameterList, const char *parameter);

// Form-related functions
Node *createFormNode(const char *name, Node *statements);

// Footer-related functions
Node *createFooterNode(const char *name, Node *statements);

// Row-related functions
Node *createRowNode(Node *statements);

// Column-related functions
Node *createColumnNode(const char *name, Node *statements);

// Nav-related functions
Node *createNavNode(const char *name, Node *statements);

// Ordered list-related functions
Node *createOrderedListNode(const char *name, Node *items);
Node *createOrderedItemList(const char *item);
Node *appendOrderedItem(Node *itemList, const char *item);

// Unordered list-related functions
Node *createUnorderedListNode(const char *name, Node *items);
Node *createBulletItemList(const char *item);
Node *appendBulletItem(Node *itemList, const char *item);

// Text-related functions
Node *createTextNode(const char *text);

// Image-related functions
Node *createImageNode(const char *url, const char *altText);

Node *createStatementList(Node *statement);
Node *appendStatement(Node *list, Node *statement);
void releaseParameterListNode(Node *parameters);


#endif