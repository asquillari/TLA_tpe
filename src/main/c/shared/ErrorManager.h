#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

typedef enum ErrorType {
    UNDIFINED_VAR = 0,
    ALREADY_DEFINED_VAR = 1,
    UNDIFINED_FUNC = 2,
    ALREADY_DEFINED_FUNC = 3,
} ErrorType;

typedef struct ErrorNode {
    ErrorType type;
    char* msg;
    struct ErrorNode* next;
    void (*printError)(int errorNumber, struct ErrorNode* aux);
} ErrorNode;

typedef struct ErrorManager {
    int errorCount;
    int errorsShown;
    ErrorNode* first;
    ErrorNode* last;
} ErrorManager;

typedef void (*printErrorFunction)(int errorNumber, ErrorNode* aux);

ErrorManager* newErrorManager();

void useUndefinedVariable(ErrorManager* em, char* varName, CompilerState* compilerState);
void addAlreadyDefinedVariable(ErrorManager* em, char* varName, CompilerState* compilerState);
void useUndefinedFunction(ErrorManager* em, char* funcName, CompilerState* compilerState);
void addAlreadyDefinedFunction(ErrorManager* em, char* funcNamem, CompilerState* compilerState);
void showErrors(ErrorManager* em);
void freeErrorManager(ErrorManager* em);

#endif