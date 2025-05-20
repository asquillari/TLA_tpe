#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>


typedef struct Program Program;
typedef struct StatementList StatementList;
typedef struct Statement Statement;

typedef struct Define Define;
typedef struct Use Use;
typedef struct Form Form;
typedef struct Footer Footer;
typedef struct Row Row;
typedef struct Column Column;
typedef struct Nav Nav;
typedef struct Text Text;
typedef struct Image Image;
typedef struct Button Button;
typedef struct Card Card;
typedef struct OrderedList OrderedList;
typedef struct UnorderedList UnorderedList;
typedef struct Table Table;
typedef struct Modifiers Modifiers;

// Programa y lista de sentencias
// -----------------------------
struct Program {
    StatementList* statements;
};

typedef struct StatementList {
	Statement* statement;
	StatementList* next;
} StatementList;

enum StatementType {
    STATEMENT_DEFINE,
    STATEMENT_USE,
    STATEMENT_FORM,
    STATEMENT_FOOTER,
    STATEMENT_ROW,
    STATEMENT_COLUMN,
    STATEMENT_NAV,
    STATEMENT_TEXT,
    STATEMENT_IMAGE,
    STATEMENT_BUTTON,
    STATEMENT_CARD,
    STATEMENT_ORDERED_LIST,
    STATEMENT_UNORDERED_LIST,
    STATEMENT_TABLE
};

typedef struct Statement {
    enum StatementType type;
    union {
        Define* define;
        Use* use;
        Form* form;
        Footer* footer;
        Row* row;
        Column* column;
        Nav* nav;
        Text* text;
        Image* image;
        Button* button;
        Card* card;
        OrderedList* ordered_list;
        UnorderedList* unordered_list;
        Table* table;
    };
} Statement;

typedef struct Parameter {
    char* key;
    char* value;
    struct Parameter* next;
} Parameter;

typedef struct ParameterList {
    Parameter* head;
} ParameterList;

typedef struct Define {
    char* name;
    ParameterList* parameters;
    ParameterList* style;
    StatementList* body;
} Define;


typedef struct Text {
    char* content;
} Text;

typedef struct Image {
	char* src;
	char* alt;
	ParameterList* style;
} Image;

typedef struct FormItem {
    char* label;
    char* placeholder;
    struct FormItem* next;
} FormItem;

typedef struct Form {
    ParameterList* style;
    ParameterList* attributes;
    FormItem* items;
} Form;

typedef struct NavItem {
    char* label;
    char* link;
    struct NavItem* next;
} NavItem;

typedef struct Nav {
    ParameterList* style;
    ParameterList* attributes;
    NavItem* items;
} Nav;


typedef struct Button {
    ParameterList* style;
    ParameterList* action;
    StatementList* body;
} Button;


typedef struct Card {
    ParameterList* style;
    StatementList* body;
} Card;

typedef struct Use {
    char* name;
    ParameterList* parameters;
} Use;

typedef struct Footer {
    ParameterList* style;
    StatementList* body;
} Footer;

typedef struct Column {
    ParameterList* style;
    StatementList* body;
} Column;

typedef struct Row {
    ParameterList* style;
    StatementList* columns; // lista de Column envueltos como Statement
} Row;



void initializeAbstractSyntaxTreeModule();
void shutdownAbstractSyntaxTreeModule();
void releaseProgram(Program* program);
void releaseStatementList(StatementList* list);
void releaseStatement(Statement* statement);
void releaseParameterList(ParameterList* list);
void releaseParameterList(ParameterList* list);
void releaseFormItems(FormItem* item);
void releaseNavItems(NavItem* item);

#endif