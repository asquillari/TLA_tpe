#include "ErrorManager.h"
#include "shared/CompilerState.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void newErrorNode(ErrorManager* em, ErrorType type, char * msg, printErrorFunction errorFunction);
static void printErrorExistsFunction(int errorNumber, ErrorNode* node);
static void printErrorUndifinedVariable(int errorNumber, ErrorNode* node);
static void printErrorExistsVariable(int errorNumber, ErrorNode* node);
static void printErrorUndifinedFunction(int errorNumber, ErrorNode* node);
static void printErrorExistsFunction(int errorNumber, ErrorNode* node);

ErrorManager * newErrorManager(){
    return calloc(1,sizeof(ErrorManager));
}

static void newErrorNode(ErrorManager* em, ErrorType type, char * msg, printErrorFunction errorFunction){
    ErrorNode * newNode = malloc(sizeof(ErrorNode));
    if(newNode == NULL){
        outOfMemory(em);
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
        "Function already defined."
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


void showErrors(ErrorManager* em){
    if(em->errorsShown){
        return;
    }
    em->errorsShown = true;
    
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
void useUndefinedVariable(ErrorManager* em, char* varName, CompilerState* compilerState){
    compilerState->succeed = FAILED;
    char * msg = malloc(strlen(varName) + 1);
    strcpy(msg,varName);
    newErrorNode(em, UNDIFINED_VAR, msg, printErrorUndifinedVariable);
}
void addAlreadyDefinedVariable(ErrorManager* em, char* varName, CompilerState* compilerState){
    compilerState->succeed = FAILED;
    char * msg = malloc(strlen(varName) + 1);
    strcpy(msg,varName);
    newErrorNode(em, UNDIFINED_VAR, msg, printErrorExistsVariable);
}
void useUndefinedFunction(ErrorManager* em, char* funcName, CompilerState* compilerState){
    compilerState->succeed = FAILED;
    char * msg = malloc(strlen(funcName) + 1);
    strcpy(msg, funcName);
    newErrorNode(em, UNDIFINED_FUNC, msg, printErrorUndifinedFunction);
}
void addAlreadyDefinedFunction(ErrorManager* em, char* funcName, CompilerState* compilerState){
    compilerState->succeed = FAILED;
    char * msg = malloc(strlen(funcName) + 1);
    strcpy(msg, funcName);
    newErrorNode(em, ALREADY_DEFINED_FUNC, msg, printErrorExistsFunction);
}
