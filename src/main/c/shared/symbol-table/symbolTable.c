
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
    Symbol *sym = symbolTableLookup(table, name);
    if (!sym || sym->type != SYM_VAR) return false;
    free(sym->value);
    sym->value = strdup(value);
    return true;
}

bool symbolTableGetValue(SymbolTable *table, const char *name, char **outValue) {
    Symbol *sym = symbolTableLookup(table, name);
    if (!sym || sym->type != SYM_VAR || !sym->value) return false;
    *outValue = strdup(sym->value);
    return true;
}
