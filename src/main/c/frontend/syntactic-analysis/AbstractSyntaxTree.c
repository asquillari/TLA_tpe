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

void releaseStatementList(Node *statements) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (statements != NULL) {
		Node *current = statements;
		while (current != NULL) {
			Node *next = current->next;
			switch (current->type) {
				case DEFINE_NODE:
					releaseDefineNode(&current->define);
					break;
				case USE_NODE:
					releaseUseNode(&current->use);
					break;
				case FORM_NODE:
					releaseFormNode(&current->form);
					break;
				case FOOTER_NODE:
					releaseFooterNode(&current->footer);
					break;
				case ROW_NODE:
					releaseRowNode(&current->row);
					break;
				case COLUMN_NODE:
					releaseColumnNode(&current->column);
					break;
				case NAV_NODE:
					releaseNavNode(&current->nav);
					break;
				case ORDERED_LIST_NODE:
					releaseOrderedListNode(&current->orderedList);
					break;
				case UNORDERED_LIST_NODE:
					releaseUnorderedListNode(&current->unorderedList);
					break;
				case TEXT_NODE:
					releaseTextNode(&current->text);
					break;
				case IMAGE_NODE:
					releaseImageNode(&current->image);
					break;
				default:
					logError(_logger, "Unknown node type: %d", current->type);
					break;
			}
			free(current);
			current = next;
		}
	}
}

void releaseDefineNode(DefineNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseParameterListNode(node->parameters);
		releaseStatementList(node->statements);
	}
}

void releaseUseNode(UseNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseParameterListNode(node->parameters);
		free(node);
	}
}

void releaseFormNode(FormNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseStatementList(node->statements);
	}
}

void releaseFooterNode(FooterNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseStatementList(node->statements);
	}
}

void releaseRowNode(RowNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		releaseStatementList(node->statements);
	}
}

void releaseColumnNode(ColumnNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseStatementList(node->statements);
	}
}

void releaseNavNode(NavNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseStatementList(node->statements);
	}
}

void releaseOrderedListNode(OrderedListNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseStatementList(node->items);
	}
}

void releaseUnorderedListNode(UnorderedListNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->name);
		releaseStatementList(node->items);
	}
}

void releaseTextNode(TextNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->text);
		free(node);
	}
}

void releaseImageNode(ImageNode *node) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (node != NULL) {
		free(node->url);
		free(node->altText);
		free(node);
	}
}