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

void releaseProgram(Program* program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL){
        releaseStatementList(program->statements);
        free(program);
	}
}

void releaseStatementList(StatementList* list) {
	while (list != NULL) {
		StatementList* next = list->next;
		releaseStatement(list->statement);
		free(list);
		list = next;
	}
}

void releaseStatement(Statement* statement) {
	if (statement == NULL) return;
    //seguro hay que ajustar los frees si es que hacemos el strdup para id y demas
	//para ajustar bien hay que ver cuales son char en el .h
    switch (statement->type) {
		case STATEMENT_PARAGRAPH:
        case STATEMENT_HEADER1:
        case STATEMENT_HEADER2:
        case STATEMENT_HEADER3:
            if (statement->text) {
                free(statement->text->content);
                free(statement->text);
            }
            break;
        case STATEMENT_IMAGE:
            releaseParameterList(statement->image->style);
            free(statement->image);
            break;
        case STATEMENT_BUTTON:
            releaseParameterList(statement->button->style);
            releaseParameterList(statement->button->action);
            releaseStatementList(statement->button->body);
            free(statement->button);
            break;
        case STATEMENT_CARD:
            releaseParameterList(statement->card->style);
            releaseStatementList(statement->card->body);
            free(statement->card);
            break;
        case STATEMENT_DEFINE:
            if (statement->define->name) free(statement->define->name);
            releaseParameterList(statement->define->parameters);
            releaseParameterList(statement->define->style);
            releaseStatementList(statement->define->body);
            free(statement->define);
            break;
        case STATEMENT_USE:
            if (statement->use->name) free(statement->use->name);
            releaseParameterList(statement->use->parameters);
            free(statement->use);
            break;
        case STATEMENT_FORM:
            releaseParameterList(statement->form->style);
            releaseParameterList(statement->form->attributes);
            releaseFormItems(statement->form->items);
            free(statement->form);
            break;
        case STATEMENT_NAV:
            releaseParameterList(statement->nav->style);
            releaseParameterList(statement->nav->attributes);
            releaseNavItems(statement->nav->items);
            free(statement->nav);
            break;
        case STATEMENT_FOOTER:
            releaseParameterList(statement->footer->style);
            releaseStatementList(statement->footer->body);
            free(statement->footer);
            break;
        case STATEMENT_COLUMN:
            releaseParameterList(statement->column->style);
            releaseStatementList(statement->column->body);
            free(statement->column);
            break;
        case STATEMENT_ROW:
            releaseParameterList(statement->row->style);
            releaseStatementList(statement->row->columns);
            free(statement->row);
            break;
        case STATEMENT_TABLE:
            releaseTable(statement);
            break;
        case STATEMENT_ORDERED_ITEM:
            if (statement->ordered_item->number) free(statement->ordered_item->number);
            releaseStatement(statement->ordered_item->body);
            free(statement->ordered_item);
            break;
        case STATEMENT_BULLET_ITEM:
            releaseStatement(statement->bullet_item->body);
            free(statement->bullet_item);
            break;
        case STATEMENT_ORDERED_LIST:
            releaseParameterList(statement->ordered_list->style);
            releaseStatementList(statement->ordered_list->items);
            free(statement->ordered_list);
            break;
        case STATEMENT_UNORDERED_LIST:
            releaseParameterList(statement->unordered_list->style);
            releaseStatementList(statement->unordered_list->items);
            free(statement->unordered_list);
            break;
        
		default:
			break;
	}

	free(statement);
}

void releaseParameterList(ParameterList* list) {
    if (!list) return;
    Parameter* current = list->head;
    while (current) {
        Parameter* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void releaseFormItems(FormItem* item) {
    while (item) {
        FormItem* next = item->next;
        if(item->label != NULL) {
            free(item->label);
        }
        if(item->placeholder != NULL) {
            free(item->placeholder);
        }
        free(item);
        item = next;
    }
}

void releaseNavItems(NavItem* item) {
    while (item) {
        NavItem* next = item->next;
        if(item->label != NULL) {
            free(item->label);
        }
        if(item->link != NULL) {
            free(item->link);
        }
        free(item);
        item = next;
    }
}


void releaseTable(Statement* statement) {
    if (!statement || !statement->table) return;

    releaseParameterList(statement->table->style);
    releaseTableRowList(statement->table->rows);
    free(statement->table);
}

void releaseTableRowList(TableRowList* list) {
    while (list) {
        TableRowList* next = list->next;
        releaseTableRow(list->row);
        free(list);
        list = next;
    }
}

void releaseTableRow(TableRow* row) {
    if (!row) return;
    releaseTableCellList(row->cells);
    free(row);
}

void releaseTableCellList(TableCellList* list) {
    while (list) {
        TableCellList* next = list->next;
        releaseTableCellContent(list->cell);
        free(list->cell);
        free(list);
        list = next;
    }
}

void releaseTableCellContent(TableCell* cell) {
    if (!cell) return;
    releaseStatementList(cell->content);
}
