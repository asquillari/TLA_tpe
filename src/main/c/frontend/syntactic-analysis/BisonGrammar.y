%{
#include "BisonActions.h"
%}

%define api.value.union.name SemanticValue

%union {
	Token token;
	char* string;
	int integer;

	struct Node* node;

	struct Program* program;
	struct StatementList* stmt_list;
	struct Define* define;
	struct Use* use;
	struct Form* form;
	struct Footer* footer;
	struct Row* row;
	struct Column* column;
	struct Nav* nav;
	struct Text* text;
	struct Image* image;
	struct Button* button;
	struct Card* card;
	struct List* list;
	struct ListItem* list_item;
	struct OrderedList* ordered_list;
	struct UnorderedList* unordered_list;
	struct ParameterList* parameter_list;
	struct ListItem* list_item_list;

	struct Table* table;
	struct TableRow* table_row;
	struct TableCell* table_cell;
	struct TableRowList* table_row_list;
	struct TableCellList* table_cell_list;
}

%token <token> DEFINE USE FORM IMG FOOTER ROW COLUMN NAV ITEM END BUTTON CARD
%token <token> OPEN_PAREN CLOSE_PAREN OPEN_BRACE CLOSE_BRACE COLON COMMA EQUALS OPEN_BRACKET CLOSE_BRACKET PIPE QUOTE
%token <token> NEWLINE UNKNOWN HEADER_1 HEADER_2 HEADER_3
%token <string> ORDERED_ITEM BULLET
%token <string> QUOTED_VALUE UNQUOTED_VALUE IDENTIFIER TEXT
%token <string> VARIABLE

%token <token> TABLE_BEGIN

%type <program> program
%type <node> statement content_item
%type <stmt_list> statement_list statement_list_nonempty content_list
%type <define> define
%type <use> use
%type <form> form
%type <footer> footer
%type <row> row
%type <column> column
%type <nav> nav
%type <ordered_list> ordered_list_statement
%type <unordered_list> unordered_list_statement
%type <image> image
%type <text> text
%type <button> button
%type <card> card
%type <table> table
%type <table_row> table_row
%type <table_cell> table_cell
%type <table_row_list> table_row_list
%type <table_cell_list> nonempty_table_cell_list

%type <parameter_list> maybe_parameters parameter_list
%type <parameter_list> style_list style_item_list
%type <parameter_list> argument_list argument_list_nonempty
%type <list_item_list> ordered_item_list bullet_item_list

%%

program:
    statement_list
    { $$ = ProgramSemanticAction(currentCompilerState(), $1); }
;

statement_list:
      /* empty */ { $$ = NULL; }
    | statement_list_nonempty { $$ = $1; }
;

statement_list_nonempty:
      statement { $$ = SingleStatementSemanticAction($1); }
    | statement_list_nonempty statement { $$ = AppendStatementSemanticAction($1, $2); }
;

statement:
      NEWLINE { $$ = NULL; }
    | define { $$ = (struct Node*)$1; }
    | use { $$ = (struct Node*)$1; }
    | form { $$ = (struct Node*)$1; }
    | footer { $$ = (struct Node*)$1; }
    | row { $$ = (struct Node*)$1; }
    | column { $$ = (struct Node*)$1; }
    | nav { $$ = (struct Node*)$1; }
    | ordered_list_statement { $$ = (struct Node*)$1; }
    | unordered_list_statement { $$ = (struct Node*)$1; }
    | image { $$ = (struct Node*)$1; }
    | text { $$ = (struct Node*)$1; }
    | button { $$ = (struct Node*)$1; }
    | card { $$ = (struct Node*)$1; }
    | table { $$ = (struct Node*)$1; }
;

define:
    DEFINE IDENTIFIER maybe_parameters statement_list_nonempty END
    { $$ = DefineSemanticAction($2, $3, $4); }
;

use:
      USE IDENTIFIER OPEN_PAREN argument_list CLOSE_PAREN { $$ = UseSemanticAction($2, $4); }
    | USE IDENTIFIER { $$ = UseSemanticAction($2, NULL); }
;

argument_list:
      argument_list_nonempty { $$ = $1; }
;

argument_list_nonempty:
      QUOTED_VALUE { $$ = SingleParameterSemanticAction(NULL, "string", $1); }
    | QUOTED_VALUE COMMA argument_list_nonempty { $$ = AppendParameterSemanticAction($3, NULL, "string", $1); }
;

maybe_parameters:
      /* empty */ { $$ = EmptyParameterListSemanticAction(); }
    | OPEN_PAREN parameter_list CLOSE_PAREN { $$ = $2; }
;

parameter_list:
      IDENTIFIER
        { $$ = SingleParameterSemanticAction($1, "string", NULL); }
    | IDENTIFIER COMMA parameter_list
        { $$ = AppendParameterSemanticAction($3, $1, "string", NULL); }
;

style_list:
      /* empty */ { $$ = EmptyParameterListSemanticAction(); }
    | style_item_list { $$ = $1; }
;

style_item_list:
      IDENTIFIER COLON UNQUOTED_VALUE
        { $$ = SingleParameterSemanticAction($1, NULL, $3); }
    | style_item_list IDENTIFIER COLON UNQUOTED_VALUE
        { $$ = AppendParameterSemanticAction($1, $2, NULL, $4); }
;

content_list:
      content_item { $$ = SingleStatementSemanticAction($1); }
    | content_list content_item { $$ = AppendStatementSemanticAction($1, $2); }
;

content_item:
      text { $$ = (struct Node*)$1; }
    | VARIABLE { $$ = (struct Node*) TextSemanticAction($1, 0); }
;

text:
      HEADER_1 QUOTED_VALUE { $$ = TextSemanticAction($2, 1); }
    | HEADER_2 QUOTED_VALUE { $$ = TextSemanticAction($2, 2); }
    | HEADER_3 QUOTED_VALUE { $$ = TextSemanticAction($2, 3); }
    | QUOTED_VALUE { $$ = TextSemanticAction($1, 0); }
;

ordered_list_statement:
    ordered_item_list { $$ = OrderedListSemanticAction($1); }
;

unordered_list_statement:
    bullet_item_list { $$ = UnorderedListSemanticAction($1); }
;

ordered_item_list:
      ORDERED_ITEM NEWLINE { $$ = ListItemSemanticAction($1); }
    | ordered_item_list ORDERED_ITEM NEWLINE
        { $$ = PrependOrderedItemSemanticAction($1, $2); }
;

bullet_item_list:
      BULLET NEWLINE { $$ = ListItemSemanticAction($1); }
    | bullet_item_list BULLET NEWLINE
        { $$ = PrependBulletItemSemanticAction($2, $1); }
;

image:
    IMG OPEN_PAREN parameter_list CLOSE_PAREN
    { $$ = ImageSemanticAction($3); }
;

form:
      FORM IDENTIFIER OPEN_PAREN parameter_list CLOSE_PAREN OPEN_PAREN parameter_list CLOSE_PAREN content_list END
        { $$ = FormSemanticAction($2, $4, $7, $9); }
    | FORM IDENTIFIER OPEN_PAREN parameter_list CLOSE_PAREN content_list END
        { $$ = FormSemanticAction($2, $4, NULL, $6); }
    | FORM IDENTIFIER content_list END
        { $$ = FormSemanticAction($2, NULL, NULL, $3); }
;

footer:
    FOOTER OPEN_BRACE style_list CLOSE_BRACE content_list END
    { $$ = FooterSemanticAction($3, $5); }
;

row:
    ROW content_list END
    { $$ = RowSemanticAction($2); }
;

column:
    COLUMN OPEN_BRACE style_list CLOSE_BRACE content_list END
    { $$ = ColumnSemanticAction($3, $5); }
;

nav:
      NAV OPEN_BRACE style_list CLOSE_BRACE bullet_item_list END
        { $$ = NavSemanticAction($3, $5); }
    | NAV bullet_item_list END
        { $$ = NavSemanticAction(NULL, $2); }
;

button:
    BUTTON OPEN_BRACE style_list CLOSE_BRACE content_list END
    { $$ = ButtonSemanticAction($3, $5); }
;

card:
    CARD OPEN_BRACE style_list CLOSE_BRACE content_list END
    { $$ = CardSemanticAction($3, $5); }
;

table:
    TABLE_BEGIN table_row_list END
    { $$ = TableSemanticAction($2); }
;

table_row_list:
      table_row { $$ = SingleTableRowAction($1); }
    | table_row_list table_row { $$ = AppendTableRowAction($1, $2); }
;

table_row:
    PIPE nonempty_table_cell_list PIPE
    { $$ = TableRowSemanticAction($2); }
;

nonempty_table_cell_list:
      table_cell { $$ = SingleTableCellAction($1); }
    | nonempty_table_cell_list PIPE table_cell
        { $$ = AppendTableCellAction($1, $3); }
;

table_cell:
    QUOTED_VALUE { $$ = TableCellSemanticAction($1); }
;
