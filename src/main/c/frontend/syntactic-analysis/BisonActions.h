#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Inicializa y libera estado interno del módulo */
void initializeBisonActionsModule();
void shutdownBisonActionsModule();

/**
 * Acciones semánticas para construir nodos del AST
 */

// Programa
Program* ProgramSemanticAction(CompilerState* compilerState, StatementList* statements);

// Sentencias
StatementList* AppendStatementSemanticAction(StatementList* list, Node* statement);
StatementList* SingleStatementSemanticAction(Node* statement);

// Texto
Text* TextSemanticAction(char* content, int level);

// Imagen
Image* ImageSemanticAction(char* src, char* alt);

// Define / Use
Define* DefineSemanticAction(char* name, ParameterList* params, StatementList* body);
Use* UseSemanticAction(char* name, ParameterList* arguments);

// Formulario
Form* FormSemanticAction(char* name, ParameterList* fields, ParameterList* attributes, StatementList* body);

// Footer / Row / Column / Nav
Footer* FooterSemanticAction(ParameterList* attrs, StatementList* body);
Row* RowSemanticAction(StatementList* columns);
Column* ColumnSemanticAction(ParameterList* attrs, StatementList* body);
Nav* NavSemanticAction(ParameterList* attrs, ListItem* items);
Button* ButtonSemanticAction(ParameterList* attrs, StatementList* body);
Card* CardSemanticAction(ParameterList* attrs, StatementList* body);

// Listas
OrderedList* OrderedListSemanticAction(ListItem* items);
UnorderedList* UnorderedListSemanticAction(ListItem* items);

// Parámetros y argumentos
ParameterList* EmptyParameterListSemanticAction();
ParameterList* SingleParameterSemanticAction(char* name, char* type, char* default_value);
ParameterList* AppendParameterSemanticAction(ParameterList* list, char* name, char* type, char* default_value);

// Atributos del bloque (action: ..., method: ..., etc.)
ParameterList* AttributeListSemanticAction(ParameterList* list, char* key, char* value);

// Items de lista
ListItem* ListItemSemanticAction(char* content);
ListItem* AppendListItemSemanticAction(ListItem* list, char* content);

ListItem* PrependOrderedItemSemanticAction(ListItem* list, char* content);
ListItem* EmptyOrderedItemListSemanticAction(void);
ListItem* PrependBulletItemSemanticAction(char* item, ListItem* tail);
ListItem* EmptyBulletItemListSemanticAction(void);

ListItem* createListItem(char* content);

#endif
