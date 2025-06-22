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

Symbol* symbolTableInsert(SymbolTable *table, const char *name, const char *ofFunction, SymbolType type, const char *initialValue) {
    if (!table || !name || strlen(name) == 0) return NULL;
    Symbol *existing = symbolTableLookup(table, name);
    if (existing) return existing;

    Symbol *sym = calloc(1, sizeof(Symbol));
    sym->name  = strdup(name);
    sym->type  = type;
    sym->value = initialValue ? strdup(initialValue) : NULL;
    sym->next  = table->head;
    sym->ofFunction = ofFunction ? strdup(ofFunction) : NULL; 
    table->head = sym;
    return sym;
}

bool symbolTableSetValue(SymbolTable *table, const char *function, const char *value, int index) {
    if (!table || !function || strlen(function) == 0)
        return false;

    int count = 0;
    for (Symbol *s = table->head; s; s = s->next) {
        if (s->type == SYM_VAR
            && s->ofFunction
            && strcmp(s->ofFunction, function) == 0) {
            if (count == index) {
                free(s->value);
                s->value = strdup(value);
                return true;
            }
            count++;
        }
    }
    return false;
}

bool symbolTableGetValue(SymbolTable *table, const char *name, char **outValue) {
    if (!table || !name || strlen(name) == 0) return NULL;
    Symbol *sym = symbolTableLookup(table, name);
    if (!sym || sym->type != SYM_VAR || !sym->value) return false;
    *outValue = strdup(sym->value);
    return true;
}

int symbolTableGetParameterCount(SymbolTable *table, const char *function) {
    if (!table || !function || strlen(function) == 0) return 0;

    int count = 0;
    for (Symbol *s = table->head; s; s = s->next) {
        if (s->type == SYM_VAR && s->ofFunction && strcmp(s->ofFunction, function) == 0) {
            count++;
        }
    }
    return count;
}