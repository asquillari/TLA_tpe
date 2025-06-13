// symbolTable.h
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    SYM_VAR,
    SYM_FUN
} SymbolType;

typedef struct Symbol {
    char          *name;      
    SymbolType     type;      
    char          *value;     
    struct Symbol *next;     
} Symbol;

// por ahora lo dejamos como una lista, quizas no es escalable y haya que cambiarlo
typedef struct SymbolTable {
    Symbol *head;
} SymbolTable;

SymbolTable* createSymbolTable(void);
void         destroySymbolTable(SymbolTable *table);

Symbol*      symbolTableLookup(SymbolTable *table, const char *name);
Symbol*      symbolTableInsert(SymbolTable *table, const char *name, SymbolType type, const char *initialValue);

bool         symbolTableSetValue(SymbolTable *table, const char *name, const char *value);
bool         symbolTableGetValue(SymbolTable *table, const char *name, char **outValue);

// *** PARA FUNCIONES ***
//todavia no se como lo implementariamos porque no deberiamos poder usar parameterlist desde aca
//no se si quizas deberia estar todo junto y no separado por tipo de simbolo
// bool         symbolTableInsertFunction(...);
// ParameterList* symbolTableGetFunctionParams(...);
// bool         symbolTableHasFunction(...);

#endif // SYMBOL_TABLE_H

