#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static const char _expressionTypeToCharacter(const ExpressionType type);
static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
static void _generateEpilogue(const int value);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
static const char _expressionTypeToCharacter(const ExpressionType type) {
	switch (type) {
		case ADDITION: return '+';
		case DIVISION: return '/';
		case MULTIPLICATION: return '*';
		case SUBTRACTION: return '-';
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return '\0';
	}
}

/**
 * Generates the output of a constant.
 */
static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
	_output(indentationLevel, "%s", "[ $C$, circle, draw, black!20\n");
	_output(1 + indentationLevel, "%s%d%s", "[ $", constant->value, "$, circle, draw ]\n");
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
//no se si necesitamos agregar algo al final, para html
static void _generateEpilogue(const int value) {
	_output(0, "%s%d%s",
		"            [ $", value, "$, circle, draw, blue ]\n"
		"        ]\n"
		"    \\end{forest}\n"
		"\\end{document}\n\n"
	);
}

/**
 * Generates the output of an expression.
 */
static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
	switch (expression->type) {
		case ADDITION:
		case DIVISION:
		case MULTIPLICATION:
		case SUBTRACTION:
			_generateExpression(1 + indentationLevel, expression->leftExpression);
			_output(1 + indentationLevel, "%s%c%s", "[ $", _expressionTypeToCharacter(expression->type), "$, circle, draw, purple ]\n");
			_generateExpression(1 + indentationLevel, expression->rightExpression);
			break;
		case FACTOR:
			_generateFactor(1 + indentationLevel, expression->factor);
			break;
		default:
			logError(_logger, "The specified expression type is unknown: %d", expression->type);
			break;
	}
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Generates the output of a factor.
 */
static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
	_output(indentationLevel, "%s", "[ $F$, circle, draw, black!20\n");
	switch (factor->type) {
		case CONSTANT:
			_generateConstant(1 + indentationLevel, factor->constant);
			break;
		case EXPRESSION:
			_output(1 + indentationLevel, "%s", "[ $($, circle, draw, purple ]\n");
			_generateExpression(1 + indentationLevel, factor->expression);
			_output(1 + indentationLevel, "%s", "[ $)$, circle, draw, purple ]\n");
			break;
		default:
			logError(_logger, "The specified factor type is unknown: %d", factor->type);
			break;
	}
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Generates the output of the program.
 */
//en el caso del ejemplo el programa solo arranca una expresion
static void _generateProgram(Program * program) {
	_generateExpression(3, program->expression);
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
// esto es lo que se genera al principio del archivo, el prologo
//tendriamos que poner lo de doctype ponele
static void _generatePrologue(void) {
	_output(0, "%s",
		"\\documentclass{standalone}\n\n"
		"\\usepackage[utf8]{inputenc}\n"
		"\\usepackage[T1]{fontenc}\n"
		"\\usepackage{amsmath}\n"
		"\\usepackage{forest}\n"
		"\\usepackage{microtype}\n\n"
		"\\begin{document}\n"
		"    \\centering\n"
		"    \\begin{forest}\n"
		"        [ \\text{$=$}, circle, draw, purple\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
//lo hace mas lindo, para html sirve para que se vea mejor
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
//esto hay que modificar si queres cambiar el tipo de salida, por ejemplo a un archivo
//el generador deberia quedar transparente al tipo de salida, por eso es importante
//que la salida la maneje output
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	//podemos tener un generador de codigo por cada tipo de nodo y hacerlo recursivo
	//nos enfoocamos en cada nodo, arrancamos con el nodo raiz y vamos bajando
	//no si o si tiene que ser recursivo, pero es una buena forma de hacerlo
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}
