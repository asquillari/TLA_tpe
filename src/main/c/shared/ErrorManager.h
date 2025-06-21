#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H


typedef enum ErrorType {
    UNDIFINED_VAR = 0,
    ALREADY_DEFINED_VAR,
    UNDIFINED_FUNC,
    ALREADY_DEFINED_FUNC,
    OUT_OF_INDEX,
    TOO_MANY_ARGS,        
    TOO_FEW_ARGS,        
    INVALID_ORDERED_LIST_ITEM
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
void addTooManyArgumentsError(ErrorManager* em, const char* funcName, int declared, int passed);
void addTooFewArgumentsError(ErrorManager* em, const char* funcName, int declared, int passed);

#endif