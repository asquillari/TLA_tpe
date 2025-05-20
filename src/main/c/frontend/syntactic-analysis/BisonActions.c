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

// Programa
Program* StatementSemanticAction(CompilerState* compilerState, StatementList* statements) {
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

StatementList* createSingleStatementList(Statement* stmt) {
    StatementList* list = calloc(1, sizeof(StatementList));
    list->statement = stmt;
    list->next = NULL;
    return list;
}

StatementList* appendStatementToList(StatementList* list, Statement* stmt) {
    StatementList* head = list;
    while (list->next != NULL) {
        list = list->next;
    }
    list->next = createSingleStatementList(stmt);
    return head;
}

ParameterList* createParameterList() {
    ParameterList* list = calloc(1, sizeof(ParameterList));
    return list;
}

void appendParameter(ParameterList* list, char* key, char* value) {
    Parameter* param = calloc(1, sizeof(Parameter));
    param->key = key;
    param->value = value;
    param->next = NULL;

    if (list->head == NULL) {
        list->head = param;
    } else {
        Parameter* current = list->head;
        while (current->next != NULL)
            current = current->next;
        current->next = param;
    }
}

Statement* DefineSemanticAction(char* name, ParameterList* parameters, ParameterList* style, StatementList* body) {
    Define* define = calloc(1, sizeof(Define));
    define->name = name;
    define->parameters = parameters;
    define->style = style;
    define->body = body;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_DEFINE;
    stmt->define = define;

    return stmt;
}

Statement* TextFromQuotedValueSemanticAction(char* value) {
	Text* text = calloc(1, sizeof(Text));
	text->content = value;  

	Statement* stmt = calloc(1, sizeof(Statement));
	stmt->type = STATEMENT_TEXT;
	stmt->text = text;

	return stmt;
}

Statement* TextFromVariableSemanticAction(char* varName) {
	Text* text = calloc(1, sizeof(Text));
	text->content = varName;

	Statement* stmt = calloc(1, sizeof(Statement));
	stmt->type = STATEMENT_TEXT;
	stmt->text = text;

	return stmt;
}

Statement* ImageSemanticAction(ParameterList* style, char* src, char* alt) {
    Image* image = calloc(1, sizeof(Image));
    image->style = style;
    image->src = src;
    image->alt = alt;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_IMAGE;
    stmt->image = image;

    return stmt;
}

Statement* ButtonSemanticAction(ParameterList* style, ParameterList* action, StatementList* body) {
    Button* btn = calloc(1, sizeof(Button));
    btn->style = style;
    btn->action = action;
    btn->body = body;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_BUTTON;
    stmt->button = btn;
    return stmt;
}

Statement* CardSemanticAction(ParameterList* style, StatementList* body) {
    Card* card = calloc(1, sizeof(Card));
    card->style = style;
    card->body = body;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_CARD;
    stmt->card = card;
    return stmt;
}

Statement* UseSemanticAction(char* name, ParameterList* parameters) {
    Use* use = calloc(1, sizeof(Use));
    use->name = name;
    use->parameters = parameters;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_USE;
    stmt->use = use;
    return stmt;
}

FormItem* FormItemSemanticAction(char* label, char* placeholder) {
    FormItem* item = calloc(1, sizeof(FormItem));
    item->label = label;
    item->placeholder = placeholder;
    return item;
}

FormItem* appendFormItem(FormItem* list, FormItem* newItem) {
    if (!list) return newItem;
    FormItem* current = list;
    while (current->next) current = current->next;
    current->next = newItem;
    return list;
}

Statement* FormSemanticAction(ParameterList* style, ParameterList* attrs, FormItem* items) {
    Form* form = calloc(1, sizeof(Form));
    form->style = style;
    form->attributes = attrs;
    form->items = items;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_FORM;
    stmt->form = form;
    return stmt;
}
NavItem* NavItemSemanticAction(char* label, char* link) {
    NavItem* item = calloc(1, sizeof(NavItem));
    item->label = label;
    item->link = link;
    return item;
}

NavItem* appendNavItem(NavItem* list, NavItem* newItem) {
    if (!list) return newItem;
    NavItem* current = list;
    while (current->next) current = current->next;
    current->next = newItem;
    return list;
}

Statement* NavSemanticAction(ParameterList* style, ParameterList* attrs, NavItem* items) {
    Nav* nav = calloc(1, sizeof(Nav));
    nav->style = style;
    nav->attributes = attrs;
    nav->items = items;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_NAV;
    stmt->nav = nav;
    return stmt;
}

Statement* FooterSemanticAction(ParameterList* style, StatementList* body){
    Footer* footer = calloc(1, sizeof(Footer));
    footer->style = style;
    footer->body = body;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_FOOTER;
    stmt->footer = footer;

    return stmt;
}

Statement* ColumnSemanticAction(ParameterList* style, StatementList* body) {
    Column* col = calloc(1, sizeof(Column));
    col->style = style;
    col->body = body;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_COLUMN;
    stmt->column = col;
    return stmt;
}

Statement* RowSemanticAction(ParameterList* style, StatementList* columns) {
    Row* row = calloc(1, sizeof(Row));
    row->style = style;
    row->columns = columns;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_ROW;
    stmt->row = row;
    return stmt;
}

