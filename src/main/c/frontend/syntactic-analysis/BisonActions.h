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
Program* StatementSemanticAction(CompilerState* compilerState, StatementList* statement);
ParameterList* createParameterList();
void appendParameter(ParameterList* list, char* key, char* value);

StatementList* createSingleStatementList(Statement* stmt);
StatementList* appendStatementToList(StatementList* list, Statement* stmt);


Statement* TextFromQuotedValueSemanticAction(char* value);
Statement* TextFromVariableSemanticAction(char* varName);

Statement* ImageSemanticAction(ParameterList* style, char* src, char* alt);

Statement* ButtonSemanticAction(ParameterList* style, ParameterList* action, StatementList* body);

Statement* CardSemanticAction(ParameterList* style, StatementList* body);

Statement* DefineSemanticAction(char* name, ParameterList* parameters, ParameterList* style, StatementList* body);

Statement* UseSemanticAction(char* name, ParameterList* parameters);

FormItem* FormItemSemanticAction(char* label, char* placeholder);
FormItem* appendFormItem(FormItem* list, FormItem* newItem);
Statement* FormSemanticAction(ParameterList* style, ParameterList* attrs, FormItem* items);

NavItem* NavItemSemanticAction(char* label, char* link);
NavItem* appendNavItem(NavItem* list, NavItem* newItem);
Statement* NavSemanticAction(ParameterList* style, ParameterList* attrs, NavItem* items);

Statement* FooterSemanticAction(ParameterList* style, StatementList* body);

Statement* ColumnSemanticAction(ParameterList* style, StatementList* body);
Statement* RowSemanticAction(ParameterList* style, StatementList* columns);


#endif
