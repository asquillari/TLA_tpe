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
Text* TextSemanticAction(char* content, int level) {
    Text* text = malloc(sizeof(Text));
    text->content = strdup(content);
    text->level = level;
    return text;
}

// Imagen
Image* ImageSemanticAction(char* src, char* alt) {
    Image* image = malloc(sizeof(Image));
    image->src = strdup(src);
    image->alt = strdup(alt);
    return image;
}

// Define
Define* DefineSemanticAction(char* name, ParameterList* params, StatementList* body) {
    Define* define = malloc(sizeof(Define));
    define->name = strdup(name);
    define->parameters = params;
    define->body = body;
    return define;
}

// Use
Use* UseSemanticAction(char* name, ParameterList* arguments) {
    Use* use = malloc(sizeof(Use));
    use->name = strdup(name);
    use->arguments = arguments;
    return use;
}

// Formulario
Form* FormSemanticAction(char* name, ParameterList* fields, ParameterList* attributes, StatementList* body) {
    Form* form = malloc(sizeof(Form));
    form->name = strdup(name);
    form->fields = fields;
    form->attributes = attributes;
    form->body = body;
    return form;
}

// Footer
Footer* FooterSemanticAction(ParameterList* attrs, StatementList* body) {
    Footer* footer = malloc(sizeof(Footer));
    footer->attributes = attrs;
    footer->body = body;
    return footer;
}

// Row
Row* RowSemanticAction(StatementList* columns) {
    Row* row = malloc(sizeof(Row));
    row->columns = columns;
    return row;
}

// Column
Column* ColumnSemanticAction(ParameterList* attrs, StatementList* body) {
    Column* column = malloc(sizeof(Column));
    column->attributes = attrs;
    column->body = body;
    return column;
}

// Nav
Nav* NavSemanticAction(ParameterList* attrs, ListItem* items) {
    Nav* nav = malloc(sizeof(Nav));
    nav->attributes = attrs;
    nav->items = items;
    return nav;
}

// Lista ordenada
OrderedList* OrderedListSemanticAction(ListItem* items) {
    OrderedList* orderedList = malloc(sizeof(OrderedList));
    orderedList->items = items;
    return orderedList;
}

// Lista no ordenada
UnorderedList* UnorderedListSemanticAction(ListItem* items) {
    UnorderedList* unorderedList = malloc(sizeof(UnorderedList));
    unorderedList->items = items;
    return unorderedList;
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
    return NULL;
}

ListItem* PrependBulletItemSemanticAction(char* item, ListItem* tail) {
    ListItem* newItem = malloc(sizeof(ListItem));
    newItem->content = strdup(item);
    newItem->next = tail;
    return newItem;
}

ListItem* EmptyBulletItemListSemanticAction(void) {
    return NULL;
}

ListItem* createListItem(char* content) {
    ListItem* item = malloc(sizeof(ListItem));
    item->content = strdup(content);
    item->next = NULL;
    return item;
}
