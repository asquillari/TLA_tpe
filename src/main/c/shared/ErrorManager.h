#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

typedef enum ErrorType {
    UNDIFINED_VAR = 0,
    ALREADY_DEFINED_VAR = 1,
    UNDIFINED_FUNC = 2,
    ALREADY_DEFINED_FUNC = 3,
    OUT_OF_INDEX = 4,
    INVALID_ORDERED_LIST_ITEM = 5
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

void useUndefinedVariable(ErrorManager* em, char* varName);
void addAlreadyDefinedVariable(ErrorManager* em, char* varName);
void useUndefinedFunction(ErrorManager* em, char* funcName);
void addAlreadyDefinedFunction(ErrorManager* em, char* funcName);
void showErrors(ErrorManager* em);
void freeErrorManager(ErrorManager* em);
void useParameterIndexOutOfRange(ErrorManager* em, char* name, int idx);
void addInvalidOrderedListError(ErrorManager *em, const char *numeroRecibido, int numeroEsperado);

#endif