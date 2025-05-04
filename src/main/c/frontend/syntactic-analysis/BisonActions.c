#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */


Node *createDefineNode(const char *name, Node *parameters, Node *statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(DEFINE_NODE);
	node->define.name = name;
	node->define.parameters = parameters;
	node->define.statements = statements;
	return node;
}
Node *createUseNode(const char *name, Node *parameters) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(USE_NODE);
	node->use.name = name;
	node->use.parameters = parameters;
	return node;
}
Node *createParameterListNode(const char *parameter) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(PARAMETER_LIST_NODE);
	node->parameterList.parameter = parameter;
	node->parameterList.next = NULL;
	return node;
}
Node *appendParameterToList(Node *parameterList, const char *parameter) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(PARAMETER_LIST_NODE);
	node->parameterList.parameter = parameter;
	node->parameterList.next = NULL;
	if (parameterList == NULL) {
		return node;
	} else {
		Node *current = parameterList;
		while (current->parameterList.next != NULL) {
			current = current->parameterList.next;
		}
		current->parameterList.next = node;
		return parameterList;
	}
}
Node *createFormNode(const char *name, Node *statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(FORM_NODE);
	node->form.name = name;
	node->form.statements = statements;
	return node;
}
Node *createFooterNode(const char *name, Node *statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(FOOTER_NODE);
	node->footer.name = name;
	node->footer.statements = statements;
	return node;
}
Node *createRowNode(Node *statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(ROW_NODE);
	node->row.statements = statements;
	return node;
}
Node *createColumnNode(const char *name, Node *statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(COLUMN_NODE);
	node->column.name = name;
	node->column.statements = statements;
	return node;
}
Node *createNavNode(const char *name, Node *statements) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(NAV_NODE);
	node->nav.name = name;
	node->nav.statements = statements;
	return node;
}
Node *createOrderedListNode(const char *name, Node *items) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(ORDERED_LIST_NODE);
	node->orderedList.name = name;
	node->orderedList.items = items;
	return node;
}
Node *createOrderedItemList(const char *item) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(ORDERED_ITEM_LIST_NODE);
	node->orderedItemList.item = item;
	node->orderedItemList.next = NULL;
	return node;
}
Node *appendOrderedItem(Node *itemList, const char *item) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(ORDERED_ITEM_LIST_NODE);
	node->orderedItemList.item = item;
	node->orderedItemList.next = NULL;
	if (itemList == NULL) {
		return node;
	} else {
		Node *current = itemList;
		while (current->orderedItemList.next != NULL) {
			current = current->orderedItemList.next;
		}
		current->orderedItemList.next = node;
		return itemList;
	}
}
Node *createUnorderedListNode(const char *name, Node *items) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(UNORDERED_LIST_NODE);
	node->unorderedList.name = name;
	node->unorderedList.items = items;
	return node;
}
Node *createBulletItemList(const char *item) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(BULLET_ITEM_LIST_NODE);
	node->bulletItemList.item = item;
	node->bulletItemList.next = NULL;
	return node;
}
Node *appendBulletItem(Node *itemList, const char *item) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(BULLET_ITEM_LIST_NODE);
	node->bulletItemList.item = item;
	node->bulletItemList.next = NULL;
	if (itemList == NULL) {
		return node;
	} else {
		Node *current = itemList;
		while (current->bulletItemList.next != NULL) {
			current = current->bulletItemList.next;
		}
		current->bulletItemList.next = node;
		return itemList;
	}
}
Node *createTextNode(const char *text) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(TEXT_NODE);
	node->text.text = text;
	return node;
}
Node *createImageNode(const char *url, const char *altText) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Node *node = createNode(IMAGE_NODE);
	node->image.url = url;
	node->image.altText = altText;
	return node;
}
