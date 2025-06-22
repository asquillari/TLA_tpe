#include "backend/code-generation/Generator.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/CompilerState.h"
#include "shared/Environment.h"
#include "shared/Logger.h"
#include "shared/String.h"
#include "shared/symbol-table/symbolTable.h"
#include "shared/ErrorManager.h"

/**
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
int main(const int count, const char ** arguments) {
    Logger * logger = createLogger("EntryPoint");
    initializeFlexActionsModule();
    initializeBisonActionsModule();
    initializeSyntacticAnalyzerModule();
    initializeAbstractSyntaxTreeModule();
    initializeGeneratorModule();

    for (int k = 0; k < count; ++k) {
        logDebugging(logger, "Argument %d: \"%s\"", k, arguments[k]);
    }

    CompilerState compilerState = {
        .abstractSyntaxtTree = NULL,
        .succeed            = true,
        .symbolTable        = createSymbolTable(),
        .value              = 0,
        .errorManager       = newErrorManager()
    };

    SyntacticAnalysisStatus synStatus = parse(&compilerState);

    if (synStatus == ACCEPT && compilerState.succeed) {
        logDebugging(logger, "Generating HTML output...");
        generate(&compilerState);
    } else {
        if (synStatus != ACCEPT) {
            logError(logger, "The syntactic-analysis phase rejects the input program.");
        }
        if (!compilerState.succeed) {
            showErrors(compilerState.errorManager);
        }
        shutdownGeneratorModule();
        shutdownAbstractSyntaxTreeModule();
        shutdownSyntacticAnalyzerModule();
        shutdownBisonActionsModule();
        shutdownFlexActionsModule();
        destroySymbolTable(compilerState.symbolTable);
        freeErrorManager(compilerState.errorManager);
        destroyLogger(logger);
        return EXIT_FAILURE;
    }

    shutdownGeneratorModule();
    shutdownAbstractSyntaxTreeModule();
    shutdownSyntacticAnalyzerModule();
    shutdownBisonActionsModule();
    shutdownFlexActionsModule();
    destroySymbolTable(compilerState.symbolTable);
    freeErrorManager(compilerState.errorManager);
    destroyLogger(logger);
    return EXIT_SUCCESS;
}