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
// Helpers
static Node* createNode(NodeType type) {
    Node* node = malloc(sizeof(Node));
    node->type = type;
    return node;
}

// Programa
Program* ProgramSemanticAction(CompilerState* compilerState, StatementList* statements) {
    Program* program = malloc(sizeof(Program));
    program->statements = statements;
    return program;
}

// Sentencias
StatementList* AppendStatementSemanticAction(StatementList* list, Node* statement) {
    if (!list) return SingleStatementSemanticAction(statement);

    StatementList* current = list;
    while (current->next) current = current->next;

    StatementList* newNode = malloc(sizeof(StatementList));
    newNode->statement = statement;
    newNode->next = NULL;

    current->next = newNode;
    return list;
}

StatementList* SingleStatementSemanticAction(Node* statement) {
    StatementList* list = malloc(sizeof(StatementList));
    list->statement = statement;
    list->next = NULL;
    return list;
}

// Texto
Node* TextSemanticAction(char* content, int level) {
    Node* node = createNode(NODE_TEXT);
    node->text = malloc(sizeof(Text)); 
    node->text->content = strdup(content);
    node->text->level = level;
    return node;
}

// Imagen
Node* ImageSemanticAction(char* src, char* alt) {
    Node* node = createNode(NODE_IMAGE);
    node->image = malloc(sizeof(Image));
    node->image->src = strdup(src);
    node->image->alt = strdup(alt);
    return node;
}

// Define
Node* DefineSemanticAction(char* name, ParameterList* params, StatementList* body) {
    Node* node = createNode(NODE_DEFINE);
    node->define = malloc(sizeof(Define));
    node->define->name = strdup(name);
    node->define->parameters = params;
    node->define->body = body;
    return node;
}

// Use
Node* UseSemanticAction(char* name, ParameterList* arguments) {
    Node* node = createNode(NODE_USE);
    node->use = malloc(sizeof(Use));
    node->use->name = strdup(name);
    node->use->arguments = arguments;
    return node;
}

// Formulario
Node* FormSemanticAction(char* name, ParameterList* fields, ParameterList* attributes, StatementList* body) {
    Node* node = createNode(NODE_FORM);
    node->form = malloc(sizeof(Form));
    node->form->name = strdup(name);
    node->form->fields = fields;
    node->form->attributes = attributes;
    node->form->body = body;
    return node;
}

// Footer
Node* FooterSemanticAction(ParameterList* attrs, StatementList* body) {
    Node* node = createNode(NODE_FOOTER);
    node->footer = malloc(sizeof(Footer));
    node->footer->attributes = attrs;
    node->footer->body = body;
    return node;
}

// Row
Node* RowSemanticAction(StatementList* columns) {
    Node* node = createNode(NODE_ROW);
    node->row = malloc(sizeof(Row));
    node->row->columns = columns;
    return node;
}

// Column
Node* ColumnSemanticAction(ParameterList* attrs, StatementList* body) {
    Node* node = createNode(NODE_COLUMN);
    node->column = malloc(sizeof(Column));
    node->column->attributes = attrs;
    node->column->body = body;
    return node;
}

// Nav
Node* NavSemanticAction(ParameterList* attrs, ListItem* items) {
    Node* node = createNode(NODE_NAV);
    node->nav = malloc(sizeof(Nav));
    node->nav->attributes = attrs;
    node->nav->items = items;
    return node;
}

// Lista ordenada
Node* OrderedListSemanticAction(ListItem* items) {
    Node* node = createNode(NODE_ORDERED_LIST);
    node->ordered_list = malloc(sizeof(OrderedList));
    node->ordered_list->items = items;
    return node;
}

// Lista no ordenada
Node* UnorderedListSemanticAction(ListItem* items) {
    Node* node = createNode(NODE_UNORDERED_LIST);
    node->unordered_list = malloc(sizeof(UnorderedList));
    node->unordered_list->items = items;
    return node;
}

// Parámetros
ParameterList* EmptyParameterListSemanticAction() {
    ParameterList* list = malloc(sizeof(ParameterList));
    list->head = NULL;
    return list;
}

ParameterList* SingleParameterSemanticAction(char* name, char* type, char* default_value) {
    Parameter* param = malloc(sizeof(Parameter));
    param->name = strdup(name);
    param->type = strdup(type);
    param->default_value = strdup(default_value);
    param->next = NULL;

    ParameterList* list = malloc(sizeof(ParameterList));
    list->head = param;
    return list;
}

ParameterList* AppendParameterSemanticAction(ParameterList* list, char* name, char* type, char* default_value) {
    Parameter* param = malloc(sizeof(Parameter));
    param->name = strdup(name);
    param->type = strdup(type);
    param->default_value = strdup(default_value);
    param->next = list->head;
    list->head = param;
    return list;
}

// Items de lista
ListItem* ListItemSemanticAction(char* content) {
    ListItem* item = malloc(sizeof(ListItem));
    item->content = strdup(content);
    item->next = NULL;
    return item;
}

ListItem* AppendListItemSemanticAction(ListItem* list, char* content) {
    ListItem* item = ListItemSemanticAction(content);
    item->next = list;
    return item;
}

ListItem* PrependOrderedItemSemanticAction(ListItem* list, char* content) {
	ListItem* item = ListItemSemanticAction(content);
	item->next = list;
	return item;
}
ListItem* EmptyOrderedItemListSemanticAction(void) {
	ListItem* list = malloc(sizeof(ListItem));
	list->content = NULL;
	list->next = NULL;
	return list;
}
ListItem* PrependBulletItemSemanticAction(char* item, ListItem* tail) {
	ListItem* newItem = malloc(sizeof(ListItem));
	newItem->content = strdup(item);
	newItem->next = tail;
	return newItem;
}
ListItem* EmptyBulletItemListSemanticAction(void) {
	ListItem* list = malloc(sizeof(ListItem));
	list->content = NULL;
	list->next = NULL;
	return list;
}