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

%token <token> DEFINE USE FORM IMG FOOTER ROW COLUMN NAV ITEM END BUTTON CARD LIST_BEGIN TABLE_BEGIN
%token <token> OPEN_PAREN CLOSE_PAREN OPEN_BRACE CLOSE_BRACE COLON COMMA EQUALS OPEN_BRACKET CLOSE_BRACKET PIPE QUOTE
%token <token> NEWLINE UNKNOWN HEADER_1 HEADER_2 HEADER_3
%token <string> ORDERED_ITEM BULLET
%token <string> QUOTED_VALUE UNQUOTED_VALUE IDENTIFIER TEXT
%token <string> VARIABLE


%type <program> program
%type <node> statement atomic_statement content_item list_statement
%type <stmt_list> statement_list statement_list_nonempty content_list
%type <define> define
%type <use> use
%type <form> form
%type <footer> footer
%type <row> row
%type <column> column
%type <nav> nav
%type <image> image
%type <text> text
%type <button> button
%type <card> card
%type <table> table
%type <table_row> table_row
%type <table_cell> table_cell
%type <table_row_list> table_row_list
%type <table_cell_list> nonempty_table_cell_list

%type <parameter_list> maybe_parameters parameter_list style_block_opt
%type <parameter_list> style_list style_item_list
%type <parameter_list> argument_list argument_list_nonempty
%type <list_item_list> nav_item_list form_item_list ordered_items unordered_items
%type <list_item> nav_item form_item

%%

program:
    statement_list
    { $$ = ProgramSemanticAction(currentCompilerState(), $1); }
;

statement_list:
      /* empty */ { $$ = NULL; }
    | statement_list_nonempty { $$ = $1; }
;

atomic_statement:
    NEWLINE                            { $$ = NULL; }
  | define                             { $$ = (struct Node*)$1; }
  | use                                { $$ = (struct Node*)$1; }
  | form                               { $$ = (struct Node*)$1; }
  | footer                             { $$ = (struct Node*)$1; }
  | row                                { $$ = (struct Node*)$1; }
  | column                             { $$ = (struct Node*)$1; }
  | nav                                { $$ = (struct Node*)$1; }
  | image                              { $$ = (struct Node*)$1; }
  | text                               { $$ = (struct Node*)$1; }
  | button                             { $$ = (struct Node*)$1; }
  | card                               { $$ = (struct Node*)$1; }
  | table                              { $$ = (struct Node*)$1; }
;

statement:
    atomic_statement                   { $$ = $1; }
  | list_statement                    { $$ = (struct Node*)$1; }
;

list_statement:
      LIST_BEGIN style_block_opt ordered_items END
        { $$ = (struct Node*)OrderedListWithStyleSemanticAction($2, $3); }

    | LIST_BEGIN style_block_opt unordered_items END
        { $$ = (struct Node*)UnorderedListWithStyleSemanticAction($2, $3); }
;

style_block_opt:
      /* empty */ { $$ = EmptyParameterListSemanticAction(); }
    | OPEN_BRACE style_list CLOSE_BRACE { $$ = $2; }
;


ordered_items:
      ORDERED_ITEM QUOTED_VALUE
        {
          ParameterList* p = SingleParameterSemanticAction("text", NULL, $2);
          ListItem* item = ListItemSemanticActionWithParameters(p);
          $$ = SingleListItemNodeSemanticAction(item);
        }
    | ordered_items ORDERED_ITEM QUOTED_VALUE
        {
          ParameterList* p = SingleParameterSemanticAction("text", NULL, $3);
          ListItem* item = ListItemSemanticActionWithParameters(p);
          $$ = AppendListItemNodeSemanticAction($1, item);
        }
;

unordered_items:
      BULLET QUOTED_VALUE
        {
          ParameterList* p = SingleParameterSemanticAction("text", NULL, $2);
          ListItem* item = ListItemSemanticActionWithParameters(p);
          $$ = SingleListItemNodeSemanticAction(item);
        }
    | unordered_items BULLET QUOTED_VALUE
        {
          ParameterList* p = SingleParameterSemanticAction("text", NULL, $3);
          ListItem* item = ListItemSemanticActionWithParameters(p);
          $$ = AppendListItemNodeSemanticAction($1, item);
        }
;


statement_list_nonempty:
    statement                          { $$ = SingleStatementSemanticAction($1); }
  | statement_list_nonempty statement  { $$ = AppendStatementSemanticAction($1, $2); }
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
     row { $$ = (struct Node*)$1; }
    | column { $$ = (struct Node*)$1; }
    | nav { $$ = (struct Node*)$1; }
    | button { $$ = (struct Node*)$1; }
    | card { $$ = (struct Node*)$1; }
    | table { $$ = (struct Node*)$1; }
    | image { $$ = (struct Node*)$1; }
    | form { $$ = (struct Node*)$1; }
    | footer { $$ = (struct Node*)$1; }
    | define { $$ = (struct Node*)$1; }
    | use { $$ = (struct Node*)$1; }
    | text { $$ = (struct Node*)$1; }
;

text:
      HEADER_1 QUOTED_VALUE   { $$ = TextSemanticAction($2, 1); }
    | HEADER_2 QUOTED_VALUE   { $$ = TextSemanticAction($2, 2); }
    | HEADER_3 QUOTED_VALUE   { $$ = TextSemanticAction($2, 3); }
    | HEADER_1 VARIABLE       { $$ = TextSemanticAction($2, 1); }
    | HEADER_2 VARIABLE       { $$ = TextSemanticAction($2, 2); }
    | HEADER_3 VARIABLE       { $$ = TextSemanticAction($2, 3); }
    | QUOTED_VALUE            { $$ = TextSemanticAction($1, 0); }
    | VARIABLE                { $$ = TextSemanticAction($1, 0); }
;


image:
    IMG OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN
    {  $$ = ImageSemanticAction($3 , $5);  }
;

form:
      FORM OPEN_BRACKET style_list CLOSE_BRACKET OPEN_BRACE style_list CLOSE_BRACE form_item_list END
        { $$ = FormSemanticAction($3, $6, $8); }

    | FORM OPEN_BRACE style_list CLOSE_BRACE OPEN_BRACKET style_list CLOSE_BRACKET form_item_list END
        { $$ = FormSemanticAction($6, $3, $8); }

    | FORM OPEN_BRACKET style_list CLOSE_BRACKET form_item_list END
        { $$ = FormSemanticAction($3, NULL, $5); }

    | FORM OPEN_BRACE style_list CLOSE_BRACE form_item_list END
        { $$ = FormSemanticAction(NULL, $3, $5); }

    | FORM form_item_list END
        { $$ = FormSemanticAction(NULL, NULL, $2); }
;



form_item_list:
      form_item { $$ = SingleListItemNodeSemanticAction($1); }
    | form_item_list form_item { $$ = AppendListItemNodeSemanticAction($1, $2); }
;

form_item:
    ITEM OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN
    {
        ParameterList* params = SingleParameterSemanticAction("label", NULL, $3);
        params = AppendParameterSemanticAction(params, "placeholder", NULL, $5);
        $$ = ListItemSemanticActionWithParameters(params);
    }
;

footer:
      FOOTER OPEN_BRACE style_list CLOSE_BRACE content_list END
        { $$ = FooterSemanticAction($3, $5); }
    | FOOTER content_list END
        { $$ = FooterSemanticAction(NULL, $2); }
;


row:
    ROW content_list END
    { $$ = RowSemanticAction($2); }
;

column:
      COLUMN OPEN_BRACE style_list CLOSE_BRACE content_list END
        { $$ = ColumnSemanticAction($3, $5); }
    | COLUMN content_list END
        { $$ = ColumnSemanticAction(NULL, $2); }
;


nav:
      NAV OPEN_BRACE style_list CLOSE_BRACE nav_item_list END
        { $$ = NavSemanticAction($3, $5); }
    | NAV nav_item_list END
        { $$ = NavSemanticAction(NULL, $2); }
;



nav_item_list:
      nav_item { $$ = SingleListItemNodeSemanticAction($1); }
    | nav_item_list nav_item { $$ = AppendListItemNodeSemanticAction($1, $2); }
;

nav_item:
    ITEM OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN
    {
        ParameterList* params = SingleParameterSemanticAction("label", NULL, $3);
        params = AppendParameterSemanticAction(params, "link", NULL, $5);
        $$ = ListItemSemanticActionWithParameters(params);
    }
;


button:
      BUTTON OPEN_BRACKET style_list CLOSE_BRACKET OPEN_BRACE style_list CLOSE_BRACE content_list END
        { $$ = ButtonWithAttrsSemanticAction($3, $6, $8); }

    | BUTTON OPEN_BRACE style_list CLOSE_BRACE OPEN_BRACKET style_list CLOSE_BRACKET content_list END
        { $$ = ButtonWithAttrsSemanticAction($6, $3, $8); }

    | BUTTON OPEN_BRACE style_list CLOSE_BRACE content_list END
        { $$ = ButtonSemanticAction($3, $5); }

    | BUTTON OPEN_BRACKET style_list CLOSE_BRACKET content_list END
        { $$ = ButtonWithAttrsSemanticAction($3, NULL, $5); }

    | BUTTON content_list END
        { $$ = ButtonWithAttrsSemanticAction(NULL, NULL, $2); }
;




card:
      CARD OPEN_BRACE style_list CLOSE_BRACE content_list END
        { $$ = CardSemanticAction($3, $5); }
    | CARD content_list END
        { $$ = CardSemanticAction(NULL, $2); }
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
