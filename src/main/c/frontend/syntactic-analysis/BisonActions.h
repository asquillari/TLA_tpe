#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/symbol-table/symbolTable.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include "../../shared/ErrorManager.h"
#include <stdlib.h>

/** Inicializa y libera estado interno del módulo */
void initializeBisonActionsModule();
void shutdownBisonActionsModule();

/**
 * Acciones semánticas para construir nodos del AST
 */

Program* StatementSemanticAction(CompilerState* compilerState, StatementList* statement);
ParameterList* createParameterList();
void appendParameter(ParameterList* list, char* key, char* value);

StatementList* createSingleStatementList(Statement* stmt);
StatementList* appendStatementToList(StatementList* list, Statement* stmt);


Statement* HeaderSemanticAction(char* value, int level);
Statement* ParagraphSemanticAction(char* value);
Statement* ParagraphVariableSemanticAction(CompilerState* compilerState, char* variableName);

Statement* ImageSemanticAction(ParameterList* style, char* src, char* alt);

Statement* ButtonSemanticAction(ParameterList* style, ParameterList* action, StatementList* body);

Statement* CardSemanticAction(ParameterList* style, StatementList* body);

Statement* DefineSemanticAction(CompilerState *st,
                                char* name,
                                ParameterList* parameters,
                                ParameterList* style,
                                StatementList* body);

Statement* UseSemanticAction(CompilerState *st,
                             char* name,
                             ParameterList* parameters);


FormItem* FormItemSemanticAction(char* label, char* placeholder);
FormItem* appendFormItem(FormItem* list, FormItem* newItem);
Statement* FormSemanticAction(ParameterList* style, ParameterList* attrs, FormItem* items);

NavItem* NavItemSemanticAction(char* label, char* link);
NavItem* appendNavItem(NavItem* list, NavItem* newItem);
Statement* NavSemanticAction(ParameterList* style, ParameterList* attrs, NavItem* items);

Statement* FooterSemanticAction(ParameterList* style, StatementList* body);

Statement* ColumnSemanticAction(ParameterList* style, StatementList* body);
Statement* RowSemanticAction(ParameterList* style, StatementList* columns);

Statement* TableSemanticAction(ParameterList* style, TableRowList* rows);
TableRowList* SingleTableRowAction(TableRow* row);
TableRowList* AppendTableRowAction(TableRowList* list, TableRow* row);
TableRow* TableRowSemanticAction(TableCellList* cells);
TableCellList* SingleTableCellAction(TableCell* cell);
TableCellList* AppendTableCellAction(TableCellList* list, TableCell* cell);
TableCell* TableCellSemanticAction(StatementList* content);

Statement* OrderedListSemanticAction(CompilerState* st, ParameterList* style, StatementList* items);
Statement* OrderedItemSemanticAction(char* number, Statement* body);

Statement* UnorderedListSemanticAction(ParameterList* style, StatementList* items);
Statement* BulletItemSemanticAction(char* bullet, Statement* body);

Statement* HeaderVariableSemanticAction(CompilerState* st, char* variableName, int level);

#endif
