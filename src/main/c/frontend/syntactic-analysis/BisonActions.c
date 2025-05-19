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
Form* FormSemanticAction(ParameterList* attrs, ParameterList* styles, ListItem* items) {
    Form* form = calloc(1, sizeof(Form));
    form->attributes = attrs;
    form->styles = styles;
    form->body = items;
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
Nav* NavSemanticAction(ParameterList* style, ListItem* items) {
    Nav* nav = calloc(1, sizeof(Nav));
    nav->attributes = style;
    nav->items = items;
    return nav;
}


// Button
Button* ButtonWithAttrsSemanticAction(ParameterList* attrs, ParameterList* styles, StatementList* body) {
    Button* button = calloc(1, sizeof(Button));
    button->attributes = attrs;
    button->styles = styles;
    button->body = body;
    return button;
}

Button* ButtonSemanticAction(ParameterList* styles, StatementList* body) {
    return ButtonWithAttrsSemanticAction(NULL, styles, body);
}


// Card
Card* CardSemanticAction(ParameterList* attrs, StatementList* body) {
    Card* card = calloc(1, sizeof(Card));
    card->attributes = attrs;
    card->body = body;
    return card;
}

OrderedList* OrderedListWithStyleSemanticAction(ParameterList* style, ListItem* items) {
    OrderedList* list = calloc(1, sizeof(OrderedList));
    list->items = items;
    list->attributes = style;
    return list;
}

UnorderedList* UnorderedListWithStyleSemanticAction(ParameterList* style, ListItem* items) {
    UnorderedList* list = calloc(1, sizeof(UnorderedList));
    list->items = items;
    list->attributes = style;
    return list;
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


Table* TableSemanticAction(TableRowList* rows) {
    Table* table = (Table*)malloc(sizeof(Table));
    table->rows = rows;
    return table;
}

TableRow* TableRowSemanticAction(TableCellList* cells) {
    TableRow* row = (TableRow*)malloc(sizeof(TableRow));
    row->cells = cells;
    return row;
}

TableRowList* SingleTableRowAction(TableRow* row) {
    TableRowList* list = (TableRowList*)malloc(sizeof(TableRowList));
    list->row = row;
    list->next = NULL;
    return list;
}

TableRowList* AppendTableRowAction(TableRowList* list, TableRow* row) {
    if (list == NULL) {
        return SingleTableRowAction(row);
    }

    TableRowList* head = list;
    while (list->next != NULL) {
        list = list->next;
    }

    TableRowList* newNode = (TableRowList*)malloc(sizeof(TableRowList));
    newNode->row = row;
    newNode->next = NULL;
    list->next = newNode;

    return head;
}

TableCellList* SingleTableCellAction(TableCell* cell) {
    TableCellList* list = (TableCellList*)malloc(sizeof(TableCellList));
    list->cell = cell;
    list->next = NULL;
    return list;
}

TableCellList* AppendTableCellAction(TableCellList* list, TableCell* cell) {
    if (list == NULL) {
        return SingleTableCellAction(cell);
    }

    TableCellList* head = list;
    while (list->next != NULL) {
        list = list->next;
    }

    TableCellList* newNode = (TableCellList*)malloc(sizeof(TableCellList));
    newNode->cell = cell;
    newNode->next = NULL;
    list->next = newNode;

    return head;
}

TableCell* TableCellSemanticAction(char* value) {
    TableCell* cell = (TableCell*)malloc(sizeof(TableCell));
    cell->content = value; 
    return cell;
}


ListItem* ParseListItems(char* itemsText) {
    ListItem* head = NULL;
    ListItem* tail = NULL;
    char* line = strtok(itemsText, "\r\n");
    while (line != NULL) {
        char* content = line;

        // Quitar prefijo numérico "n. "
        if (isdigit((unsigned char)content[0])) {
            char* dot = strchr(content, '.');
            if (dot && dot[1] == ' ') {
                content = dot + 2;
            }
        }
        // Quitar prefijo de viñeta "* "
        else if (content[0] == '*' && content[1] == ' ') {
            content += 2;
        }

        // Creamos un nodo ListItem con el contenido limpio
        ListItem* item = ListItemSemanticAction(strdup(content));
        // Enlazamos al final de la lista
        if (head == NULL) {
            head = tail = item;
        } else {
            tail->next = item;
            tail = item;
        }

        line = strtok(NULL, "\r\n");
    }

    return head;
}

ListItem* SingleListItemNodeSemanticAction(ListItem* item) {
    return item;
}

ListItem* AppendListItemNodeSemanticAction(ListItem* list, ListItem* item) {
    if (list == NULL) return item;
    ListItem* current = list;
    while (current->next != NULL)
        current = current->next;
    current->next = item;
    return list;
}

ListItem* ListItemSemanticActionWithParameters(ParameterList* parameters) {
    ListItem* item = calloc(1, sizeof(ListItem));
    item->parameters = parameters;
    return item;
}
