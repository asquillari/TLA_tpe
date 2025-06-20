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
    logDebugging(_logger, "Appending parameter: %s = %s", key ? key : "NULL", value ? value : "NULL");
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

Statement* DefineSemanticAction(CompilerState *st,
                                char* name,
                                ParameterList* parameters,
                                ParameterList* style,
                                StatementList* body) {
    /*                              
    if(symbolTableLookup(st->symbolTable, name) == NULL) {
        addAlreadyDefinedFunction(st->errorManager, name);
        st->succeed = false;
        return NULL;
    }
    symbolTableInsert(st->symbolTable, name, SYM_FUN, NULL);
    for(Parameter* p = parameters->head; p != NULL; p = p->next) {
        if(symbolTableLookup(st->symbolTable, p->key) == NULL) {
            addAlreadyDefinedFunction(st->errorManager, p->key);
            st->succeed = false;
            return NULL;
        }
        symbolTableInsert(st->symbolTable, p->key, SYM_VAR, NULL);
    }
        */  
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

Statement* HeaderSemanticAction(char* value, int level) {
    Text* t = calloc(1, sizeof(Text));
    t->content = value;
    Statement* s = calloc(1, sizeof(Statement));
    switch (level) {
      case 1: s->type = STATEMENT_HEADER1; break;
      case 2: s->type = STATEMENT_HEADER2; break;
      case 3: s->type = STATEMENT_HEADER3; break;
    }
    s->text = t;
    return s;
}

Statement* ParagraphSemanticAction(char* value) {
    Text* t = calloc(1, sizeof(Text));
    t->content = value;
    Statement* s = calloc(1, sizeof(Statement));
    s->type = STATEMENT_PARAGRAPH;
    s->text = t;
    return s;
}

Statement* ParagraphVariableSemanticAction(CompilerState* compilerState, char* variableName){
    /*
    if(symbolTableLookup(compilerState->symbolTable, variableName) == NULL) {
        useUndefinedVariable(compilerState->errorManager, variableName);
        compilerState->succeed = false;
        return NULL;
    }
        */
    Text* t = calloc(1, sizeof(Text));
    t->content = variableName;
    Statement* s = calloc(1, sizeof(Statement));
    s->type = STATEMENT_PARAGRAPH;
    s->text = t;
    return s;
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

Statement* UseSemanticAction(CompilerState *st,
                             char* name,
                             ParameterList* parameters) {
                                /*
    if(symbolTableLookup(st->symbolTable, name) == NULL) {
        useUndefinedFunction(st->errorManager, name);
        st->succeed = false;
        return NULL;
    }
        */
    //aca falataria chequear que los parametros sean iguales
    
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

Statement* TableSemanticAction(ParameterList* style, TableRowList* rows) {
    Table* table = calloc(1, sizeof(Table));
    table->style = style;
    table->rows = rows;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_TABLE;
    stmt->table = table;
    return stmt;
}
TableRowList* SingleTableRowAction(TableRow* row) {
    TableRowList* list = calloc(1, sizeof(TableRowList));
    list->row = row;
    list->next = NULL;
    return list;
}
TableRowList* AppendTableRowAction(TableRowList* list, TableRow* row) {
    TableRowList* current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = calloc(1, sizeof(TableRowList));
    current->next->row = row;
    return list;
}
TableRow* TableRowSemanticAction(TableCellList* cells) {
    TableRow* row = calloc(1, sizeof(TableRow));
    row->cells = cells;
    return row;
}
TableCellList* SingleTableCellAction(TableCell* cell) {
    TableCellList* list = calloc(1, sizeof(TableCellList));
    list->cell = cell;
    list->next = NULL;
    return list;
}
TableCellList* AppendTableCellAction(TableCellList* list, TableCell* cell) {
    TableCellList* current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = calloc(1, sizeof(TableCellList));
    current->next->cell = cell;
    return list;
}
TableCell* TableCellSemanticAction(StatementList* content) {
    TableCell* cell = calloc(1, sizeof(TableCell));
    cell->content = content;
    return cell;
}

Statement* OrderedListSemanticAction(ParameterList* style, StatementList* items) {
    OrderedList* list = calloc(1, sizeof(OrderedList));
    list->style = style;
    list->items = items;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_ORDERED_LIST;
    stmt->ordered_list = list;
    return stmt;
}

Statement* OrderedItemSemanticAction(char* number, Statement* body) {
    OrderedItem* item = calloc(1, sizeof(OrderedItem));
    item->number = number;
    item->body = body;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_ORDERED_ITEM;
    stmt->ordered_item = item;
    return stmt;
}

Statement* UnorderedListSemanticAction(ParameterList* style, StatementList* items) {
    UnorderedList* list = calloc(1, sizeof(UnorderedList));
    list->style = style;
    list->items = items;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_UNORDERED_LIST;
    stmt->unordered_list = list;
    return stmt;
}

Statement* BulletItemSemanticAction(char* bullet, Statement* body) {
    BulletItem* item = calloc(1, sizeof(BulletItem));
    item->symbol = bullet;
    item->body = body;

    Statement* stmt = calloc(1, sizeof(Statement));
    stmt->type = STATEMENT_BULLET_ITEM;
    stmt->bullet_item = item;
    return stmt;
}
