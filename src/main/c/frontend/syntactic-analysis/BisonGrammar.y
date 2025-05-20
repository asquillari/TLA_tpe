%{
#include "BisonActions.h"
%}

%define api.value.union.name SemanticValue

%union {
	Token token;
	char* string;
	int integer;


	struct Program* program;
    struct Statement* statement;
    struct StatementList* statement_list;
    struct ParameterList* parameter_list;

	struct Define* define;
	struct Use* use;
	struct Form* form;
    struct FormItem* form_item;
	struct Footer* footer;
	struct Row* row;
	struct Column* column;
	struct Nav* nav;
    struct NavItem* nav_item;
	struct Text* text;
	struct Image* image;
	struct Button* button;
	struct Card* card;
	struct OrderedList* ordered_list;
	struct UnorderedList* unordered_list;
	struct Table* table;
}

%token <token> DEFINE USE FORM IMG FOOTER ROW COLUMN NAV ITEM END BUTTON CARD LIST_BEGIN TABLE_BEGIN
%token <token> OPEN_PAREN CLOSE_PAREN OPEN_BRACE CLOSE_BRACE COLON COMMA OPEN_BRACKET CLOSE_BRACKET PIPE 
%token <token> NEWLINE HEADER_1 HEADER_2 HEADER_3

%token <token>  UNKNOWN EQUALS

%token <string> ORDERED_ITEM BULLET
%token <string> QUOTED_VALUE UNQUOTED_VALUE IDENTIFIER 
%token <string> VARIABLE


%type <program> program
%type <statement> statement 
%type <statement_list> statement_list content maybe_content column_list

%type <parameter_list> style_parameters action_parameters
%type <parameter_list> style_parameter_list identifier_list parameters use_parameters use_parameter_list
%type <parameter_list> maybe_style maybe_action maybe_parameters maybe_use

%type <form_item> form_item form_item_list
%type <nav_item> nav_item nav_item_list

%type <statement> define
%type <statement> use
%type <statement> form
%type <statement> footer
%type <statement> row
%type <statement> column
%type <statement> nav
%type <statement> image
%type <statement> text
%type <statement> button
%type <statement> card
%type <statement> table
%type <statement> ordered_list
%type <statement> unordered_list



%%

program:
    statement_list  { $$ = StatementSemanticAction(currentCompilerState(), $1); }
  ;

statement_list:
    statement { $$ = createSingleStatementList($1); }
  | statement_list statement { $$ = appendStatementToList($1, $2); }
  ;


statement:
    define { $$ = $1; }
    | use { $$ = $1; }
    | form { $$ = $1; }
    | footer { $$ = $1; }
    | row { $$ = $1; }
    | nav { $$ = $1; }
    | image { $$ = $1; }
    | text { $$ = $1; }
    | button { $$ = $1; }
    | card { $$ = $1; }
    | table { $$ = NULL; }
    | ordered_list { $$ = NULL; }
    | unordered_list { $$ = NULL; }
;

define:
    DEFINE IDENTIFIER maybe_parameters maybe_style statement_list END {
        $$ = DefineSemanticAction($2, $3, $4, $5);
    }
;


maybe_parameters:
      /* vacío */ { $$ = NULL; }
    | parameters { $$ = $1; }
;

parameters:
    OPEN_PAREN identifier_list CLOSE_PAREN { $$ = $2; }
;

identifier_list:
    IDENTIFIER {
        ParameterList* list = createParameterList();
        appendParameter(list, $1, NULL); 
        $$ = list;
    }
  | IDENTIFIER COMMA identifier_list {
        appendParameter($3, $1, NULL);
        $$ = $3;
    }
;


style_parameters:
    OPEN_BRACE style_parameter_list CLOSE_BRACE { $$ = $2; }
  ;

style_parameter_list:
    /* vacío */ { $$ = createParameterList(); }
  | style_parameter_list IDENTIFIER COLON UNQUOTED_VALUE {
        appendParameter($1, $2, $4); $$ = $1;
    }
  ;

use:
    USE IDENTIFIER maybe_use {
        $$ = UseSemanticAction($2, $3);
    }
;

maybe_use:
      /* vacío */ { $$ = NULL; }
    | use_parameters { $$ = $1; }

use_parameters:
    OPEN_PAREN use_parameter_list CLOSE_PAREN { $$ = $2; }
;

use_parameter_list:
      QUOTED_VALUE {
          ParameterList* list = createParameterList();
          appendParameter(list, NULL, $1); 
          $$ = list;
      }
    | QUOTED_VALUE COMMA use_parameter_list {
          appendParameter($3, NULL, $1); 
          $$ = $3;
      }
;


form:
    FORM maybe_style maybe_action form_item_list END {
        $$ = FormSemanticAction($2, $3, $4);
    }
;

form_item_list:
    /* vacío */ { $$ = NULL; }
  | form_item_list form_item {
        $$ = appendFormItem($1, $2);
    }
;

form_item:
    ITEM OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN {
        $$ = FormItemSemanticAction($3, $5); // label, placeholder
    }
;



action_parameters:
    OPEN_BRACKET style_parameter_list CLOSE_BRACKET { $$ = $2; }
  ;



footer:
    FOOTER style_parameters statement_list END {
        $$ = FooterSemanticAction($2, $3);
    }


row:
    ROW maybe_style column_list END {
        $$ = RowSemanticAction($2, $3);
    }
;

column_list:
    column { $$ = createSingleStatementList($1); }
  | column_list column {
        $$ = appendStatementToList($1, $2);
    }
;

column:
    COLUMN style_parameters statement_list END {
        $$ = ColumnSemanticAction($2, $3);
    }
;


nav:
    NAV maybe_style maybe_action nav_item_list END {
        $$ = NavSemanticAction($2, $3, $4);
    }
;

nav_item_list:
    /* vacío */ { $$ = NULL; }
  | nav_item_list nav_item {
        $$ = appendNavItem($1, $2);
    }
;

nav_item:
    ITEM OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN {
        $$ = NavItemSemanticAction($3, $5); // label, link
    }
;



image:
    IMG maybe_style OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN
        { $$ = ImageSemanticAction($2, $4, $6); }
;

maybe_style:
      /* vacío */                    { $$ = NULL; }
    | style_parameters               { $$ = $1; }
;


text:
      HEADER_1 QUOTED_VALUE   { $$ = TextFromQuotedValueSemanticAction($2); }
    | HEADER_2 QUOTED_VALUE   { $$ = TextFromQuotedValueSemanticAction($2); }
    | HEADER_3 QUOTED_VALUE   { $$ = TextFromQuotedValueSemanticAction($2); }
    | HEADER_1 VARIABLE       { $$ = TextFromVariableSemanticAction($2); }
    | HEADER_2 VARIABLE       { $$ = TextFromVariableSemanticAction($2); }
    | HEADER_3 VARIABLE       { $$ = TextFromVariableSemanticAction($2); }
    | QUOTED_VALUE            { $$ = TextFromQuotedValueSemanticAction($1); }
    | VARIABLE                { $$ = TextFromVariableSemanticAction($1); }


button:
    BUTTON maybe_style maybe_action maybe_content END {
        $$ = ButtonSemanticAction($2, $3, $4);
    }
;

maybe_action:
    /* vacío */ { $$ = NULL; }
  |  action_parameters  { $$ = $1; }
  ;


maybe_content:
    /* vacío */ { $$ = NULL; }
  | content     { $$ = $1; }
;

content:
    text { $$ = createSingleStatementList($1); }
  | content text { $$ = appendStatementToList($1, $2); }
;


card:
    CARD maybe_style maybe_content END { $$ = CardSemanticAction($2, $3); }


table:
    TABLE_BEGIN style_parameters table_items END { $$ = NULL; }

table_items:
    table_row_list
  ;

table_row_list:
    table_row
  | table_row_list table_row
  ;

table_row:
    PIPE table_cell_list PIPE NEWLINE
  ;

table_cell_list:
    content
  | table_cell_list PIPE content
  ;


ordered_list:
    LIST_BEGIN style_parameters ordered_list_items END { $$ = NULL; }

ordered_list_items:
    ordered_list_items ORDERED_ITEM text
  | ORDERED_ITEM text
  ;

unordered_list:
    LIST_BEGIN style_parameters unordered_list_items END { $$ = NULL; }


unordered_list_items:
    unordered_list_items BULLET text
  | BULLET text
  ;
