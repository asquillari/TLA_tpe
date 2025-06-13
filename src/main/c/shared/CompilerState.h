#ifndef COMPILER_STATE_HEADER
#define COMPILER_STATE_HEADER

#include "Type.h"
#include "symbol-table/symbolTable.h"

/**
 * The general status of a compilation.
*/
typedef enum {
	SUCCEED = 0,
	FAILED = 1
} CompilationStatus;

/**
 * The global state of the compiler. Should transport every data structure
 * needed across the different phases of a compilation.
 */
typedef struct {
	// The root node of the AST.
	void * abstractSyntaxtTree;
	// The current compilation status.		
	// A flag that indicates the current state of the compilation so far.
	boolean succeed;

	SymbolTable * symbolTable;

	// The computed value of the entire program (only for the calculator).
	int value;
} CompilerState;

#endif
