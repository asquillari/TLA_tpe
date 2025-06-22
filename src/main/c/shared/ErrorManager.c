#include "ErrorManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void newErrorNode(ErrorManager* em, ErrorType type, char * msg, printErrorFunction errorFunction);
static void printErrorExistsFunction(int errorNumber, ErrorNode* node);
static void printErrorUndifinedVariable(int errorNumber, ErrorNode* node);
static void printErrorExistsVariable(int errorNumber, ErrorNode* node);
static void printErrorUndifinedFunction(int errorNumber, ErrorNode* node);
static void printErrorExistsFunction(int errorNumber, ErrorNode* node);
static void printErrorTooManyArgs(int errNo, ErrorNode* node);
static void printErrorTooFewArgs(int errNo, ErrorNode* node);

ErrorManager * newErrorManager(){
    return calloc(1,sizeof(ErrorManager));
}

static void newErrorNode(ErrorManager* em, ErrorType type, char * msg, printErrorFunction errorFunction){
    ErrorNode * newNode = malloc(sizeof(ErrorNode));
    if(newNode == NULL){
        return;
    }
    newNode->msg = msg;
    newNode->type = type;
    newNode->next = NULL;
    newNode->printError = errorFunction;

    if(em->first == NULL){
        em->first = newNode;
        em->last = newNode;
    }else{
        em->last->next = newNode;
    }
    em->errorCount += 1;
}

static void printError(int errorNumber, ErrorNode* node){
    static char* messages[] = {
        "Undefined variable.",
        "Variable already defined.",
        "Undefined function.",
        "Function already defined.",
        "Too many parameters for function.",
        "Invalid ordered list item."
                              };
    
    fprintf(stderr, "[Error %i]: %s.\n", errorNumber, messages[node->type]);
    return;
}

static void printErrorUndifinedVariable(int errorNumber, ErrorNode* node){
    printf("[Error %i]: Undefined reference to variable \"%s\".\n", errorNumber, node->msg);
    return;
}

static void printErrorExistsVariable(int errorNumber, ErrorNode* node){
    printf("[Error %i]: Variable \"%s\" already defined.\n", errorNumber, node->msg);
    return;
}

static void printErrorUndifinedFunction(int errorNumber, ErrorNode* node){
    printf("[Error %i]: Undefined reference to function \"%s\".\n", errorNumber, node->msg);
    return;
}

static void printErrorExistsFunction(int errorNumber, ErrorNode* node){
    printf("[Error %i]: Function \"%s\" already defined.\n", errorNumber, node->msg);
    return;
}

static void printErrorTooManyVariables(int errorNumber, ErrorNode* node){
    printf("[Error %i]: Too many parameters for function \"%s\".\n", errorNumber, node->msg);
    return;
}

static void printInvalidOrderedListError(int errorNumber, ErrorNode* node){
    printf("[Error %i]: Invalid ordered list item: expected %s.\n", errorNumber, node->msg);
    return;
}

static void printErrorTooManyArgs(int errNo, ErrorNode* node) {
    printf ("[Error %d]: Too many arguments for function %s.\n", errNo, node->msg);
    return;
}

static void printErrorTooFewArgs(int errNo, ErrorNode* node) {
    printf( "[Error %d]: Too few arguments for function %s.\n", errNo, node->msg);
    return;
}


void showErrors(ErrorManager* em){
    if(em->errorsShown){
        return;
    }
    em->errorsShown = 0;
    
    ErrorNode * aux = em->first;
    for(int i=1 ; aux!=NULL ; ++i){
        aux->printError(i,aux);
        aux = aux->next;
    }
}

void freeErrorManager(ErrorManager* em){
    ErrorNode * en = em->first;
    while(en!=NULL){
        ErrorNode * aux = en->next;
        if(en->msg!=NULL)
            free(en->msg);
        free(en);
        en = aux;
    }
    free(em);
}
void useUndefinedVariable(ErrorManager* em, char* varName){
    char * msg = malloc(strlen(varName) + 1);
    strcpy(msg,varName);
    newErrorNode(em, UNDIFINED_VAR, msg, printErrorUndifinedVariable);
}
void addAlreadyDefinedVariable(ErrorManager* em, char* varName){
    char * msg = malloc(strlen(varName) + 1);
    strcpy(msg,varName);
    newErrorNode(em, UNDIFINED_VAR, msg, printErrorExistsVariable);
}
void useUndefinedFunction(ErrorManager* em, char* funcName){
    char * msg = malloc(strlen(funcName) + 1);
    strcpy(msg, funcName);
    newErrorNode(em, UNDIFINED_FUNC, msg, printErrorUndifinedFunction);
}
void addAlreadyDefinedFunction(ErrorManager* em, char* funcName){
    char * msg = malloc(strlen(funcName) + 1);
    strcpy(msg, funcName);
    newErrorNode(em, ALREADY_DEFINED_FUNC, msg, printErrorExistsFunction);
}

void useParameterIndexOutOfRange(ErrorManager* em, char* name, int idx){
    char * ms = malloc(strlen(name) + 1);
    strcpy(ms, name);
    newErrorManager(em, OUT_OF_INDEX, ms, printErrorTooManyVariables);
}

void addInvalidOrderedListError(ErrorManager *em, const char *numeroRecibido, int numeroEsperado) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", numeroEsperado);

    char *msg = malloc(strlen(buf) + 1);
    if (!msg) return;
    strcpy(msg, buf);

    newErrorNode(em,
                 INVALID_ORDERED_LIST_ITEM, 
                 msg,
                 printInvalidOrderedListError);
}

void addTooManyArgumentsError(ErrorManager* em, const char* funcName, int declared, int passed) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%s: expected %d args but got %d", funcName, declared, passed);
    char* msg = malloc(strlen(buf)+1);
    strcpy(msg, buf);
    newErrorNode(em, TOO_MANY_ARGS, msg, printErrorTooManyArgs);
}

void addTooFewArgumentsError(ErrorManager* em, const char* funcName, int declared, int passed) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%s: expected %d args but got %d", funcName, declared, passed);
    char* msg = malloc(strlen(buf)+1);
    strcpy(msg, buf);
    newErrorNode(em, TOO_FEW_ARGS, msg, printErrorTooFewArgs);
}
