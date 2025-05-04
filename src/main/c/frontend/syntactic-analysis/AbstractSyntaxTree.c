#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				releaseExpression(expression->leftExpression);
				releaseExpression(expression->rightExpression);
				break;
			case FACTOR:
				releaseFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				releaseConstant(factor->constant);
				break;
			case EXPRESSION:
				releaseExpression(factor->expression);
				break;
		}
		free(factor);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseExpression(program->expression);
		free(program);
	}
}

void releaseDefineNode(DefineNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseParameterListNode(node->parameters);
		releaseProgram(node->statements);
		free(node);
	}
}
void releaseUseNode(UseNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseParameterListNode(node->parameters);
		free(node);
	}
}
void releaseFormNode(FormNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseProgram(node->statements);
		free(node);
	}
}
void releaseFooterNode(FooterNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseProgram(node->statements);
		free(node);
	}
}
void releaseRowNode(RowNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		releaseProgram(node->statements);
		free(node);
	}
}
void releaseColumnNode(ColumnNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseProgram(node->statements);
		free(node);
	}
}
void releaseNavNode(NavNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseProgram(node->statements);
		free(node);
	}
}
void releaseOrderedListNode(OrderedListNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseProgram(node->items);
		free(node);
	}
}
void releaseUnorderedListNode(UnorderedListNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseProgram(node->items);
		free(node);
	}
}
void releaseTextNode(TextNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->text);
		free(node);
	}
}
void releaseImageNode(ImageNode *node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->url);
		free(node->altText);
		free(node);
	}
}