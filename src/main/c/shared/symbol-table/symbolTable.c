#include "symbolTable.h"
#include <stdlib.h>
#include <string.h>

SymbolTable* createSymbolTable(void) {
    SymbolTable *table = calloc(1, sizeof(SymbolTable));
    table->head = NULL;
    return table;
}

typedef struct Symbol Symbol;
void destroySymbolTable(SymbolTable *table) {
    if (!table) return;
    Symbol *cur = table->head;
    while (cur) {
        Symbol *next = cur->next;
        free(cur->name);
        free(cur->value);
        free(cur);
        cur = next;
    }
    free(table);
}

Symbol* symbolTableLookup(SymbolTable *table, const char *name) {
    for (Symbol *s = table->head; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

Symbol* symbolTableInsert(SymbolTable *table, const char *name, SymbolType type, const char *initialValue) {
    if (!table || !name || strlen(name) == 0) return NULL;
    Symbol *existing = symbolTableLookup(table, name);
    if (existing) return existing;

    Symbol *sym = calloc(1, sizeof(Symbol));
    sym->name  = strdup(name);
    sym->type  = type;
    sym->value = initialValue ? strdup(initialValue) : NULL;
    sym->next  = table->head;
    table->head = sym;
    return sym;
}

bool symbolTableSetValue(SymbolTable *table, const char *name, const char *value) {
    if (!table || !name || strlen(name) == 0) return false;
    Symbol *sym = symbolTableLookup(table, name);
    if (!sym || sym->type != SYM_VAR) return false;
    free(sym->value);
    sym->value = strdup(value);
    return true;
}

bool symbolTableGetValue(SymbolTable *table, const char *name, char **outValue) {
    if (!table || !name || strlen(name) == 0) return NULL;
    Symbol *sym = symbolTableLookup(table, name);
    if (!sym || sym->type != SYM_VAR || !sym->value) return false;
    *outValue = strdup(sym->value);
    return true;
}

// Nota: impementracion de la parte de funciones de la tabla de simbolos
/*
Symbol* symbolTableInsertFunction(SymbolTable *table, const char *name, void *params) {
    if (!table || !name || strlen(name) == 0) return NULL;
    Symbol *existing = symbolTableLookup(table, name);
    if (existing) return existing;

    Symbol *sym = calloc(1, sizeof(Symbol));
    sym->name  = strdup(name);
    sym->type  = SYM_FUN;
    sym->params = (void *)params; // aca no se si castearlo aca o cuando llamo a la funcion
    sym->next  = table->head;
    table->head = sym;
    return sym;
}

bool symbolTableHasFunction(SymbolTable *table, const char *name) {
    Symbol *sym = symbolTableLookup(table, name);
    return sym && sym->type == SYM_FUN;
}

void * symbolTableGetFunctionParams(SymbolTable *table, const char *name) {
    Symbol *sym = symbolTableLookup(table, name);
    if (sym && sym->type == SYM_FUN) {
        return sym->params;
    }
    return NULL;
}
*/

//Nota: implementacion de las funciones de scopes
/*
void pushScope(SymbolTableStack **stack) {
    SymbolTableStack *newScope = calloc(1, sizeof(SymbolTableStack));
    newScope->table = createSymbolTable();
    newScope->next = *stack;
    *stack = newScope;
}

void popScope(SymbolTableStack **stack) {
    if (!stack || !*stack) return;
    SymbolTableStack *top = *stack;
    *stack = top->next;
    destroySymbolTable(top->table);
    free(top);
}

Symbol* scopedLookup(SymbolTableStack *stack, const char *name) {
    for (SymbolTableStack *s = stack; s; s = s->next) {
        Symbol *sym = symbolTableLookup(s->table, name);
        if (sym) return sym;
    }
    return NULL;
}

Symbol* scopedInsert(SymbolTableStack *stack, const char *name, SymbolType type, const char *initialValue) {
    if (!stack) return NULL;
    return symbolTableInsert(stack->table, name, type, initialValue);
}
*/
