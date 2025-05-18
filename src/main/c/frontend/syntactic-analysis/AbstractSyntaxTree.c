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

void releaseNode(Node* node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_DEFINE:
            releaseParameterList(node->define->parameters);
            releaseStatementList(node->define->body);
            free(node->define->name);
            free(node->define);
            break;
        case NODE_USE:
            releaseParameterList(node->use->arguments);
            free(node->use->name);
            free(node->use);
            break;
        case NODE_FORM:
            releaseParameterList(node->form->fields);
            releaseParameterList(node->form->attributes);
            releaseStatementList(node->form->body);
            free(node->form->name);
            free(node->form);
            break;
        case NODE_FOOTER:
            releaseParameterList(node->footer->attributes);
            releaseStatementList(node->footer->body);
            free(node->footer);
            break;
        case NODE_ROW:
            releaseStatementList(node->row->columns);
            free(node->row);
            break;
        case NODE_COLUMN:
            releaseParameterList(node->column->attributes);
            releaseStatementList(node->column->body);
            free(node->column);
            break;
        case NODE_NAV:
            releaseParameterList(node->nav->attributes);
            releaseListItems(node->nav->items);
            free(node->nav);
            break;
        case NODE_BUTTON:
            releaseParameterList(node->button->attributes);
            releaseStatementList(node->button->body);
            free(node->button);
            break;
        case NODE_CARD:
            releaseParameterList(node->card->attributes);
            releaseStatementList(node->card->body);
            free(node->card);
            break;
        case NODE_TEXT:
            free(node->text->content);
            free(node->text);
            break;
        case NODE_IMAGE:
            free(node->image->src);
            free(node->image->alt);
            free(node->image);
            break;
        
        case NODE_ORDERED_LIST:
            releaseListItems(node->ordered_list->items);
            free(node->ordered_list);
            break;
        case NODE_UNORDERED_LIST:
            releaseListItems(node->unordered_list->items);
            free(node->unordered_list);
            break;
        default:
            break;
    }

    free(node);
}

void releaseStatementList(StatementList* list) {
    while (list != NULL) {
        StatementList* next = list->next;
        releaseNode(list->statement);
        free(list);
        list = next;
    }
}

void releaseParameterList(ParameterList* params) {
    if (params == NULL) return;
    Parameter* current = params->head;
    while (current != NULL) {
        Parameter* next = current->next;
        free(current->name);
        free(current->type);
        free(current->default_value);
        free(current);
        current = next;
    }
    free(params);
}

void releaseListItems(ListItem* items) {
    while (items != NULL) {
        ListItem* next = items->next;
        free(items->content);
        free(items);
        items = next;
    }
}