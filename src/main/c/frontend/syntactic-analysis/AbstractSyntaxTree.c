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

// ------------------------
// Lista de Statements
// ------------------------
void releaseStatementList(StatementList* list) {
	while (list != NULL) {
		StatementList* next = list->next;
		releaseStatement(list->statement);
		free(list);
		list = next;
	}
}

// ------------------------
// Statement
// ------------------------
void releaseStatement(Statement* statement) {
	if (statement == NULL) return;

	switch (statement->type) {
		case STATEMENT_TEXT:
            logDebugging(_logger, "Releasing text: %s", __FUNCTION__);
			if (statement->text != NULL) {
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
            releaseParameterList(statement->define->parameters);
            releaseParameterList(statement->define->style);
            releaseStatementList(statement->define->body);
            free(statement->define);
            break;
        case STATEMENT_USE:
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
        free(item);
        item = next;
    }
}

void releaseNavItems(NavItem* item) {
    while (item) {
        NavItem* next = item->next;
        free(item);
        item = next;
    }
}

