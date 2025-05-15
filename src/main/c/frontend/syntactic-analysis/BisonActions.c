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
    Program * program = calloc(1, sizeof(Program));
    program->statements = statements;
	compilerState->abstractSyntaxtTree = program;
	if(0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default(0): %d", flexCurrentContext());
		compilerState->succeed = false;
	} else {
		compilerState->succeed = true;
	}
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
Text* TextSemanticAction(char* content, int level) {
    Text* text = calloc(1, sizeof(Text));
    text->content = content;  
    text->level = level;
    return text;
}

// Imagen
Image* ImageSemanticAction(char* src, char* alt) {
    Image* image = calloc(1, sizeof(Image));
	image->src = src;  
	image->alt = alt; 
	return image;
}

// Define
Define* DefineSemanticAction(char* name, ParameterList* params, StatementList* body) {
    Define * define = calloc(1, sizeof(Define));
	define->name = name;
	define->parameters = params;
	define->body = body;
	return define;
}

// Use
Use* UseSemanticAction(char* name, ParameterList* arguments) {
    Use * use = calloc(1, sizeof(Use));
	use->name = name;
	use->arguments = arguments;
	return use;
}

// Formulario
Form* FormSemanticAction(char* name, ParameterList* fields, ParameterList* attributes, StatementList* body) {
    Form* form = calloc(1, sizeof(Form));
	form->name = name;
	form->fields = fields;
	form->attributes = attributes;
	form->body = body;
	return form;
}

// Footer
Footer* FooterSemanticAction(ParameterList* attrs, StatementList* body) {
    Footer* footer = calloc(1, sizeof(Footer));
	footer->attributes = attrs;
	footer->body = body;
	return footer;
}

// Row
Row* RowSemanticAction(StatementList* columns) {
    Row* row = calloc(1, sizeof(Row));
	row->columns = columns;
	return row;
}

// Column
Column* ColumnSemanticAction(ParameterList* attrs, StatementList* body) {
    Column* column = calloc(1, sizeof(Column));
	column->attributes = attrs;
	column->body = body;
	return column;
}

// Nav
Nav* NavSemanticAction(ParameterList* attrs, ListItem* items) {
    Nav* nav = calloc(1, sizeof(Nav));
	nav->attributes = attrs;
	nav->items = items;
	return nav;
}

// Lista ordenada
OrderedList* OrderedListSemanticAction(ListItem* items) {
    OrderedList* orderedList = calloc(1, sizeof(OrderedList));
	orderedList->items = items;
	return orderedList;
}

// Lista no ordenada
UnorderedList* UnorderedListSemanticAction(ListItem* items) {
    UnorderedList* unorderedList = calloc(1, sizeof(UnorderedList));
	unorderedList->items = items;
	return unorderedList;
}

// Parámetros
ParameterList* EmptyParameterListSemanticAction() {
    ParameterList* list = calloc(1, sizeof(ParameterList));
    list->head = NULL;
    return list;
}

ParameterList* SingleParameterSemanticAction(char* name, char* type, char* default_value) {
    Parameter* param = calloc(1, sizeof(Parameter));
    param->name = name;
    param->type = type;
    param->default_value = default_value;
    param->next = NULL;

    ParameterList* list = malloc(sizeof(ParameterList));
    list->head = param;
    return list;
}

ParameterList* AppendParameterSemanticAction(ParameterList* list, char* name, char* type, char* default_value) {
    Parameter* param = calloc(1, sizeof(Parameter));
    param->name = name;
    param->type = type;
    param->default_value = default_value;
    param->next = list->head;
    list->head = param;
    return list;
}

// Items de lista
ListItem* ListItemSemanticAction(char* content) {
    ListItem* item = calloc(1, sizeof(ListItem));
    item->content = content;
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
    return NULL;
}

ListItem* PrependBulletItemSemanticAction(char* item, ListItem* tail) {
    ListItem* newItem = calloc(1, sizeof(ListItem));
    newItem->content = item;
    newItem->next = tail;
    return newItem;
}

ListItem* EmptyBulletItemListSemanticAction(void) {
    return NULL;
}

ListItem* createListItem(char* content) {
    ListItem* item = calloc(1, sizeof(ListItem));
    item->content = content;
    item->next = NULL;
    return item;
}
